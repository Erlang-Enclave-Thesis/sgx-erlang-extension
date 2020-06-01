FROM sconecuratedimages/fork:crosscompiler

WORKDIR /erlang

ENV otpDir otp_src_22.3

ENV otpTarball ${otpDir}.tar.gz

RUN wget http://erlang.org/download/${otpTarball}

RUN tar -xf ${otpTarball} && mv ${otpDir} erlsrc

WORKDIR /erlang/erlsrc

ENV ERL_TOP /erlang/erlsrc

RUN apk add make

RUN apk add perl

RUN apk add ncurses-dev

RUN ./configure

# Patch mmap to only allocate 16 MB
RUN sed -i 's/ERTS_LITERAL_VIRTUAL_AREA_SIZE (UWORD_CONSTANT(1)\*1024\*1024\*1024)/ERTS_LITERAL_VIRTUAL_AREA_SIZE (UWORD_CONSTANT(1)*16*1024*1024)/' \
	erts/emulator/sys/common/erl_mmap.h

# Try patch makefiles
RUN sed -i 's/ERL = erl -boot start_clean/ERL = erl -boot start_clean +Meamin +MMscs 16 +MIscs 16/' make/otp.mk.in

ENV SCONE_VERSION=1

RUN SCONE_VERSION=1 make

CMD /bin/sh -c "/erlang/erlsrc/bin/erl"

