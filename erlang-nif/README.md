A prototype which calls C/C++ code inside of SGX via a Erlang Natively Implemented Function (NIF).
A very basic prototype which supports incrementing and returning a secret number in the enclave.
This version utilizes SGX. Similar functionality without SGX is located separately in `without-sgx/`.
A quick and dirty prototype, we make no promises of its function or reliablilty.


## Setup

Erlang/OTP 20 [erts-9.2] [source] [64-bit] [smp:4:4] [ds:4:4:10] [async-threads:10] [kernel-poll:false]

Eshell V9.2

GCC / G++  7.5.0-3, Ubuntu 18.04, NUC7PJYH, DCAP driver


# NIF

## Warning
The Erlang NIF upgrade functionality have not been developed fully.
This means that even though a reload of the Erlang code is performed
it won't keep track of the separate enclaves (which have the same id
which could be a problem of its own...).

## How to Build/Execute the NIF Code

1. Install Intel(R) SGX SDK for Linux* OS
2. Make sure your environment is set:
    `source ${sgx-sdk-install-path}/environment`
3. Build the project with the prepared Makefile:
  a. Hardware Mode, Debug build:
    1) Enclave with no mitigation:
      $ make
    2) Enclave with mitigations for indirects and returns only:
      $ make MITIGATION-CVE-2020-0551=CF
    3) Enclave with full mitigation:
      $ make MITIGATION-CVE-2020-0551=LOAD
  b. Hardware Mode, Pre-release build:
    1) Enclave with no mitigation:
      $ make SGX_PRERELEASE=1 SGX_DEBUG=0
    2) Enclave with mitigations for indirects and returns only:
      $ make SGX_PRERELEASE=1 SGX_DEBUG=0 MITIGATION-CVE-2020-0551=CF
    3) Enclave with full mitigation:
      $ make SGX_PRERELEASE=1 SGX_DEBUG=0 MITIGATION-CVE-2020-0551=LOAD
  c. Hardware Mode, Release build:
    1) Enclave with no mitigation:
      $ make SGX_DEBUG=0
    2) Enclave with mitigations for indirects and returns only:
      $ make SGX_DEBUG=0 MITIGATION-CVE-2020-0551=CF
    3) Enclave with full mitigation:
      $ make SGX_DEBUG=0 MITIGATION-CVE-2020-0551=LOAD
  d. Simulation Mode, Debug build:
    $ make SGX_MODE=SIM
  e. Simulation Mode, Pre-release build:
    $ make SGX_MODE=SIM SGX_PRERELEASE=1 SGX_DEBUG=0
  f. Simulation Mode, Release build:
    $ make SGX_MODE=SIM SGX_DEBUG=0
4. Execute the binary indirectly via Erlang:
  `erl`
  `c(enclave_communicator).`
5. Remember to "make clean" before switching build mode

## How to connect Erlang Proceeses to the NIF
The file `enclave_communicator.erl` can be used as a start for a program
using the NIF.

```
make
erl
c(enclave_communicator).
timer:tc(enclave_communicator, increment,"").
timer:tc(enclave_communicator, rtrn,"").
```

