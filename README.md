
# Erlang-SGX

This work was created in a Master's thesis for Chalmers University together with Ericsson AB by [Eliot Roxbergh](https://github.com/Eliot-Roxbergh) and [Emil Hemdal](https://github.com/emilhem).
Here we explore several methods of protecting Erlang workloads with Intel SGX secure enclaves.
The practical parts consist of prototypes for calling C/C++ code in SGX from Erlang, which includes OpenSSL functionality, as well as efforts to execute the whole BEAM runtime itself inside of SGX.
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

**Note:** Parts of erl\_interface were deprecated with the release of Erlang/OTP 23 which therefore does not support the C Node prototype, everything else we provide works fine with this release. Therefore, to use all provided functionality an earlier version can be used and the project has also been tested with Erlang/OTP 20 and 22.


## Usefulness of Erlang SGX

The idea with this project was to provide security benefits to Erlang, while supporting legacy codebases.
We propose two different methods by which we can achieve this;

1. Replace NIFs used in BEAM with SGX versions. For instance, the NIFs used in the _crypto_ library under the hood could be replaced to provide hardened crypto functions seamlessly for current Erlang applications. This could also enable confidentiality of the C binaries in SGX. Here the overhead is small, _without_ optimizations our NIFs had an overhead of between 30 and 250 microseconds depending on context switches. This should be very doable as we presented a minimum viable product here.

2. Run Erlang code itself in SGX. This could be achieved in a number of ways (future work...), and would protect legacy Erlang applications. Although, security and performance need to be evaluated.


## Future Work Ideas

#### BEAM SGX

1. Compile BEAM to another target, such as WebAssembly and run in SGX.. can we use [Lumen](https://github.com/lumen/lumen)?

2. If BEAM is to be used, a first step could be to examinate each syscall performed by BEAM (67 unique syscalls for '_hello world_'). Thereby, we could determine which calls could be ignored, resolved internally in SGX, or forwarded out to OS if absolutely necessary. Remember, a vast majority of calls performed are made from the BEAM scheduler and could be ignored at a performance cost. It is also possible to tweak build options to lessen these calls, but BEAM was not really built with this in mind so this needs to be investigated further in that case. Thereafter, the dream solution involves a custom layer specifically optimized for BEAM according to these syscalls. Then, to possibly, statically preload modules to lessen initial IO by BEAM and memory-mapped IO for improved performance (quickly mentioned in the thesis)

   Build options which could reduce number of syscalls: by increasing `ETHR_YIELD_AFTER_BUSY_LOOPS` or by setting `+sbwt none +sbwtdcpu none +sbwtdio none` (explained [here](https://stressgrid.com/blog/beam_cpu_usage/)). More information can be read out with [microstate accounting](https://erlang.org/doc/man/msacc.html) ([API](https://erlang.org/doc/man/erlang.html#statistics_microstate_accounting)).

3. MPMC pipe use in BEAM is a big hurdle for Graphene. We are not quite sure if this could effect other frameworks as well, are these pipes only used intra-process? In SGX we ideally want end-to-end communication and encryption just to be sure nothing leaks during external communication. Therefore, it could be an important feature to fix in BEAM.


