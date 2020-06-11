Graphene is an application wrapper to make software run in enclaves, which seem to get a lot of support from Intel.

You should already have a system with the SGX driver installed before continuing with this guide.

# Prerequisites

A working SGX system with the SGX driver installed.

Signing keys for SGX signing: [Setup Keys](sgx-development-setup.md)

Graphene wants a environment variable set for the location of the private key. Set _SGX\_SIGNER\_KEY_ to this location in a shellfile (eg. ~/.bashrc) like this:

`export SGX_SIGNER_KEY=/home/user/keys/private-key.pem`

# SGX-DCAP source repo

Clone the git repo <https://github.com/intel/SGXDataCenterAttestationPrimitives> somewhere on the machine.

Find the latest stable release by its tag in the GitHub repo, or run `git tag --list` in Git directly.

`git checkout [latest release tag]`

In this example we used _DCAP\_1.5_

## Graphene Driver

In future versions of either SGX or Graphene this driver might no longer be needed.

Use the official guide here (step 3 regarding Graphene SGX driver): <https://graphene.readthedocs.io/en/latest/building.html#building-with-intel-sgx-support>

Remember to give it the path to the SGXDataCenterAttestationPrimitives that you cloned earlier. Provide it with the path to the _driver/linux_ directory in the repo.

# Installation

Try to use the official guide here: <https://graphene.readthedocs.io/en/latest/building.html>

Although we have condensed the steps below:

Install the build tools by executing the following
```
sudo apt install build-essential autoconf gawk bison
sudo apt install libprotobuf-c-dev protobuf-c-compiler python3-protobuf
```

Then run
`git clone https://github.com/oscarlab/graphene.git`
in a directory where you want the source files for graphene to be placed.

```
cd graphene #enter cloned dir
git submodule update --init -- Pal/src/host/Linux-SGX/sgx-driver/
```

Then to begin the building process, run
```
make SGX=1
```
Depending on if you have installed the graphene driver or not it might ask for the path to the _SGXDataCenterAttestationPrimitives_ repo you cloned earlier. Provide it with the absolute path to the _driver/linux_ directory of the repository and it will be happy.
