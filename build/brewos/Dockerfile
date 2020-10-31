FROM debian:stretch

RUN apt-get --quiet --yes update \
 && apt-get --quiet --yes install yasm xorriso qemu \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists

COPY --from=toolchain /usr/local /usr/local

WORKDIR /usr/src/brewos

CMD /bin/bash
