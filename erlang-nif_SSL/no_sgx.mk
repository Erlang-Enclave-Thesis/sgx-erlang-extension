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

#NOTE: this version runs WITHOUT sgx but is based on the
#	sgx version's makefile causing some confusion.
#
#	A reason for this is that we aim to keep this version as
#	similar to the sgx version as possible for benchmarking,
#	it also has the same folder structure and functionality.
#	What is referred to as 'enclave' here is simply the earlier
#	enclaved code which is now running in the regular OS instead.
#	Additionally, the SGX_ARCH and SGX_DEBUG flags are not
#	as important now but they do still change some settings
#	in the compilation of the C/C++ code.


#HARDCODED PATH! Where OpenSSL 1.1.1d is built
OPENSSL_LIBRARY_PATH := /home/ericsson/openssl/lib
OPENSSL_INCLUDES := /home/ericsson/openssl/include
##


SGX_ARCH ?= x64
SGX_DEBUG ?= 0
SGX_COMMON_FLAGS := -m64

ifeq ($(SGX_DEBUG), 1)
ifeq ($(SGX_PRERELEASE), 1)
$(error Cannot set SGX_DEBUG and SGX_PRERELEASE at the same time!!)
endif
endif

ifeq ($(SGX_DEBUG), 1)
        SGX_COMMON_FLAGS += -O0 -g
else
        SGX_COMMON_FLAGS += -O2
endif

OpenSSL_Crypto_Library_Name := -lssl -lcrypto
SgxSSL_Link_Libraries := -L$(OPENSSL_LIBRARY_PATH) -Wl,--whole-archive -Wl,--no-whole-archive $(OpenSSL_Crypto_Library_Name)


SGX_COMMON_FLAGS += -Wall -Wextra -Winit-self -Wpointer-arith -Wreturn-type \
                    -Waddress -Wsequence-point -Wformat-security \
                    -Wmissing-include-dirs -Wfloat-equal -Wundef -Wshadow \
                    -Wcast-align -Wcast-qual -Wconversion -Wredundant-decls
SGX_COMMON_CFLAGS := $(SGX_COMMON_FLAGS) -Wjump-misses-init -Wstrict-prototypes -Wunsuffixed-float-constants
SGX_COMMON_CXXFLAGS := $(SGX_COMMON_FLAGS) -Wnon-virtual-dtor -std=c++11

######## App Settings ########

App_C_Files := App_no-sgx/App.c $(wildcard App_no-sgx/EnclavedErlangCFunc/*.c)
App_Include_Paths := -IApp -I$(OPENSSL_INCLUDES)

App_C_Flags := -fPIC -Wno-attributes $(App_Include_Paths)

# Three configuration modes - Debug, prerelease, release
#   Debug - Macro DEBUG enabled.
#   Prerelease - Macro NDEBUG and EDEBUG enabled.
#   Release - Macro NDEBUG enabled.
ifeq ($(SGX_DEBUG), 1)
        App_C_Flags += -DDEBUG -UNDEBUG -UEDEBUG
else ifeq ($(SGX_PRERELEASE), 1)
        App_C_Flags += -DNDEBUG -DEDEBUG -UDEBUG
else
        App_C_Flags += -DNDEBUG -UEDEBUG -UDEBUG
endif

App_Cpp_Flags := $(App_C_Flags)

App_C_Objects := $(App_C_Files:.c=.o)

App_Name := app.so

######## Enclave Settings ########

Enclave_Cpp_Files := Enclave/Enclave.cpp $(wildcard Enclave/tests/*.cpp)
Enclave_C_Files := $(wildcard Enclave/*.c) $(wildcard Enclave/tests/*.c)

Enclave_Cpp_Objects := $(sort $(Enclave_Cpp_Files:.cpp=.o))
Enclave_C_Objects := $(Enclave_C_Files:.c=.o)

Enclave_Include_Paths := -IEnclave -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(LIBCXX_INC) -I$(OPENSSL_INCLUDES)

Enclave_C_Flags := $(Enclave_Include_Paths)  -nostdinc -fvisibility=hidden -fpie -ffunction-sections -fdata-sections $(MITIGATION_CFLAGS)
CC_BELOW_4_9 := $(shell expr "`$(CC) -dumpversion`" \< "4.9")
ifeq ($(CC_BELOW_4_9), 1)
	Enclave_C_Flags += -fstack-protector
else
	Enclave_C_Flags += -fstack-protector-strong
endif

Enclave_Cpp_Flags := $(Enclave_C_Flags) -nostdinc++ -std=c++11
Enclave_C_Flags += -std=c11

Enclave_Security_Link_Flags := -Wl,-z,relro,-z,now,-z,noexecstack

SGX_SHARED_LIB_FLAG := -Wl,-rpath,${SGX_LIBRARY_PATH}
App_Link_Flags := $(SGX_COMMON_CFLAGS) $(Enclave_Security_Link_Flags) $(SGX_SHARED_LIB_FLAG) -lpthread $(SgxSSL_Link_Libraries)

ifeq ($(SGX_MODE), HW)
ifeq ($(SGX_DEBUG), 1)
	Build_Mode = HW_DEBUG
else ifeq ($(SGX_PRERELEASE), 1)
	Build_Mode = HW_PRERELEASE
else
	Build_Mode = HW_RELEASE
endif
else
ifeq ($(SGX_DEBUG), 1)
	Build_Mode = SIM_DEBUG
else ifeq ($(SGX_PRERELEASE), 1)
	Build_Mode = SIM_PRERELEASE
else
	Build_Mode = SIM_RELEASE
endif
endif


.PHONY: all target run
all: target

ifeq ($(Build_Mode), HW_RELEASE)
target:  $(App_Name) $(Enclave_Name)
else
target: $(App_Name) $(Signed_Enclave_Name)
endif

run: all
ifneq ($(Build_Mode), HW_RELEASE)
	@$(CURDIR)/$(App_Name)
	@echo "RUN  =>  $(App_Name) [$(SGX_MODE)|$(SGX_ARCH), OK]"
endif

.config_$(Build_Mode)_$(SGX_ARCH):
	@rm -f .config_* $(App_Name) $(Enclave_Name) $(Signed_Enclave_Name) $(App_Cpp_Objects) $(App_C_Objects) 
	@touch .config_$(Build_Mode)_$(SGX_ARCH)



######## Default C/C++ to object files #########

.SUFFIXES: .c .o .erl .beam

.c.o:
	@echo "C Compling: $@ from $<"
	$(CC) $(SGX_COMMON_CFLAGS) $(App_C_Flags) -c $< -o $@

.cpp.o:
	@echo "CPP Compling: $@ from $<"
	$(CXX) $(SGX_COMMON_CPPFLAGS) $(App_CPP_Flags) -c $< -o $@

######## App Objects ########

App_C_Erl_Flags := -I/usr/local/lib/erlang/usr/include/

App_Link_Erl_Flags := -L/usr/local/lib/erlang/usr/lib/ -lei -lpthread

App_no-sgx/App.o: App_no-sgx/App.c
	$(CC) $(SGX_COMMON_CFLAGS) $(App_C_Flags) $(App_C_Erl_Flags) -c $< -o $@
	@echo "CC   <=  $<"

$(App_Name): $(App_C_Objects) $(App_Cpp_Objects)
	$(CXX) $^ -shared -o $@ $(App_Link_Flags) $(App_Link_Erl_Flags)
	@echo "LINK =>  $@"

.PHONY: clean

clean:
	@rm -f .config_* $(App_Name) $(Enclave_Name) $(App_C_Objects) $(App_Cpp_Objects) *.beam
