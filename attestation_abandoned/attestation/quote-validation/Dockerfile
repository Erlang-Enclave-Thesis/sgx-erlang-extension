
FROM ubuntu:18.04 AS buildenv

RUN apt update && apt dist-upgrade -y

RUN apt install -y wget gnupg git build-essential

RUN wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' > /etc/apt/sources.list.d/intel-sgx.list
RUN apt update

RUN apt install -y libsgx-dcap-ql libsgx-dcap-ql-dev libsgx-ae-qve libsgx-dcap-default-qpl

WORKDIR /opt/intel

ENV SGXSDKBIN=sgx_linux_x64_sdk_2.9.100.2.bin

RUN wget https://download.01.org/intel-sgx/sgx-dcap/1.5/linux/distro/ubuntuServer18.04/${SGXSDKBIN} && chmod +x ${SGXSDKBIN} && yes yes | ./${SGXSDKBIN} && rm ${SGXSDKBIN}

RUN echo "source /opt/intel/sgxsdk/environment" >> /root/.bashrc
RUN chmod +x /opt/intel/sgxsdk/environment

WORKDIR /sgx

COPY QuoteVerificationServer QuoteVerificationServer

COPY keys keys

WORKDIR /sgx/QuoteVerificationServer

RUN make

RUN /opt/intel/sgxsdk/bin/x64/sgx_sign sign -key ../keys/enclave_private_key.pem -enclave enclave.so -out enclave.signed.so -config Enclave/Enclave.config.xml

COPY sgx_default_qcnl.conf /etc/sgx_default_qcnl.conf

ENTRYPOINT ["app"]

