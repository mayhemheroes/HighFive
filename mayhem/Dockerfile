# Build Stage
FROM --platform=linux/amd64 ubuntu:22.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y clang cmake git libhdf5-dev \
    libboost-dev libboost-serialization1.74-dev libboost-system1.74-dev

## Add source code to the build stage.
ADD . /highfive
WORKDIR /highfive

## Build
RUN mkdir build && cd build && \
    cmake .. -DHIGHFIVE_FUZZ=On && \
    make -j$(nproc)

# Package Stage
FROM --platform=linux/amd64 ubuntu:22.04
COPY --from=builder /highfive/build/mayhem/fuzz_highfive /fuzz_highfive
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y lib32stdc++6 libstdc++6 lib32stdc++6 libstdc++6 libhdf5-103-1 \
    libssl3 libcurl4 libsz2 zlib1g libc6-i386 libc6 libgcc-s1 lib32gcc-s1 libc6 libc6-i386 libnghttp2-14 libidn2-0 \
    librtmp1 libssh-4 libpsl5 libssl3 libgssapi-krb5-2 libldap-2.5-0 libzstd1 libbrotli1 libaec0 libunistring2 \
    libgnutls30 libhogweed6 libnettle8 libgmp10 libkrb5-3 libk5crypto3 libcom-err2 libkrb5support0 libsasl2-2 \
    libbrotli1 libp11-kit0 libtasn1-6 libkeyutils1 libc6 libc6-i386 libffi8

## Set up fuzzing!
ENTRYPOINT []
CMD /fuzz_highfive
