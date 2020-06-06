*tl;dr* this does not work, althought the vital syscalls seems to be supported in Graphene the use of MPMC in BEAM hinders Graphene support.

# Erlang for Graphene

We worked with the Graphene community to investigate how Erlang can be supported by their framework.
The important point we discovered with their help, is the problem with MPMC pipes in Erlang as per <https://github.com/Erlang-Enclave-Thesis/graphene-erlang/issues/2>.

See also:

https://github.com/Erlang-Enclave-Thesis/graphene-erlang/issues?q=

https://github.com/oscarlab/graphene/issues/1464

https://github.com/oscarlab/graphene/issues/1463

## The problem with BEAM + Graphene

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
According to Graphene developer Dmitrii Kuvaiskii, there is no way for Graphene to determine if the pipes are used for inter-thread or inter-process communication (https://github.com/Erlang-Enclave-Thesis/graphene-erlang/issues/2).
As a result, Graphene chooses to _tls_ encrypt all pipes although he notes it could be possible to introduce a keyword in Graphene if it is certain that _all_ pipes are only used within one process and therefore not necessary to encrypt.
The Erlang experts at Ericsson remarked that these pipes are used for two things.
Namely, they are used to wake up schedulers sleeping in `poll/epoll/select` configured in `erl_poll.c` as well as by Erlang ports by the function `open_port({spawn,_})`.
The later uses UNIX domain sockets and pipes to create child processes such as `intel_gethost` and modification of this feature would require much more work in `sys_drivers.c`.


