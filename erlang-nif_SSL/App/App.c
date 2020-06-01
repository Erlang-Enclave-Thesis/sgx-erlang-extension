/*
 * Copyright (C) 2011-2020 Intel Corporation. All rights reserved.
 * Copyright (C) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB (BSD 3-Clause Licensed as per separate LICENSE file)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>
#define MAX_PATH FILENAME_MAX

#include <sgx_urts.h>
#include "Enclave_u.h"


//New includes
#include <libgen.h>
#include <stdlib.h>
#include <pthread.h>
//

// BEGIN FROM ERLANG
#include <erl_nif.h>
// END FROM ERLANG

#include "App.h"
#include "EnclavedErlangCFunc/Test.h"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

typedef struct _sgx_errlist_t {
	sgx_status_t err;
	const char *msg;
	const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] =
		{ { SGX_ERROR_UNEXPECTED, "Unexpected error occurred.",
		NULL }, { SGX_ERROR_INVALID_PARAMETER, "Invalid parameter.",
		NULL }, { SGX_ERROR_OUT_OF_MEMORY, "Out of memory.",
		NULL }, { SGX_ERROR_ENCLAVE_LOST, "Power transition occurred.",
				"Please refer to the sample \"PowerTransition\" for details." },
				{ SGX_ERROR_INVALID_ENCLAVE, "Invalid enclave image.",
				NULL }, { SGX_ERROR_INVALID_ENCLAVE_ID,
						"Invalid enclave identification.",
						NULL }, { SGX_ERROR_INVALID_SIGNATURE,
						"Invalid enclave signature.",
						NULL }, { SGX_ERROR_OUT_OF_EPC, "Out of EPC memory.",
				NULL },
				{ SGX_ERROR_NO_DEVICE, "Invalid SGX device.",
						"Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards." },
				{ SGX_ERROR_MEMORY_MAP_CONFLICT, "Memory map conflicted.",
				NULL }, { SGX_ERROR_INVALID_METADATA,
						"Invalid enclave metadata.",
						NULL }, { SGX_ERROR_DEVICE_BUSY, "SGX device was busy.",
				NULL }, { SGX_ERROR_INVALID_VERSION,
						"Enclave version was invalid.",
						NULL }, { SGX_ERROR_INVALID_ATTRIBUTE,
						"Enclave was not authorized.",
						NULL }, { SGX_ERROR_ENCLAVE_FILE_ACCESS,
						"Can't open enclave file.",
						NULL }, };

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret) {
	size_t idx = 0;
	size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

	for (idx = 0; idx < ttl; idx++) {
		if (ret == sgx_errlist[idx].err) {
			if (NULL != sgx_errlist[idx].sug)
				printf("Info: %s\n", sgx_errlist[idx].sug);
			printf("Error: %s\n", sgx_errlist[idx].msg);
			break;
		}
	}

	if (idx == ttl)
		printf(
				"Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n",
				ret);
}

/* Initialize the enclave:
 *   Call sgx_create_enclave to initialize an enclave instance
 */
//int initialize_enclave(void) {
//	sgx_status_t ret = SGX_ERROR_UNEXPECTED;
//
//	/* Call sgx_create_enclave to initialize an enclave instance */
//	/* Debug Support: set 2nd parameter to 1 */
//	ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, NULL, NULL,
//			&global_eid, NULL); // @suppress("Symbol is not resolved")
//	if (ret != SGX_SUCCESS) {
//		print_error_message(ret);
//		return -1;
//	}
//	return 0;
//}

/* Initialize the enclave:
 *   Step 1: retrive the launch token saved by last transaction
 *   Step 2: call sgx_create_enclave to initialize an enclave instance
 *   Step 3: save the launch token if it is updated
 */
int initialize_enclave(void)
{
    char token_path[MAX_PATH] = {'\0'};
    sgx_launch_token_t token = {0};
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int updated = 0;
    /* Step 1: retrive the launch token saved by last transaction */

    /* try to get the token saved in $HOME */
    const char *home_dir = getpwuid(getuid())->pw_dir;
    if (home_dir != NULL &&
        (strlen(home_dir)+strlen("/")+sizeof(TOKEN_FILENAME)+1) <= MAX_PATH) {
        /* compose the token path */
        strncpy(token_path, home_dir, strlen(home_dir));
        strncat(token_path, "/", strlen("/"));
        strncat(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME)+1);
    } else {
        /* if token path is too long or $HOME is NULL */
        strncpy(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME));
    }

    FILE *fp = fopen(token_path, "rb");
    if (fp == NULL && (fp = fopen(token_path, "wb")) == NULL) {
        printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
    }
    printf("token_path: %s\n", token_path);
    if (fp != NULL) {
        /* read the token from saved file */
        size_t read_num = fread(token, 1, sizeof(sgx_launch_token_t), fp);
        if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
            /* if token is invalid, clear the buffer */
            memset(&token, 0x0, sizeof(sgx_launch_token_t));
            printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
        }
    }

    /* Step 2: call sgx_create_enclave to initialize an enclave instance */
    /* Debug Support: set 2nd parameter to 1 */

    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);

    if (ret != SGX_SUCCESS) {
        print_error_message(ret);
        if (fp != NULL) fclose(fp);

        return -1;
    }

    /* Step 3: save the launch token if it is updated */

    if (updated == FALSE || fp == NULL) {
        /* if the token is not updated, or file handler is invalid, do not perform saving */
        if (fp != NULL) fclose(fp);
        return 0;
    }

    /* reopen the file with write capablity */
    fp = freopen(token_path, "wb", fp);
    if (fp == NULL) return 0;
    size_t write_num = fwrite(token, 1, sizeof(sgx_launch_token_t), fp);
    if (write_num != sizeof(sgx_launch_token_t))
        printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
    fclose(fp);

    return 0;
}

/* OCall functions */
void uprint(const char *str)
{
    /* Proxy/Bridge will check the length and null-terminate
     * the input string to prevent buffer overflow.
     */
    printf("%s", str);
    fflush(stdout);
}


void usgx_exit(int reason)
{
        printf("usgx_exit: %d\n", reason);
        exit(reason);
}


void* thread_test_func(void* p)
{
        new_thread_func(global_eid);
        return NULL;
}

int ucreate_thread()
{
        pthread_t thread;
        int res = pthread_create(&thread, NULL, thread_test_func, NULL);
        return res;
}




/* OCall functions */
void ocall_print_string(const char *str) {
	/* Proxy/Bridge will check the length and null-terminate
	 * the input string to prevent buffer overflow.
	 */
	printf("%s", str);
}

// BEGIN FROM ERLANG
static int load(ErlNifEnv* caller_env, void** priv_data, ERL_NIF_TERM load_info) {
	/* Changing dir to where the executable is.*/
	//TODO This is from Intel's example code, do we need it?
	//char absolutePath[MAX_PATH];
	//char *ptr = NULL;
	//
	//ptr = realpath(dirname(argv[0]), absolutePath);
	//
	//if (ptr == NULL || chdir(absolutePath) != 0)
	//	return 1;
	//
	printf("HELLO LOAD!\n");

	/* Initialize the enclave */
	if (initialize_enclave() < 0) {
		printf("Something went wrong!\n");
		return -1;
	}

	//TODO below can be added to ensure SSL is working. Probably not at all necessary to run here?
   	//sgx_status_t status = t_sgxssl_call_apis(global_eid);
   	//if (status != SGX_SUCCESS) {
   	//    printf("Call to t_sgxssl_call_apis has failed.\n");
   	//    return 1;    //Test failed
   	//}


	return 0;
}

static void unload(ErlNifEnv* caller_env, void* priv_data) {
	/* Destroy the enclave */
	sgx_destroy_enclave(global_eid);

	printf("HELLO UNLOAD!\n");
}

static int upgrade(ErlNifEnv* caller_env, void** priv_data, void** old_priv_data, ERL_NIF_TERM load_info) {
	printf("HELLO UPGRADE!\n");
	// Unload probably shouldn't be called here since Erlang keeps two versions running simultanously.
	// Unload will automatically be called when two reloads have been called, killing the oldest version.
	return load(caller_env, priv_data, load_info);
}

static ERL_NIF_TERM increment_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    my_increment();
    return enif_make_int(env, 0);
}

static ERL_NIF_TERM rtrn_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int ret;
    ret = my_return_a();
    return enif_make_int(env, ret);
}

static ERL_NIF_TERM rsa_sign_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    //The expected size in bytes of the RSA signature (w/ NID_sha1)
    #define BUFFER_SIZE 512

    // -- Get input from Erlang -- //

    unsigned int len;
    if(!enif_get_list_length(env,argv[0],&len))
        return;
    len++; //for null termination
    const unsigned char const cleartext[len]; //payload
    if(enif_get_string(env,argv[0],cleartext,len,ERL_NIF_LATIN1) < 1)
        return;

    // -- Get result (signature) from enclave -- //

    unsigned char* const ciphertext = malloc(BUFFER_SIZE * sizeof(char));
    int32_t ret = my_rsa_sign(cleartext, ciphertext, BUFFER_SIZE);

    // -- Return result (signature) to Erlang -- //

    ERL_NIF_TERM termp;
    unsigned char* bin = enif_make_new_binary(env,(BUFFER_SIZE)*sizeof(char), &termp);
    memcpy(bin,ciphertext,(BUFFER_SIZE)*sizeof(char));
    free(ciphertext);

    return termp;
}

static ERL_NIF_TERM rsa_encrypt_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    //The expected size in bytes of the RSA ciphertext (w/ RSA_PKCS1_PADDING)
    #define BUFFER_SIZE 512

    // -- Get input from Erlang -- //

    unsigned int len;
    if(!enif_get_list_length(env,argv[0],&len))
        return;
    len++; //for null termination
    const unsigned char const cleartext[len]; //payload
    if(enif_get_string(env,argv[0],cleartext,len,ERL_NIF_LATIN1) < 1)
        return;

    // -- Get result (ciphertext) from enclave -- //

    unsigned char* const ciphertext = malloc(BUFFER_SIZE * sizeof(char));
    int32_t ret = my_rsa_encrypt(cleartext, ciphertext, BUFFER_SIZE);

    // -- Return result (ciphertext) to Erlang -- //

    ERL_NIF_TERM termp;
    unsigned char* bin = enif_make_new_binary(env,(BUFFER_SIZE)*sizeof(char), &termp);
    memcpy(bin,ciphertext,(BUFFER_SIZE)*sizeof(char));
    free(ciphertext);

    return termp;
}

static ERL_NIF_TERM rsa_get_pubkey_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    //The size of the generated RSA public key is by default 526 chars (bytes)
    #define BUFFER_SIZE 526
    int32_t ret = 0;

    char * const pubkey = malloc(BUFFER_SIZE * sizeof(char));

    ret = my_rsa_get_pubkey(pubkey, BUFFER_SIZE);

    //return public key as Erlang Term (without null termination)
    ERL_NIF_TERM termp;
    unsigned char* bin = enif_make_new_binary(env,(BUFFER_SIZE)*sizeof(char), &termp);
    memcpy(bin,pubkey,(BUFFER_SIZE)*sizeof(char));
    free(pubkey);

    return termp;
}

//Generate 4096 bit RSA key, uses example code from Intel (RSA_key_gen)
static ERL_NIF_TERM rsa_key_gen_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int ret;
    ret = my_rsa_key_gen();
    return enif_make_int(env, ret);
}


static ErlNifFunc nif_funcs[] = {
    {"increment", 0, increment_nif},
    {"rtrn", 0, rtrn_nif},
    {"rsa_sign", 1, rsa_sign_nif},
    {"rsa_encrypt", 1, rsa_encrypt_nif},
    {"rsa_get_pubkey", 0, rsa_get_pubkey_nif},
    {"rsa_key_gen", 0, rsa_key_gen_nif}
};

ERL_NIF_INIT(enclave_communicator, nif_funcs, load, NULL, upgrade, unload)
// END FROM ERLANG

