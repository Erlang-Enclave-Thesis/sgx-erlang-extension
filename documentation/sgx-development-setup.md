On this page we explain how to set up a development environment for Intel SGX.

This consists of two parts.
First to create a local SGX simulation environment, this also supports debugging with Eclipse.
Second is to generate signing keys necessary for running `Enclave.so` in release mode.


# Local Simulation Environment

```
wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add -
echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
sudo apt update && sudo apt upgrade -y
sudo apt install libsgx-urts libsgx-dcap-ql libsgx-dcap-ql-dev libsgx-dcap-default-qpl libsgx-dcap-default-qpl-dev -y
```

Install Intel SDK <https://download.01.org/intel-sgx/latest/dcap-latest/linux/distro/ubuntuServer18.04/>

Then source ~/Downloads/sgx (e.g. add to .bashrc)


## Eclipse Plugin

Install Eclipse ("For C/C++ developers") -> Download: <https://www.eclipse.org/downloads/>

Add SGX plugin to Eclipse ("Add new software.." in eclipse) -> Download: <https://01.org/intel-software-guard-extensions/downloads>

Now we're ready to run enclaved code in simulation mode which we can debug from Eclipse.

### Verifying Functionality with C Node

Start eclipse and open the project (e.g. _sgx-erlang-extension/erlang-c-node_), build it in Eclipse, make sure _enclave\_communicator.erl_ contains your hostname (@...).

Go to the path, set secret cookie and start Erlang. You may now use the program!

```
erl -sname e1 -setcookie "Very Secret Cookie"
c(enclave_communicator).
enclave_communicator:increment().
enclave_communicator:return().
#if this hangs make sure hostname was entered correctly and that EPDM is running: ps aux |grep epmd
```

Verified working 2020-04-24, setup:
```
Linux 5.3.0-46-generic #38~18.04.1-Ubuntu SMP Tue Mar 31 04:17:56 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
Erlang/OTP 22 [erts-10.6] [source] [64-bit] [smp:4:4] [ds:4:4:10] Eshell V10.6
```

# Create Key Pair for Signing

To develop applications for Intel SGX you will eventually need signing keys. Note that the keys we generate here are not meant for actual deployment since these keys should be kept much more secure than your development machine.

Reference regarding keys and signing: [Intel SGX Docs](https://download.01.org/intel-sgx/latest/linux-latest/docs/) (specifically Developer Reference ("Enclave Signing Tool"))

_"Key file should follow the PEM format which contains an unencrypted RSA 3072-bit key. The public exponent must be 3."_ - Developer Reference

## Steps

1. Enter a directory where you want your keys to be located.
2. Run  
   `openssl genrsa -out private_key.pem -3 3072`  
   to generate a private RSA key.

3. Run  
   `openssl rsa -in private_key.pem -pubout -out public_key.pem`  
   to generate the public part of the key.

### Signing the Enclave

The signing enclaves should be performed by the _sgx\_sign_ utility.

Sign enclave.so file you wish to run in release mode, e.g.:
```
make SGX_DEBUG=0
~/sgxsdk/bin/x64/sgx_sign sign -key ~/private_key.pem -enclave enclave.so -out enclave.signed.so -config Enclave/Enclave.config.xml
```

# PCCS Admin Tools

For handling certificates for DCAP, take a look at PCCS admin tools.
This is located in the DCAP repository `tools/PccsAdminTool`, with the script `pccsadmin.py`.
