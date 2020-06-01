
# NIF

Does not use SGX what so ever, otherwise same functionality (increment, return)

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


