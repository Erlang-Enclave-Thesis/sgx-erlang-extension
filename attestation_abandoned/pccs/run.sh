#!/bin/sh

docker run -v /dev/sgx:/dev/sgx --publish 0.0.0.0:8081:8081 --privileged --rm -d pccs

