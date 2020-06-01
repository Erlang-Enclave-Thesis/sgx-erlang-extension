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


#include "Test.h"

void my_increment(void)
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_increment(global_eid);
    if (ret != SGX_SUCCESS) {
        abort();
    }
}

int32_t my_return_a(void)
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int32_t a;
    ret = ecall_return_a(global_eid, &a);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return a;
}

//Gets plaintext and populates ciphertext (the signature) memory accordingly
int32_t my_rsa_sign(const unsigned char* const plaintext, unsigned char* const ciphertext, unsigned int ciphertext_len){
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int32_t status;
    ret = ecall_rsa_sign(global_eid, &status, plaintext, ciphertext, ciphertext_len);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return status;
}
//Gets plaintext and populates ciphertext memory accordingly
int32_t my_rsa_encrypt(const unsigned char* const plaintext, unsigned char* const ciphertext, int ciphertext_len){
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int32_t status;
    ret = ecall_rsa_encrypt(global_eid, &status, plaintext, ciphertext, ciphertext_len);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return status;
}

int32_t my_rsa_get_pubkey(unsigned char* const pubkey, int requested_len){
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int32_t status;
    ret = ecall_rsa_get_pubkey(global_eid, &status, pubkey, requested_len);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return status;
}

int32_t my_rsa_key_gen(){
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int32_t status;
    ret = ecall_rsa_key_gen(global_eid, &status);
    if (ret != SGX_SUCCESS) {
        abort();
    }
    return status;
}

