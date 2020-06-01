
# SCONE Evaluation


## What is working?
Basic Erlang workloads can be executed in the SCONE enclave.

## What does not work?

### External communication
Communication with other Erlang processes outside the SCONE enclave does not
work. The reason for this seems to be that since SCONE uses a docker
environment the "host" running the enclaved ERTS can't talk with the EPMD
outside of the enclave. It can not start its own EPMD either. The reasons for
this is currently unknown. Even starting EPMD deamonized separately does not
work.

