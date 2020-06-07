## NOTICE:
**We are still cleaning up this repostory for final deadline late June.**

TODO: add further instructions/documentation in separate `documentation/` and final thesis once complete.
The main parts are described except for theoretical parts: "Creating a Custom Layer" and "Replacing BEAM" ...

# SGX-Erlang-extension

This work was created in a Master's thesis for Chalmers University together with Ericsson AB by Eliot Roxbergh and Emil Hemdal.
It consists of prototypes for calling secure C/C++ code from Erlang, including OpenSSL, as well as efforts to execute the whole BEAM runtime itself inside of SGX.

For the C/C++ parts we provide working prototypes that enable Erlang to call basic SGX-secured functions via NIF and C Nodes (`erlang-nif/` an `erlang-c-node/`).
The NIF example is also extended to include Intel SGX SSL (OpenSSL 1.1.1d) and thereby enable cryptographic functionality inside of the secure enclave (`erlang-nif_SSL/`).
These three examples are also ported to run in the regular untrusted OS as to show a basecase for performance evaluation, and they can be found in `without-sgx/` as well as in `erlang-nif_SSL/` which hosts both untrusted and SGX versions.

For executing BEAM inside of SGX, the two frameworks Graphene and SCONE were evaluted. Graphene can _only_ support Erlang if the issue of MPMC pipes is resolved and SCONE is working on official support which at time of this project is very limited. These are provided in `graphene/` and `scone/`, however SCONE is dependent on a proprietary docker image (from the Scontain team) not provided here.

Additionally, two basic examples were constructed to measure and compare the syscalls performed by Erlang BEAM and Java JVM, located in `without-sgx/hello-worlds/`.
The evaluation results can be found in `evaluation/` which includes basic run time measurements of C/C++ prototypes as well as the results of syscall tests with strace.

For the C/C++ parts it is very possible to enable different methods of attestation as we only utilize the native Intel SGX SDK functionality.
Remote attestation with DCAP was attempted but abandoned due to other more priorities closer to our research question, these implementations are found in `attestation_abandoned/`.
(Also, since a regular call is made to SGX any supported functionality therein should be ready to go including other supported runtimes such as Rust-SGX as long as these can communicate with the Erlang NIF or C Node.)

Finally, we want to make it clear that our aim was to investigate different methods of protecting Erlang with Intel SGX and that therefore any code produced is very rudimentary and can certainly be improved. Regardless, we hope that these examples and conclusions can assist the community.


## Setup

Intel NUC7PJYH with SGX2 hardware support using the DCAP 1.5 driver.

Ubuntu 18.04, SGX SDK 2.9.1, Erlang/OTP 23, GCC/G++ 10.1.0-2

**Note:** Parts of erl\_interface were deprecated with the release of Erlang/OTP 23 which therefore does not support the C Node prototype, everything else we provide works fine with this release. Therefore, to use all provided functionality an earlier version can be used and the project has also been tested with Erlang/OTP 20.
