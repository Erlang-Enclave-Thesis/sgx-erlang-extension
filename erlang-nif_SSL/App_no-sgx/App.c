/* BSD 3-Clause License
 *
 * Copyright (c) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>
#define MAX_PATH FILENAME_MAX

//New includes
#include <libgen.h>
#include <stdlib.h>
#include <pthread.h>
//

// For Erlang
#include <erl_nif.h>

#include "App.h"
#include "EnclavedErlangCFunc/Test.h"

/* OCall function */
void uprint(const char *str)
{
    printf("%s", str);
    fflush(stdout);
}

/* OCall function */
void ocall_print_string(const char *str) {
	printf("%s", str);
}

// -- BEGIN FOR ERLANG --

static int load(ErlNifEnv* caller_env, void** priv_data, ERL_NIF_TERM load_info) {
	printf("HELLO LOAD!\n");
	return 0;
}

static void unload(ErlNifEnv* caller_env, void* priv_data) {
	//TODO free RSA resources here?

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
    int32_t ret;
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
// -- END FOR ERLANG --

