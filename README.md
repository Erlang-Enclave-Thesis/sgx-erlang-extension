## NOTICE:
**We are still cleaning up this repostory for final deadline late June.**


TODO: add further instructions/documentation in separate 'documentation/' and final thesis once complete.
Clean up Makefiles and add warning about specific hardcoded settings.

# SGX-Erlang-extension

This work was created in a Master's thesis for Chalmers University together with Ericsson AB by Eliot Roxbergh and Emil Hemdal.
It consists of prototypes for calling secure C/C++ code from Erlang, including OpenSSL, as well as efforts to execute the whole BEAM runtime itself inside of SGX. The two frameworks Graphene and SCONE were evaluted. Graphene can _only_ support Erlang if the issue of MPMC pipes is resolved and SCONE is working on official support which at time of this project is very limited. These are provided in _'graphene/'_ and _'scone/'_, however SCONE is dependent on a proprietary docker image which we received from the SCONE team.
For the C/C++ code parts we provide working prototypes that enable Erlang to call basic SGX-secured functions via NIF and C Nodes (_'erlang-nif' 'erlang-c-node/'_).
The NIF example is also extended to include Intel SGX SSL (OpenSSL 1.1.1d) and thereby enable cryptographic functionality inside of the secure enclave (_'erlang-nif_SSL'_).
These three examples are also ported to run in the untrusted area as to show a basecase for performance evaluation, and they can be found in _'without-sgx/'_ as well as in _'erlang-nif\_SSL'_ which hosts both untrusted and SGX versions.

Additionally, a few basic examples were constructed to measure and compare the syscalls performed by Erlang BEAM and Java JVM, located in _'without-sgx/hello-worlds/'_.

The evaluation results can be found in _'evaluation/'_.

For the C/C++ parts it is very possible to enable different methods of attestation as it only requires the native Intel SGX SDK functionality.
Remote attestation with DCAP was attempted but abandoned due to other more priorities closer to our research question, these are found in _'attestation\_abandoned/'_. Since a regular call is made to SGX, any supported functionality therein should be ready to go including other supported runtimes such as Rust-SGX.

Finally, we want to make it clear that our aim was to investigate methods of protecting Erlang with Intel SGX and that therefore any code produced is very rudimentary and can certainly be improved. Regardless, we hope that these examples and conclusions can assist the community.


## Setup

Intel NUC7PJYH with SGX2 hardware support using the DCAP 1.5 driver.

Ubuntu 18.04, SGX SDK 2.9.1, Erlang/OTP 23, GCC/G++ 10.1.0-2

**Note:** Parts of erl\_interface were deprecated with the release of Erlang/OTP 23 which therefore does not support the C Node prototype, everything else works fine with this release however. Therefore, to use all provided functionality an earlier version can be used and it has been tested with Erlang/OTP 20.


