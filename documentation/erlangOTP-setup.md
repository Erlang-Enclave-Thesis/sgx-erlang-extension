# Building Erlang

We've only built a standard version of Erlang/OTP.
With the default settings (can add flags to ./configure), there was no support for jinterface, odbc, wx, and the Erlang/OTP documentation is not included - in our case.
Note, what is included in this build processes changes according to what dependencies are available on the system with ./configure.

**Future work:** here it could be investigated what build options are available to e.g. lessen the syscalls performed or to lessen the size of the Erlang runtime (which could be ported to SGX enclaves).

## Prerequisites

Regular build tools. Probably installed from adding support for SGX.

Download the Erlang/OTP archive from <https://www.erlang.org/downloads>

Extract the archive

## Installation

Enter the directory and run

```
./configure
make
sudo make install
```

You should now have Erlang/OTP installed.

**HOWEVER!** When building manually instead of installing from repo, EPMD does not seem to start, which is required for C Node and external communication!

# Enable EPMD service

Having EPMD running as a service instead of letting erl control it will allow starting C Nodes without starting and Erlang process beforehand.

If you are using systemd, insert the following in the file _/etc/systemd/system/epmd.service_.

```
[Unit]
Description=Erlang Port Mapper Daemon
After=network.target

[Service]
ExecStart=/usr/local/bin/epmd
Restart=always
Type=simple

[Install]
WantedBy=multi-user.target
```

Remember that if you installed Erlang in another way then it might be located somewhere else. `which epmd` should help you locate epmd.

Run

`sudo systemctl enable epmd.service && sudo systemctl start epmd.service`

to enable and start the service.

## Try Erlang C-Node

In the [../erlang-c-node](../erlang-c-node) directory you can run _make_ to build the project.

Start the application in one terminal running

`./app`

Start the Erlang side by running the following in another terminal

```
erl -sname e1 -setcookie "Very Secret Cookie"
```

Then run

```
c(enclave_communicator).
enclave_communicator:increment().
enclave_communicator:return().
```

to test the application functionality.

