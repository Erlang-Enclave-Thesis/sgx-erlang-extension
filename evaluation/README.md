We conducted simple measurements of syscalls (with strace) and execution time (with timerfd in Erlang).
The raw data is provided in this directory, below we summarize the results (copied from our thesis).



# strace

The syscalls performed by BEAM were measured to get an idea of what the overhead could be for running Erlang in SGX as well as to determine what syscalls need to be supported inside of the enclave.
The measurements were made with `strace` to determine what and how many syscalls are performed by BEAM for simple workloads, including all calls made by any children the process creates.
The same measurements were also made with a functionally equivalent Java workload for JVM) as a reference point since that is a VM runtime which has been ported to SGX.

The workloads tested consist of two simple hello world programs, the first only
performs a simple print while the second launches five additional threads which
all print a message. The compilation of the programs are not included in these
measurements. As seen in our data, Erlang performs vastly more syscalls for the
basic hello world program, around 40% more types of syscalls and 940% more calls
in total. When adding threading, JVM requires more syscalls while Erlang's behavior
does not change, bringing the difference down to 660%. It is worth nothing that
while the calls made by the JVM were quite stable, the total number of calls for
BEAM peaked at 40k the first execution to vary between 15-25k for subsequent calls.
For four executions the amount of syscalls made by BEAM averaged around 25k, but
this is only a estimate as the numbers varied greatly between executions.
The Java runtime differs in many ways from BEAM and if we would introduce additional
functionality in the Java program such as multi-threading we believe that
the numbers should converge, as BEAM already includes many of this functionality
at start. As in this case, the BEAM syscalls remained the same while JVM performed
substantially more syscalls when threading was used. We assume the reason
for this is because BEAM by default creates multiple schedulers regardless of the
program executed while Java is more conservative. Additionally, for BEAM (and
JVM we assume) the number of calls can be reduced by tweaking build options for
the runtime. It is also important to note that far from every call is vital for the
performance of the runtime, as a majority calls performed by BEAM is done by the
scheduler, e.g. probing cpus (`sched_yield`) and sleeping workloads for microsecond
amounts (`timerfd`).


# timer

The time of executing the prototype functions in C from Erlang was measured for
both C Node and NIF and both with SGX and similar functionality completely with-
out SGX. The basic function which provides the ability to increment and return
a number from inside the enclave were tested for both designs. Additionally, the
RSA functionality with OpenSSL was tested for the NIF. The tests were conducted
using a simple wrapper function which calls Erlang `timer:tc`. These tests are available
as `eval_increment` and `eval_RSA` in the source code which we provide (`enclave_communicator.erl`).
The tests conducted only measure the execution time of said Erlang functions and
the results may vary depending on other load on the device that moment or whether
the data was in cache at the moment of execution, especially as we are counting microseconds.
Regardless, general conclusions can be drawn from this data about the
delay caused by using SGX without optimizations. However, as we measure from
time of call on the untrusted side, until the enclave returns, the majority of delay
is due to context switches.
The data does therefore not show the effectiveness of executing inside
of SGX which should yield similar performance as it is executed in the untrusted area
on the CPU with encryption performed in hardware.
What it does show is the worst case overhead, caused by SGX for a given workload.

The overhead percentage changed drastically depending on the workload,
however the overhead in absolute numbers was more stable as expected.
The overhead ranged from 27 to 250μs, excluding the return function and RSA key gen which
returned negative overhead or varied a lot.
From this data we observe that the overhead of calling a function in SGX without any optimization techniques
seems to be around 30μs. The OpenSSL functionality RSA signing and encryption
had greater overhead, but this can be due to the fact
that it may perform multiple context switches to provide the necessary functionality.

