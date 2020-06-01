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
-export([increment/0, return/0, run_tests/0]).

increment() ->
    call_cnode({increment}).

return() ->
    call_cnode({return}).

call_cnode(Msg) ->
    {any, 'c1@elxag18nn12'} ! {call, self(), Msg},
    receive
	    {cnode, Result} ->
	        Result
    end.


run_tests() ->
    test_time(increment),
    test_time(return).

test_time(Function) ->
    Iterations = 30,
    io:fwrite("~nCalling ~w, ~w times~n",[Function,Iterations]),
    {Results, Sum} = test_time_helper([],0,Iterations,Function),
    Average = Sum/Iterations,
    io:fwrite("Average time: ~w μs~n",[Average]),
    io:fwrite("Average time excluding first element: ~w μs~n",[(Sum-(lists:nth(1,Results)))/(Iterations-1)]),
    io:fwrite("Raw data: ~w~n",[Results]).

test_time_helper(Results,Sum,0,_) ->
    {Results, Sum};

test_time_helper(Results,Sum,Count,Function) ->
    Time = measure_time(Function),
    test_time_helper(lists:append(Results,[Time]), Sum+Time, Count-1, Function).

measure_time(Function) ->
    {Time, _} = timer:tc(enclave_communicator,Function,""),
    Time.
