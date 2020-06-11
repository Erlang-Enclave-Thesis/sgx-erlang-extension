In this directory we provide some basic documentation for installing and using SGX.

These documents were mostly written during the setup phase, early in the project, and might therefore be outdated.
Use with care. No warranty provided.

# Table of Contents

## Intro

[System setup, software used, NUC config](nuc-setup.md) (we assume DCAP support)

[Some quick notes on acronyms and such](terminology.md)

## Installation Steps

 
1. [Install SGX components](sgx-setup.md)
2. [Fix SGX simulation support and create RSA key-pair for signing enclave](sgx-development-setup.md)
3. [Install/build Erlang](erlangOTP-setup.md)

## Using Frameworks

### SCONE
[Basic installation of SCONE](scone-setup.md)

Erlang specific parts are located in [../scone](../scone)

### Graphene

[Basic installation of Graphene](graphene-setup.md)

Erlang specific parts are located in [../graphene](../graphene)




## Abandoned

[Some steps to use MiniKube with SGX (not used)](abandoned_MiniKube.md)
