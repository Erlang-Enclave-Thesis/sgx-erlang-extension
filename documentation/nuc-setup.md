# Hardware

Intel NUC NUC7PJYH (supports SGX2 and DCAP)

Kingston 8GB DDR4 RAM (KVR24S17S8/8)

Samsung 860 EVO (MZ-76E500B)

## NUC BIOS

Disabling Secure Boot can be helpful if you cannot get custom drivers (such as the SGX driver) signed.

Other than Secure Boot, no changes were made in the BIOS.

## Operating System

Ubuntu 18.04 LTS 64 bit was picked as recommended by Intel.

### Software

A number of different software versions were used.
There were no major differences between versions, except that erl\_interface functionality was deprecated in Erlang/OTP 23 which therefore did not support our C Node prototype.

#### Final Setup

DCAP 1.5 driver. 

Ubuntu 18.04, SGX SDK 2.9.1, Erlang/OTP 23, GCC/G++ 10.1.0-2

#### Previous Setups Tested

DCAP 1.4 and 1.6

Erlang/OTP 20 and 22

GCC 8.3
