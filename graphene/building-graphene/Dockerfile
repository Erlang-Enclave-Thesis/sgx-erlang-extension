#
# BSD 3-Clause License
#
# Copyright (c) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


# Build Graphene in Docker
#
# This Dockerfile was only used for a proof of concept testing to build
# Graphene in a Docker environment which could be useful on some systems. It
# was not really used in our thesis other than as a playground when building
# Graphene.
#
# This file was fully created by us


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

