#
# Copyright (C) 2011-2020 Intel Corporation. All rights reserved.
# Copyright (C) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB (BSD 3-Clause Licensed as per separate LICENSE file)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#   * Neither the name of Intel Corporation nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

######## App Settings ########

# App_Cpp_Files := $(wildcard App/EnclavedErlangCFunc/*.cpp)
App_C_Files := App/App.c $(wildcard App/EnclavedErlangCFunc/*.c)
App_Include_Paths := -IApp

App_C_Flags := -fPIC -Wno-attributes $(App_Include_Paths)

# Three configuration modes - Debug, prerelease, release
#   Debug - Macro DEBUG enabled.
#   Prerelease - Macro NDEBUG and EDEBUG enabled.
#   Release - Macro NDEBUG enabled.
#App_C_Flags += -DNDEBUG -UEDEBUG -UDEBUG

#App_Cpp_Flags := $(App_C_Flags)
#App_Link_Flags := -l$(Urts_Library_Name) -lpthread
App_Link_Flags := -lpthread

# App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)
App_C_Objects := $(App_C_Files:.c=.o)

App_Name := app.so

.PHONY: build

build: $(App_Name)
	#@

######## Default C/C++ to object files #########

.SUFFIXES: .c .o .erl .beam

.c.o:
	@echo "C Compling: $@ from $<"
	$(CC) $(App_C_Flags) -c $< -o $@

.cpp.o:
	@echo "CPP Compling: $@ from $<"
	$(CXX) $(App_CPP_Flags) -c $< -o $@

######## App Objects ########

App_C_Erl_Flags := -I/usr/local/lib/erlang/usr/include

App_Link_Erl_Flags := -L/usr/local/lib/erlang/usr/lib -lerl_interface -lei -lpthread

App/App.o: App/App.c
	$(CC) $(App_C_Flags) $(App_C_Erl_Flags) -c $< -o $@
	@echo "CC   <=  $<"

$(App_Name): App/App.o App/EnclavedErlangCFunc/Test.o
	$(CC) $(App_C_Flags) $(App_C_Erl_Flags) -shared $^ -o $@
	@echo "CC   <=  $<"

.PHONY: clean

clean:
	@rm -f .config_* $(App_Name) $(App_C_Objects) $(App_Cpp_Objects)
