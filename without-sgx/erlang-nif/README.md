Basic NIF example without SGX which supports increment and return functions.
The very same functionlity as the SGX version allowing for comparison.

# NIF

## How to connect Erlang Proceeses to the NIF
The file `enclave_communicator.erl` can be used as a start for a program
using the NIF.

```
make
erl
c(enclave_communicator).
timer:tc(enclave_communicator, increment,"").
timer:tc(enclave_communicator, rtrn,"").
```


