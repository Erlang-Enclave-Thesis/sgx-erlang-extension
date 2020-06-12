
# Erlang with Graphene

The files in this directory shows our progress with BEAM in Graphene, buildable with the Makefile.
We also provide a Dockerfile to build Graphene itself in Docker.

Although the vital syscalls seems to be supported in Graphene, the use of MPMC in BEAM hinders Graphene support!

## Prequisites

Install Graphene. This can be done as per our instructions in [../documentation/graphene-setup.md](../documentation/graphene-setup.md),
or by using our [Dockerfile](building-graphene/Dockerfile) to build in Docker.
When testing Graphene we built it by the first method, without Docker.

## Our Implementation

#### Regular build

`GRAPHENEDIR=/home/ericsson/graphene SGX=1 make`

#### Debug build

`GRAPHENEDIR=/home/ericsson/graphene SGX=1 DEBUG=1 make`

#### Run after building

`SGX=1 /home/ericsson/graphene/Runtime/pal_loader erlexec.manifest`


## Current Status

We worked with the Graphene community to investigate how Erlang can be supported by their framework.
The important point we discovered with their help, is the problem with MPMC pipes in Erlang as per <https://github.com/Erlang-Enclave-Thesis/graphene-erlang/issues/2>.

There are two methods of overcoming this: either to add support in Graphene for a special keyword that promises all pipes are only used intra-process (an option in manifest like: _sgx.all_pipes_are_inside_one_process_) (sounds risky..?) OR exchange MPMC pipes in Erlang for end-to-end communication (requires changes to the BEAM runtime).

See also:

https://github.com/Erlang-Enclave-Thesis/graphene-erlang/issues?q=

https://github.com/oscarlab/graphene/issues/1464

https://github.com/oscarlab/graphene/issues/1463

## Additional Details

For Graphene, most syscalls performed by beam for our hello world program (`without-sgx/hello-worlds/hello_world.erl`) are
supported, all but 9 out of the 67 according to the Graphene documentation.
An additional five were claimed to have partial support.
However, when cross-referencing with their source code,
not nine but only seven syscalls were not implemented at all.
The seven completely unsupported syscalls in Graphene were: `sysinfo`,
`prlimit64`, `madvice`, `timerfd_settime`, `timerfd_create`, `prctl`, and `statfs`.
Seemingly,
most of the unsupported syscalls could be ignored at the cost of performance and
stability which was partly confirmed by Graphene.

It became evident that although some syscalls could be ignored or later supported by Graphene, the main issue is BEAM's use of MPMC pipes instead of point-to-point communication in some cases.
According to Graphene developer [Dmitrii Kuvaiskii](https://github.com/dimakuv), there is no way for Graphene to determine if the pipes are used for inter-thread or inter-process communication (https://github.com/Erlang-Enclave-Thesis/graphene-erlang/issues/2).
As a result, Graphene chooses to TLS encrypt all pipes although he notes it could be possible to introduce a keyword in Graphene if it is certain that _all_ pipes are only used within one process and therefore not necessary to encrypt.
The Erlang experts at Ericsson remarked that these pipes are used for two things.
Namely, they are used to wake up schedulers sleeping in `poll/epoll/select` configured in `erl_poll.c` as well as by Erlang ports by the function `open_port({spawn,_})`.
The later uses UNIX domain sockets and pipes to create child processes such as `intel_gethost` and modification of this feature would require much more work in `sys_drivers.c`.

