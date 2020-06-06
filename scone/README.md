**tl;dr** this works and can provide very basic Erlang functionality, however it requires access to the private and proprietary Docker image from SCONE.

# Erlang in SCONE

## Building
docker build --pull -t scone-docker-erlang .

## Description taken from our thesis

As we had rely in prebuilt Docker images from a private repository without available
source code, most work we did on SCONE is based on personal communications with
the their team and especially Christof Fetzer. First, a request had to be made to
the Scontain team to acquire access to their Docker images. Access was granted and
a Dockerfile was created that utilized their SCONE crosscompiler image to compile
Erlang/OTP for SCONE's shim layer. Building Erlang/OTP proved to be quite difficult
because Erlang requires quite a lot of syscalls and the runtime allocates a lot
of memory if no options are provided. A patch was produced to force Erlang to use
less memory by default by adjusting the default values, but this still did not help
since Erlang just could not start in the SCONE environment to completely build all
components. The SCONE developers were asked for support which lead to them sup-
plying an experimental image specifically for beam with a partially working Erlang
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
relying on the SCONE developers, no further progress was made on this front. The
SCONE team is working on supporting
