## NOTICE:
**We are still cleaning up this repostory for final deadline late June.**

# SGX-Erlang-extension

This work was created in a Master's thesis for Chalmers University together with Ericsson AB by Eliot Roxbergh and Emil Hemdal.
It consists of prototypes for calling secure C code from Erlang, including OpenSSL, as well as efforts to execute the whole BEAM runtime itself inside of SGX.

## Setup

Intel NUC7PJYH with SGX2 hardware support using the DCAP 1.5 driver.

Ubuntu 18.04, SGX SDK 2.9.1, Erlang/OTP 23, GCC/G++ 10.1.0-2

**Note:** Parts of erl\_interface were deprecated with the release of Erlang/OTP 23 which therefore does not support the C Node prototype, everything else works fine with this release however.

