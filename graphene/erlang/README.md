# Erlang

### Regular build:

`GRAPHENEDIR=/home/ericsson/graphene SGX=1 make`

### Debug build:

`GRAPHENEDIR=/home/ericsson/graphene SGX=1 DEBUG=1 make`

### Run after building by executing

`SGX=1 /home/ericsson/graphene/Runtime/pal_loader erlexec.manifest`
