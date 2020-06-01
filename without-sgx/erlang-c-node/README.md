
# C Node

## How to connect Erlang Proceeses to the C Node
The file `enclave_communicator.erl` can be used as a start for a program
connecting to the C Node.

Remember to start `erl` with a name and the correct secret cookie
```
make
erl -sname e1 -setcookie "Very Secret Cookie"
c(enclave_communicator).
timer:tc(enclave_communicator, increment,"").
timer:tc(enclave_communicator, return,"").
```


