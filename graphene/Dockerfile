
#
# Docker Graphene Proof of Concept
#
# Copyright (c) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB
# All rights reserved.
#
# NOTE: This Dockerfile was only used for a proof of concept testing to build
# Graphene in a Docker environment which could be useful on some systems. It
# was not really used in our thesis other than as a playground when building
# Graphene.
#
# This file was fully created by us and is therefore safely licensed under the
# BSD 3-Clause License
#
#

FROM ubuntu:18.04 AS buildenv

RUN apt update && apt dist-upgrade -y

RUN apt install -y wget gnupg git build-essential autoconf gawk bison \
    libprotobuf-c-dev protobuf-c-compiler python3-protobuf \
    linux-headers-5.3.0-46-generic \
    libcurl4-openssl-dev


RUN wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' > /etc/apt/sources.list.d/intel-sgx.list
RUN apt update

# RUN apt install -y libsgx-dcap-ql libsgx-dcap-ql-dev libsgx-ae-qve libsgx-dcap-default-qpl

WORKDIR /opt/intel

ENV SGXSDKBIN=sgx_linux_x64_sdk_2.9.100.2.bin

RUN wget https://download.01.org/intel-sgx/sgx-dcap/1.5/linux/distro/ubuntuServer18.04/${SGXSDKBIN} && chmod +x ${SGXSDKBIN} && yes yes | ./${SGXSDKBIN} && rm ${SGXSDKBIN}

RUN echo "source /opt/intel/sgxsdk/environment" >> /root/.bashrc
RUN chmod +x /opt/intel/sgxsdk/environment

WORKDIR /

RUN git clone --branch DCAP_1.5 --depth 1 https://github.com/intel/SGXDataCenterAttestationPrimitives.git DCAP

RUN git clone https://github.com/oscarlab/graphene.git

WORKDIR /graphene

RUN git submodule update --init -- Pal/src/host/Linux-SGX/sgx-driver/

WORKDIR /graphene/Pal/src/host/Linux-SGX/sgx-driver/

RUN echo "/DCAP/driver/linux" | make

WORKDIR /graphene

RUN SGX=1 make

ENTRYPOINT ["/bin/bash"]

