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

// BEGIN FROM ERLANG
#include <erl_nif.h>
// END FROM ERLANG

#include "App.h"
#include "EnclavedErlangCFunc/Test.h"

// BEGIN FROM ERLANG
static int load(ErlNifEnv* caller_env, void** priv_data, ERL_NIF_TERM load_info) {
	printf("HELLO LOAD!\n");

	return 0;
}

static void unload(ErlNifEnv* caller_env, void* priv_data) {

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

static ErlNifFunc nif_funcs[] = {
    {"increment", 0, increment_nif},
    {"rtrn", 0, rtrn_nif}
};

ERL_NIF_INIT(enclave_communicator, nif_funcs, load, NULL, upgrade, unload)
// END FROM ERLANG

