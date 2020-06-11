**This document only includes our own efforts to port Erlang to SCONE. But after contacting their team, we discovered that they are working on official language support. This image would provide some support for Erlang and can possibly be requested by manually emailing the Scontain team (at this time).**
This private docker image adds a patch to the Erlang process (_sys\_drivers.c_) which could be how they succeeded in building it, as well as adding settings such as _SCONE\_HEAP_.

Useful resource: <https://sconedocs.github.io>

# Issue

We tried to port Erlang to SCONE which seemed to require us to build Erlang with the SCONE cross compiler.
Possibly to link in necessary libraries and shielding layer?
In this stage we had issues with the memory allocation used, although we manage to get a fair bit in the compilation process.


Are all system calls required supported to enable Erlang to start the processes required for remote communication?
Since SCONE is run inside a docker container it will have its own hostname and IP address which also means that EPMD needs to be launched in such an environment which adds an additional binary that needs to run in the enclave.


# Prerequisites

## Docker


```
sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io
sudo usermod -aG docker $USER
```

## Get Docker Scone repo access

Go to Scone's website and request community/evaluation access for the private Docker repositories.

Get Erlang into enclave (won't build yet)

Use our [Dockerfile](../scone/Dockerfile).

Run the following in the [folder](../scone) where the Dockerfile is located. Probably best if the directory is otherwise empty

`docker build --pull -t scone-docker-erlang .`

Erlang should now build but it will fail because of memory allocation issues (Erlang super carrier). Trying to patch `erl_mmap.h` to use less memory such as 16 MB will remove the super carrier issue but the process will fail at `sl_alloc: Cannot allocate 42160 bytes of memory (of type "prepared_code").`

