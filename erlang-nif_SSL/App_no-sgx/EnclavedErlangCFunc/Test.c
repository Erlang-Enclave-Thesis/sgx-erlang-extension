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

#include "Enclave.cpp"
#include "Test.h"

//NOTE these errors are not SGX related anymore! We changed these values.
#define SGX_ERROR_UNEXPECTED 1
#define SGX_SUCCESS 0

void my_increment(void)
{
    int32_t ret = SGX_SUCCESS;
    ecall_increment();
}

int32_t my_return_a(void)
{
    int32_t ret = SGX_ERROR_UNEXPECTED;
    int32_t num = ecall_return_a();
    return num;
}

//Gets plaintext and populates ciphertext (the signature) memory accordingly
int32_t my_rsa_sign(const unsigned char* const plaintext, unsigned char* const ciphertext, unsigned int ciphertext_len){
    int32_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_rsa_sign(plaintext, ciphertext, ciphertext_len);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return ret;
}
//Gets plaintext and populates ciphertext memory accordingly
int32_t my_rsa_encrypt(const unsigned char* const plaintext, unsigned char* const ciphertext, int ciphertext_len){
    int32_t ret = SGX_ERROR_UNEXPECTED;
    int32_t status;
    ret = ecall_rsa_encrypt(plaintext, ciphertext, ciphertext_len);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return ret;
}

int32_t my_rsa_get_pubkey(unsigned char* const pubkey, int requested_len){
    int32_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_rsa_get_pubkey(pubkey, requested_len);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return ret;
}

int32_t my_rsa_key_gen(){
    int32_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_rsa_key_gen();
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return ret;
}

