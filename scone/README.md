**tl;dr** SCONE works and can provide very basic Erlang functionality, however it requires access to their private and proprietary Docker image specifically for Erlang (_sconecuratedimages/experimental:erlang_). This image is experimental with limited Erlang functionality. It could also work to cross compile Erlang directly with the regular SCONE image, but we did not succeed with this.

*The Dockerfile in this directory shows our efforts to manually cross compile Erlang for SCONE, and does not work, as described below.*

# Erlang with SCONE

We began with building Erlang with the SCONE cross compiler, however we got stuck with Erlang memory allocation. This implementation is found in this directory in [Dockerfile](Dockerfile).

As we did not manage to build Erlang manually with the regular private SCONE image we asked to use the experimental image instead which is explained last in this readme. The Scontain team is working on adding official support for Erlang.


## Prequisites

Get and install SCONE: [../documentation/scone-setup.md](../documentation/scone-setup.md),

Also see their documentation: <https://sconedocs.github.io>

## Cross compiling Erlang for SCONE 

We began with trying to cross compile Erlang for SCONE, it is currently not working.

### Current Status

With the Dockerfile, the Erlang build will fail because of memory allocation issues.
We tried to patch this with `erl_mmap.h` to use less memory such as 16 MB.
This fixed the previous error "cannot allocate super carrier", but the process will instead fail at `sl_alloc: Cannot allocate 42160 bytes of memory (of type "prepared_code")`.

### Steps to Reproduce

To try to build Erlang, run the command below in the directory where the Dockerfile is located (this directory).
Probably best if the directory is otherwise empty.

`docker build --pull -t scone-docker-erlang .`




### Comments

We tried to "port" Erlang to SCONE which seemed to require us to build Erlang with the SCONE cross compiler.
Possibly to link in necessary libraries and the shielding layer?
In this stage we had issues with the memory allocation used, although we manage to get a fair bit in the compilation process.

Are all system calls required supported to enable Erlang to start the processes required for remote communication?
Since SCONE is run inside a docker container it will have its own hostname and IP address which also means that EPMD needs to be launched in such an environment which adds an additional binary that needs to run in the enclave.

## Using Experimental Erlang Docker Image

Limited Erlang/OTP functionality is working with the SCONE experimental Erlang image!

Mainly, this private docker image adds a patch to Erlang/OTP (`sys_drivers.c`) which could be how they succeeded in building it, as well as setting configurables such as `SCONE_HEAP`.

### Prerequisites

Email Scontain developers to get access to private Erlang protoype image, we received access from Christof Fetzer at Scontain.

### Running

Pulling SCONE's prebuilt Docker image for Erlang

```
docker pull sconecuratedimages/experimental:erlang-22.3.2
```

Run it

```
docker run --rm -it sconecuratedimages/experimental:erlang-22.3.2
```

More details at the Git repository https://github.com/scontain/erlang-examples/

## Summary from Thesis

As we had rely in prebuilt Docker images from a private repository without available
source code, most work we did on SCONE is based on personal communications with
the their team and especially [Christof Fetzer](https://github.com/christoffetzer).
First, a request had to be made to
the Scontain team to acquire access to their Docker images. Access was granted and
a Dockerfile was created that utilized their SCONE crosscompiler image to compile
Erlang/OTP for SCONE's shim layer. Building Erlang/OTP proved to be quite difficult
because Erlang requires quite a lot of syscalls and the runtime allocates a lot
of memory if no options are provided. A patch was produced (`sed` lines in Dockerfile)
to force Erlang to use
less memory by default by adjusting the default values, but this still did not help
since Erlang just could not start in the SCONE environment to completely build all
components. The SCONE developers were asked for support which lead to them supplying
an experimental image specifically for beam with a partially working Erlang
executable. This image could unfortunately not communicate with external Erlang
processes because certain fork operations did not succeed which caused network
related operations to fail, but it could support some basic Erlang functionality.
The functionality was ascertained in SGX simulation mode by running a number of tests
provided by SCONE. Unfortunately the combination of the hardware, bios, and
driver used was not able to execute the image on SCONE's enclave running in hardware
mode, therefore the simulation mode was used to test the functionality of the
image. The SCONE developers claimed to have succeeded in running Erlang in the
SCONE hardware enclave on their systems and did not know why it could not run
in our hardware mode. As little progress could be made without the source code or
relying on the SCONE developers, no further progress was made on this front.
The Scontain team is working on supporting Erlang officially but they still have some
way to go until it is production-ready.
