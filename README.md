## NOTICE:
**We are still cleaning up this repostory for final deadline late June.**

**TODO:** Add final thesis once complete.

**TODO:** We could add summary of theoretical parts: "Creating a Custom Layer" and "Replacing BEAM". Additionally, conclusions or abstract could be mentioned. But all this is to be found in the thesis.

# SGX-Erlang-extension

This work was created in a Master's thesis for Chalmers University together with Ericsson AB by [Eliot Roxbergh](https://github.com/Eliot-Roxbergh) and [Emil Hemdal](https://github.com/emilhem).
It consists of prototypes for calling secure C/C++ code from Erlang, including OpenSSL, as well as efforts to execute the whole BEAM runtime itself inside of SGX.
Details of the initial setup and configuration can be found in (`documentation/`).

For the C/C++ parts we provide working prototypes that enable Erlang to call basic SGX-secured functions via NIF and C Nodes (`erlang-nif/` an `erlang-c-node/`).
The NIF example is also extended to include Intel SGX SSL (OpenSSL 1.1.1d) and thereby enable cryptographic functionality inside of the secure enclave (`erlang-nif_SSL/`).
These three examples are also ported to run in the regular untrusted OS as to show a basecase for performance evaluation, and they can be found in `without-sgx/` as well as in `erlang-nif_SSL/` which hosts both untrusted and SGX versions.

For executing BEAM inside of SGX, the two frameworks Graphene and SCONE were evaluted. Graphene can only support Erlang _if_ the issue of MPMC pipes is resolved. SCONE on the other hand, is working on official support for Erlang which at time of this project is very limited.
These efforts are provided in `graphene/` and `scone/`, however SCONE is dependent on a proprietary docker image (from the Scontain team) not provided here.

Additionally, two basic examples were constructed to measure and compare the syscalls performed by Erlang BEAM and Java JVM, located in `without-sgx/hello-worlds/`.
The evaluation results can be found in `evaluation/` which includes basic run time measurements of C/C++ prototypes as well as the results of syscall tests with strace.

For the C/C++ parts it is very possible to enable different methods of attestation as we only utilize the native Intel SGX SDK functionality.
Remote attestation with DCAP was attempted but abandoned due to other more priorities closer to our research question, these implementations are found in `attestation_abandoned/`.
Moreover, we note that it could be possible to call other ported SGX runtimes from Erlang such as Rust-SGX as long as these can communicate with the Erlang NIF or the C Node (via C code).

Finally, we want to make it clear that our aim was to investigate different methods of protecting Erlang with Intel SGX and that therefore any code produced is very rudimentary and can certainly be improved. Regardless, we hope that these examples and conclusions can assist the community.


## Setup

Intel NUC7PJYH with SGX2 hardware support using the DCAP 1.5 driver.

Ubuntu 18.04, SGX SDK 2.9.1, Erlang/OTP 23, GCC/G++ 10.1.0-2

**Note:** Parts of erl\_interface were deprecated with the release of Erlang/OTP 23 which therefore does not support the C Node prototype, everything else we provide works fine with this release. Therefore, to use all provided functionality an earlier version can be used and the project has also been tested with Erlang/OTP 20.
