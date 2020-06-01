Showcase of Erlang NIF SGX and comparison with non-SGX version.
A quick and dirty prototype, we make no promises of its function or reliablilty.

Tested on:

Erlang/OTP 23 [erts-11.0] [source] [64-bit] [smp:4:4] [ds:4:4:10] [async-threads:1]
Eshell V11.0

GCC / G++ 10.1.0-2, Ubuntu 18.04,
NUC7PJYH, DCAP 1.5 driver, SGX SDK 2.9.1


# NIF with SSL

Erlang NIFs that support functions as well as OpenSSL API.
We provide increment and return for basic testing, as well RSA functions to evaluate OpenSSL.
The SGX version uses Intel SGX SSL which builds OpenSSL 1.1.1d for SGX,
the other untrusted version uses plain OpenSSL 1.1.1d which should behave largely the same.
Otherwise, the C/C++ code (our NIF) is as similar as possible between these two versions
with the same functionality and OpenSSL API calls.
For the unprotected version the enclaved code (Enclave.cpp) is compiled into the unsafe App.so file instead of a separate Enclave.so file.

## SGX Warning
The Erlang NIF upgrade functionality have not been developed fully.
This means that even though a reload of the Erlang code is performed
it won't keep track of the separate enclaves (which have the same id
which could be a problem of its own...).

## Requirements and build options for SGX version

1. Install Intel(R) SGX SDK for Linux* OS
2. Make sure your environment is set:
    `source ${sgx-sdk-install-path}/environment`
3. Build OpenSSL libraries for SGX (https://github.com/intel/intel-sgx-ssl)
4. Build the project with the prepared Makefile:
  a. Hardware Mode, Debug build:
    1) Enclave with no mitigation:
      $ make sgx
    2) Enclave with mitigations for indirects and returns only:
      $ make sgx MITIGATION-CVE-2020-0551=CF
    3) Enclave with full mitigation:
      $ make sgx MITIGATION-CVE-2020-0551=LOAD
  b. Hardware Mode, Pre-release build:
    1) Enclave with no mitigation:
      $ make sgx SGX_PRERELEASE=1 SGX_DEBUG=0
    2) Enclave with mitigations for indirects and returns only:
      $ make sgx SGX_PRERELEASE=1 SGX_DEBUG=0 MITIGATION-CVE-2020-0551=CF
    3) Enclave with full mitigation:
      $ make sgx SGX_PRERELEASE=1 SGX_DEBUG=0 MITIGATION-CVE-2020-0551=LOAD
  c. Hardware Mode, Release build:
    1) Enclave with no mitigation:
      $ make sgx SGX_DEBUG=0
    2) Enclave with mitigations for indirects and returns only:
      $ make sgx SGX_DEBUG=0 MITIGATION-CVE-2020-0551=CF
    3) Enclave with full mitigation:
      $ make sgx SGX_DEBUG=0 MITIGATION-CVE-2020-0551=LOAD
  d. Simulation Mode, Debug build:
    $ make sgx SGX_MODE=SIM
  e. Simulation Mode, Pre-release build:
    $ make sgx SGX_MODE=SIM SGX_PRERELEASE=1 SGX_DEBUG=0
  f. Simulation Mode, Release build:
    $ make sgx SGX_MODE=SIM SGX_DEBUG=0
5. Remember to "make clean" before switching build mode

## Requirements and build options for non-SGX version

1. Compile OpenSSL 1.1.1d into /home/ericsson/openssl
2. Build the project with the prepare Makefile:
  a. Debug mode:
	$ make no-sgx
  c. Pre-release mode:
	$ make no-sgx SGX_PRERELEASE=1 SGX_DEBUG=0
  b. Release mode:
	$ make no-sgx SGX_DEBUG=0
3. Remember to "make clean" before switching build mode


## Error: Please use the correct uRTS library from PSW package

If you get this error when building in HW mode it is because LD_LIBRARY_PATH=/opt/intel/sgxsdk/lib64/.

Fix this by setting the path where PSW was installed, this worked for us:
```
LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu erl
```

# Evaluation

## Time measurement

Use functions eval_rsa() and eval_increment() to run a subset of our NIF functions 30 times. They return the average execution time in microseconds.

## Syscalls

Simple strace measurement of syscalls can be made, this will generate a lot of files since Erlang starts many children. The command fuser can be used to find out what PID is using a certain resource like app.so

```
erlc enclave_communicator.erl
strace -ff -tt -o strace erl -noshell -s enclave_communicator increment -s init stop
```


# How to run the Erlang NIFs
The file `enclave_communicator.erl` can be used as a start for a program
using the NIF.
A number of functions are provided in the erl file, eval\_rsa() and eval\_increment() is used to time measurements.

## SGX Hardware

```
make clean && make sgx SGX_DEBUG=0 SGX_MODE=HW
/home/ericsson/sgxsdk/bin/x64/sgx_sign sign -key ~/private_key.pem -enclave enclave.so -out enclave.signed.so -config Enclave/Enclave.config.xml
LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu erl
c(enclave_communicator).
enclave_communicator:eval_rsa().
enclave_communicator:eval_increment().
```

## SGX Simulated

```
make clean && make sgx SGX_DEBUG=0 SGX_MODE=SIM
erl
c(enclave_communicator).
enclave_communicator:eval_rsa().
enclave_communicator:eval_increment().
`


## No SGX

```
make clean && make no-sgx SGX_DEBUG=0
erl
c(enclave_communicator).
enclave_communicator:eval_rsa().
enclave_communicator:eval_increment().
`


