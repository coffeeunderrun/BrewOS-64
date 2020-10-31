FROM debian:stretch

ENV DEBIAN_FRONTEND noninteractive

WORKDIR /usr/src

RUN apt-get --quiet --yes update \
 && apt-get --quiet --yes install build-essential gawk texinfo bison file wget \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists

RUN wget --quiet https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.gz \
 && tar xvzf binutils-2.35.tar.gz \
 && cd binutils-2.35 \
 && ./configure --prefix=/usr/local --target=x86_64-elf --with-sysroot --disable-multilib --disable-nls \
 && make \
 && make install-strip \
 && cd .. \
 && rm -rf binutils-2.35 \
 && rm binutils-2.35.tar.gz

RUN wget --quiet https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz \
 && tar xvzf gcc-10.2.0.tar.gz \
 && cd gcc-10.2.0 \
 && contrib/download_prerequisites \
 && cd .. \
 && mkdir build-gcc \
 && cd build-gcc \
 && ../gcc-10.2.0/configure --prefix=/usr/local --target=x86_64-elf --enable-languages=c,c++ --without-headers --disable-multilib --disable-nls \
 && make all-gcc\
 && make all-target-libgcc\
 && make install-strip-gcc\
 && make install-strip-target-libgcc \
 && cd .. \
 && rm -rf build-gcc \
 && rm -rf gcc-10.2.0 \
 && rm gcc-10.2.0.tar.gz

RUN wget --quiet https://ftp.gnu.org/gnu/gdb/gdb-10.1.tar.gz \
 && tar xvzf gdb-10.1.tar.gz \
 && cd gdb-10.1 \
 && ./configure --prefix=/usr/local --target=x86_64-elf --disable-nls \
 && make \
 && make install-strip \
 && cd .. \
 && rm -rf gdb-10.1 \
 && rm gdb-10.1.tar.gz
