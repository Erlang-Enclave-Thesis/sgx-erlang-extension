Some of these have more in depth descriptions in the Intel SGX Developer Reference, see link below:

<https://download.01.org/intel-sgx/latest/linux-latest/docs/>

<https://download.01.org/intel-sgx/latest/linux-latest/docs/Intel_SGX_Developer_Reference_Linux_2.9.1_Open_Source.pdf>

# Acronyms

## Intel

SGX - Software Guard Extensions

PSW - Platform Software

FLC - Flexible Launch Control

### TCB

TCB - Trusted Code Base (user application in enclave)

TCB - Trusted Computing Base (enclave implementation itself, including microcode and Intel features).
Quote from [Intel forums](https://software.intel.com/en-us/forums/intel-software-guard-extensions-intel-sgx/topic/852325):
_"includes platform-specific information related to the CPU microcode. This TCB is used by DCAP to ensure that the platform that the enclave is running on is indeed a trusted platform. The TCB is an important part of the attestation process. I recommend you read the [Intel SGX ECDSA QuoteLibReference_DCAP_API.pdf](https://download.01.org/intel-sgx/sgx-dcap/1.6/linux/docs/Intel_SGX_ECDSA_QuoteLibReference_DCAP_API.pdf) which contains more info on how the TCB is used."_

