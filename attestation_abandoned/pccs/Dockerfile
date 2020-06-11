
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
#


FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive
ENV USER=root

RUN apt update && apt dist-upgrade -y

RUN apt install -y wget curl gnupg git build-essential

RUN wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' > /etc/apt/sources.list.d/intel-sgx.list
RUN apt update

RUN curl -sL https://deb.nodesource.com/setup_12.x | bash -

RUN apt install -y nodejs

RUN apt install -y --no-install-recommends libsgx-dcap-ql libsgx-ae-qve
RUN apt install -y --no-install-recommends sgx-dcap-pccs

WORKDIR /opt/intel/sgx-dcap-pccs

RUN npm install --engine-strict

COPY config.json /opt/intel/sgx-dcap-pccs/config/production.json

RUN mkdir ssl_key

RUN openssl genrsa 1024 > ssl_key/private.pem 
RUN openssl req -new -key ssl_key/private.pem -out ssl_key/csr.pem -subj "/C=SE/ST=VG/L=Gothenburg/O=Ericsson/OU=Mobility/CN=localhost"
RUN openssl x509 -req -days 365 -in ssl_key/csr.pem -signkey ssl_key/private.pem -out ssl_key/file.crt

ENV NODE_ENV=production

STOPSIGNAL SIGHUP

EXPOSE 8081/tcp

ENTRYPOINT ["node", "pccs_server.js"]

