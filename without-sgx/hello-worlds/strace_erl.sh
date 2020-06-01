#!/bin/bash -e
# Commands used for simple strace output
# full output: strace.txt
# summary: strace_summary.txt

# BSD 3-Clause License
#
# Copyright (c) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


rm hello_world.beam 2>/dev/null || true
erl -compile hello_world_threaded.erl
#strace -f -t -o data/strace.txt erl -noshell -s hello_world init -s init stop
strace -f -c -o data/strace_erlang_helloworldthreaded.txt erl -noshell -s hello_world_threaded init -s init stop
strace -f -c -o data/strace_erlang_helloworldthreaded_2.txt erl -noshell -s hello_world_threaded init -s init stop
echo

#TODO this is not reliable, use awk to count lines instead
let UNIQUE_CALLS=$( wc -l < data/strace_erlang_helloworldthreaded.txt  )-4
cat data/strace_erlang_helloworldthreaded.txt
echo "Unique syscalls: ~${UNIQUE_CALLS}"
echo; echo
let UNIQUE_CALLS=$( wc -l < data/strace_erlang_helloworldthreaded_2.txt  )-4
cat data/strace_erlang_helloworldthreaded_2.txt
echo "Unique syscalls: ~${UNIQUE_CALLS}"
echo; echo

# Bonus for debugging: track all processes related to the started Erlang workload, save to separate file for each PID, time in microseconds
#strace -ff -tt -o strace erl -noshell -s hello_world init -s init stop

