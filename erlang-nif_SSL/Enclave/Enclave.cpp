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
 *
 */

#include "Enclave.h"
#include "Enclave_t.h" /* print_string */
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>

#include <string.h>

#include "sgx_trts.h"

// OpenSSL //

#include <tSgxSSL_api.h>

#include <openssl/ec.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#define ADD_ENTROPY_SIZE        32


// -- Variables for Erlang NIF functionality -- //
// TODO this is probably highly unsafe! (private key here, not freed)
//Variable returned to Erlang
static int32_t a = 0;
static EVP_PKEY *evp_pkey;
static RSA *keypair;
// -- END Variables for Erlang NIF functionality -- //

/*
 * printf:
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
//NOTE: this can be enabled for debugging!
void printf(const char *fmt, ...)
{
    //char buf[BUFSIZ] = {'\0'};
    //va_list ap;
    //va_start(ap, fmt);
    //vsnprintf(buf, BUFSIZ, fmt, ap);
    //va_end(ap);
    //uprint(buf);
    return;
}

typedef void CRYPTO_RWLOCK;

struct evp_pkey_st {
    int type;
    int save_type;
    int references;
    const EVP_PKEY_ASN1_METHOD *ameth;
    ENGINE *engine;
    union {
        char *ptr;
        struct rsa_st *rsa;     /* RSA */
    } pkey;
    int save_parameters;
    STACK_OF(X509_ATTRIBUTE) *attributes; /* [ 0 ] */
    CRYPTO_RWLOCK *lock;
} /* EVP_PKEY */ ;



void rsa_key_gen()
{
        BIGNUM *bn = BN_new();
        if (bn == NULL) {
                printf("BN_new failure: %ld\n", ERR_get_error());
            return;
        }
        int ret = BN_set_word(bn, RSA_F4);
    if (!ret) {
        printf("BN_set_word failure\n");
            return;
        }

        keypair = RSA_new();
        if (keypair == NULL) {
                printf("RSA_new failure: %ld\n", ERR_get_error());
            return;
        }
        ret = RSA_generate_key_ex(keypair, 4096, bn, NULL);
        if (!ret) {
        printf("RSA_generate_key_ex failure: %ld\n", ERR_get_error());
            return;
        }

        evp_pkey = EVP_PKEY_new();
	//EVP_PKEY *evp_pkey = EVP_PKEY_new();

        if (evp_pkey == NULL) {
                printf("EVP_PKEY_new failure: %ld\n", ERR_get_error());
                return;
        }
        EVP_PKEY_assign_RSA(evp_pkey, keypair);

        // public key - string
        int len = i2d_PublicKey(evp_pkey, NULL);
        unsigned char *buf = (unsigned char *) malloc (len + 1);
        unsigned char *tbuf = buf;
        i2d_PublicKey(evp_pkey, &tbuf);

        // print public key
        printf ("{\"public\":\"");
        int i;
        for (i = 0; i < len; i++) {
            printf("%02x", (unsigned char) buf[i]);
        }
        printf("\"}\n");
	printf("\n\n\n%d\n\n\n",len);

        free(buf);

        // private key - string
        len = i2d_PrivateKey(evp_pkey, NULL);
        buf = (unsigned char *) malloc (len + 1);
        tbuf = buf;
        i2d_PrivateKey(evp_pkey, &tbuf);

        // print private key
        printf ("{\"private\":\"");
        for (i = 0; i < len; i++) {
            printf("%02x", (unsigned char) buf[i]);
        }
	
        printf("\"}\n");

        free(buf);

        BN_free(bn);

	//TODO: WARNING: We removed this for now as to enable subsequent calls by NIF
	//		  to use this key! Is not freed!
        //EVP_PKEY_free(evp_pkey);

        if (evp_pkey->pkey.ptr != NULL) {
          RSA_free(keypair);
        }

	a = 0; //OUR TEST THING (0=SUCCESS)
}


int vprintf_cb(Stream_t stream, const char * fmt, va_list arg)
{
        char buf[BUFSIZ] = {'\0'};

        int res = vsnprintf(buf, BUFSIZ, fmt, arg);
        if (res >=0) {
                sgx_status_t sgx_ret = uprint((const char *) buf);
                TEST_CHECK(sgx_ret);
        }
        return res;
}

void t_sgxssl_call_apis()
{
    int ret = 0;
    
    printf("Starting tests\n");
    
    SGXSSLSetPrintToStdoutStderrCB(vprintf_cb);

    //CRYPTO_set_mem_functions(priv_malloc, priv_realloc, priv_free);

    // Initialize SGXSSL crypto
    OPENSSL_init_crypto(0, NULL);
    
    rsa_key_gen();
    //ret = rsa_test();
}

// END OF OpenSSL //


// For Erlang NIFs //
void ecall_increment(void) {
	a++;
	int32_t ret=0;
}

int32_t ecall_return_a(void) {
    return a;
}


//writes signed message (ciphertext) to given address
int32_t ecall_rsa_sign(const unsigned char* const plaintext, unsigned char*const ciphertext, unsigned int ciphertext_len) {
    int32_t ret = 0;

    // -- RSA Encryption -- //
    unsigned int len = ciphertext_len;
    unsigned int plen = (unsigned int) sizeof(plaintext);
    int type = NID_sha1;

    ret = RSA_sign(type, plaintext, plen, ciphertext, &len, keypair);

    if(ret != 1){
	    printf("RSA_sign error %d",ret);
	    return 1; //error
    }
    else
	    ret = 0; //success

    return ret;
}

//writes encrypted message to given address
int32_t ecall_rsa_encrypt(const unsigned char* const plaintext, unsigned char*const ciphertext, int ciphertext_len) {
    int32_t ret = 0;
    int expected_csize = 512; //by default we expect 512B

    int len = ciphertext_len;
    
    int plen = sizeof(plaintext);
    int num = RSA_public_encrypt(plen, plaintext, ciphertext, keypair,
                                 RSA_PKCS1_PADDING);

    //num is size of encrypted data
    if(num != expected_csize){
	    printf("RSA_public_encrypt error, expected %dB got %d",
			    expected_csize,num);
	    return 1;
    }
    return ret;
}

// Returns pubkey created by ecall_rsa_key_gen, to provided pointer of length 526+1
int ecall_rsa_get_pubkey(unsigned char* const pubkey_return_ptr, int requested_len) {
    int32_t ret = 0;
    if (evp_pkey == NULL) {
        printf("EVP_PKEY_new failure: %ld\n", ERR_get_error());
        return 1;
    }
    // public key - string
    // NOTE performed len + 1 here, unlike original code
    int len = i2d_PublicKey(evp_pkey, NULL); // + 1;
    unsigned char *buf = (unsigned char *) malloc (len);
    unsigned char *tbuf = buf;
    i2d_PublicKey(evp_pkey, &tbuf);

    if(len != requested_len){
        printf("ECALL requested incorrect RSA key size (%d != %d)\n",
			len,requested_len);
	return 2;
    }

    //Save public key to UNTRUSTED MEMORY
    memcpy(pubkey_return_ptr, buf, len);
  
    free(buf);
    return ret;
}

int32_t ecall_rsa_key_gen(void) {
    int32_t ret;
    SGXSSLSetPrintToStdoutStderrCB(vprintf_cb);

    // Initialize SGXSSL crypto
    ret = OPENSSL_init_crypto(0, NULL);

    if(ret == 1)
	    ret = 0; //success
    else
	    ret = 1; //error

    rsa_key_gen();
    return ret;
}
// END For Erlang NIFs //
