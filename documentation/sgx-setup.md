On this page we explain what has to be done in order to enable SGX support on a supported Intel NUC.

#Initial requirements
Add the repository from Intel by running

```
wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add -
echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
sudo apt update
```

## Installing DCAP SGX driver

The DCAP driver is needed if you want to use third-party attestation.

Follow instructions (STEP 2) here: <https://github.com/openenclave/openenclave/blob/master/docs/GettingStartedDocs/install_oe_sdk-Ubuntu_18.04.md#2-install-the-intel-sgx-dcap-driver>

Intel has a guide as well which might be useful to know about. Intel's guide: <https://download.01.org/intel-sgx/latest/dcap-latest/linux/docs/Intel_SGX_DCAP_Linux_SW_Installation_Guide.pdf>

### Post-installation
Not sure if needed, but Intel mentions a _sgx\_prv_ group that the user of enclave should use if they want to use their own provision enclave. Added user to that group by doing the following

`usermod -a -G sgx_prv $USER`

## Install DCAP and PSW (platform software) runtime packages

These packages will be used when developing/running software for/on SGX DCAP.

### Prerequisites

Install the DCAP driver first.

### Installation

`sudo apt update && sudo apt install libsgx-urts libsgx-dcap-ql libsgx-dcap-ql-dev libsgx-dcap-default-qpl libsgx-dcap-default-qpl-dev -y`

## Installing SGX PCCS (Provisioning Certificate Caching Service)

### Prerequisites

Ensure that node.js is installed. You can run the following to install node.js:

```
curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
sudo apt install -y nodejs
```

### Installation

The installation will ask you for http and https proxy settings. It will also ask you about your Intel PCS (Provisioning Certificate Service) API key. Get one from <https://api.portal.trustedservices.intel.com/provisioning-certification>. It will also ask you about certificates, for development purposes this can be self-generated.

`sudo apt install sgx-dcap-pccs`

### Post-install steps
Since the installation creates a self signed certificate for TLS you have the do the following to make the service work as intended. In a production deployment a real certificate should be used instead.

Modify `/etc/sgx_default_qcnl.conf` to contain

`USE_SECURE_CERT=FALSE`

## Installing SGX EPID Quote Service

`sudo apt install libsgx-epid`

## Installing SGX Unified Quote Service

`sudo apt install libsgx-quote-ex`

## Intel SGX SDK
If you need Intel SGX SDK for your project, this section will explain how to acquire it.

### Installation
Go to <https://download.01.org/intel-sgx/latest/dcap-latest/linux/distro/ubuntuServer18.04/> and copy the link to the SDK.

Run `wget` or `curl` to download the file to the NUC. Make it executable with `chmod +x` and run it as a regular user to install it.

### Post-install steps

In `.bashrc` add the below line, to persistently get access to the tools provided by sgxsdk.
`source [installation-path]/sgxsdk/environment`

## SGX-OpenSSL
If you wish to use the enclaved OpenSSL version you may enable it by following the instructions at <https://github.com/intel/intel-sgx-ssl>.

In our case we performed the following steps:

```
#prerequisite Intel SGX SDK and download source to OpenSSL_1.1.1d
git clone https://github.com/intel/intel-sgx-ssl
#add OpenSSL_1.1.1d.tar.gz in repo in folder openssl_source/, note it must have have this name and extract to this name OpenSSL_1.1.1d/
make clean
make test
make sgxssl_no_mitigation DEBUG=1 SGX_MODE=HW VERBOSE=1 #warning: this disables mitigations, our compiler did not support a mitigation flag
sudo make install #copies stuff to  /opt/intel/sgxssl/
```

## Open Enclave
If you need Open Enclave this will explain how to get it to work.

### Installation
**Step 2** should already be completed from "Installing DCAP SGX driver” step.

Follow instructions here:
<https://github.com/openenclave/openenclave/blob/master/docs/GettingStartedDocs/install_oe_sdk-Ubuntu_18.04.md

### Post-install steps
In .bashrc add the below line, to persistently get access to the tools provided by open-enclave.
`source /opt/openenclave/share/openenclave/openenclaverc`

## Testing Intel SGX

Confirming that Intel SGX is installed and working is a crucial step before beginning to develop software on the platform.

### Open Enclave

Prerequisites: Open Enclave installed

<https://github.com/openenclave/openenclave/blob/master/docs/GettingStartedDocs/Linux_using_oe_sdk.md#samples>

Comment: We had some issues with the samples remote_attestation and attested_tls

### Intel SGX SDK

Prerequisites: Intel SGX SDK installed

Enter the intel sgx sdk (sgxsdk) directory. Enter the SampleCode directory. Run the samples.

Comment: We had some issues with the samples for Switchless

