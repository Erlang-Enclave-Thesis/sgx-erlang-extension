
FROM ubuntu:18.04

WORKDIR /sgx

RUN apt update && apt dist-upgrade -y

RUN apt install -y wget gnupg git build-essential

RUN wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' > /etc/apt/sources.list.d/intel-sgx.list
RUN apt update

RUN apt install -y libsgx-dcap-ql libsgx-dcap-ql-dev libsgx-ae-qve libsgx-dcap-default-qpl

RUN wget https://download.01.org/intel-sgx/sgx-dcap/1.5/linux/distro/ubuntuServer18.04/sgx_linux_x64_sdk_2.9.100.2.bin && chmod +x sgx_linux_x64_sdk_2.9.100.2.bin && yes yes | ./sgx_linux_x64_sdk_2.9.100.2.bin

RUN git clone --branch DCAP_1.5 --depth 1 https://github.com/intel/SGXDataCenterAttestationPrimitives.git DCAP

# WORKDIR /sgx/DCAP/QuoteVerification

RUN echo "source /sgx/sgxsdk/environment" >> /root/.bashrc

COPY sgx_default_qcnl.conf /etc/sgx_default_qcnl.conf

ENTRYPOINT ["/bin/bash"]

