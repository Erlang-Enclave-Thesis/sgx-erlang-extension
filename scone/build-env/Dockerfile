FROM alpine:3 AS build

WORKDIR /erlang

ENV otpVer 22.3

ENV otpDir otp_src_${otpVer}

ENV otpTarball ${otpDir}.tar.gz

RUN wget http://erlang.org/download/${otpTarball}

RUN tar -xf ${otpTarball} && mv ${otpDir} erlsrc

WORKDIR /erlang/erlsrc

ENV ERL_TOP /erlang/erlsrc

RUN apk add make

RUN apk add gcc

RUN apk add libc-dev

RUN apk add perl

RUN apk add ncurses-dev

RUN ./configure

RUN make

#FROM sconecuratedimages/fork:crosscompiler AS scone

#COPY --from=build /erlang/erlsrc /erlang/erlsrc

#FROM alpine:3 AS final

#COPY --from=scone /opt/scone/lib/ld-scone-x86_64.so.1 /opt/scone/lib/ld-scone-x86_64.so.1
#COPY --from=scone /opt/scone/cross-compiler/x86_64-linux-musl/lib/libc.scone-x86_64.so.1 /opt/scone/cross-compiler/x86_64-linux-musl/lib/libc.scone-x86_64.so.1
#COPY --from=scone /etc/sgx-musl.conf /etc/sgx-musl.conf

CMD /bin/sh -c "/erlang/erlsrc/bin/erl"

