%  BSD 3-Clause License
%
%  Copyright (c) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are met:
%
%  1. Redistributions of source code must retain the above copyright notice, this
%     list of conditions and the following disclaimer.
%
%  2. Redistributions in binary form must reproduce the above copyright notice,
%     this list of conditions and the following disclaimer in the documentation
%     and/or other materials provided with the distribution.
%
%  3. Neither the name of the copyright holder nor the names of its
%     contributors may be used to endorse or promote products derived from
%     this software without specific prior written permission.
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
%  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
%  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
%  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
%  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
%  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
%  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
%  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-module(enclave_communicator).
-export([increment/0, rtrn/0, rsa_sign/1, rsa_encrypt/1, rsa_get_pubkey/0, rsa_key_gen/0, rsa_basic_operations/1,test_rsa/0, eval_rsa/0, eval_increment/0]).
-on_load(init/0).

init() ->
    ok = erlang:load_nif("./app", 0).

increment() ->
    exit(nif_library_not_loaded).

rtrn() ->
    exit(nif_library_not_loaded).

%NOTE: the RSA keypair is newly generated and overwritten for each call to rsa_key_gen
rsa_key_gen() ->
    exit(nif_library_not_loaded).

rsa_get_pubkey() ->
    exit(nif_library_not_loaded).

rsa_sign(_PAYLOAD) ->
    exit(nif_library_not_loaded).

rsa_encrypt(_PAYLOAD) ->
    exit(nif_library_not_loaded).


%Test all RSA functions
test_rsa() ->
	Payload = "My data is this!",
	0 = rsa_key_gen(), %abort if return code != 0
	Signed  = rsa_sign(Payload),
	Encrypted = rsa_encrypt(Payload),
	Pubkey  = rsa_get_pubkey(),
	io:fwrite("Cleartext: ~p~nSigned data: ~w~nEncryped data: ~w~nPublic Key: ~w~n",[Payload,Signed,Encrypted,Pubkey]).

%Primary RSA operations for evaluation
rsa_basic_operations(Payload) ->
	0 = rsa_key_gen(),
	rsa_sign(Payload),
	rsa_encrypt(Payload).

%Measure execution time for increment functions
eval_increment() ->
    test_time(increment,""),
    test_time(rtrn,"").

%Measure execution time for primary RSA operations
eval_rsa() ->
    io:fwrite("~nRunning RSA keygen, sign, and encrypt together"),
    test_time(rsa_basic_operations, "DATA TO ENCRYPT/SIGN.."),
    io:fwrite("~nRunning RSA these functions separately"),
    test_time(rsa_key_gen, ""),
    test_time(rsa_sign, "DATA.."),
    test_time(rsa_encrypt, "DATA..").


% -----------------------------------------------------------------------------------------------
% -- test_time (and its helper functions) to measure execution time for a certain amount of runs --
% -----------------------------------------------------------------------------------------------

test_time(Function,FuncInput) ->
    Iterations = 30,
    io:fwrite("~nCalling ~w, ~w times~n",[Function,Iterations]),
    {Results, Sum} = test_time_helper([],0,Iterations,Function,FuncInput),
    Average = Sum/Iterations,
    io:fwrite("Average time: ~w μs~n",[Average]),
    io:fwrite("Average time excluding first element: ~w μs~n",[(Sum-(lists:nth(1,Results)))/(Iterations-1)]),
    io:fwrite("Raw data: ~w~n",[Results]).

test_time_helper(Results,Sum,0,_,_) ->
    {Results, Sum};

test_time_helper(Results,Sum,Count,Function,FuncInput) ->
    Time = measure_time(Function,FuncInput),
    test_time_helper(lists:append(Results,[Time]), Sum+Time, Count-1, Function, FuncInput).

measure_time(Function, []) ->
    {Time, _} = timer:tc(enclave_communicator,Function, []),
    Time;

measure_time(Function, Input) ->
    {Time, _} = timer:tc(enclave_communicator,Function,[Input]),
    Time.

% -----------------------------------------------------------------------------------------------
% -----------------------------------------------------------------------------------------------

