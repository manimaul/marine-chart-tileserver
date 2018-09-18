FROM debian:stretch
MAINTAINER manimaul@gmail.com
ENV PROXYGEN_VERSION="2018.08.20.00"

## Proxygen Build
RUN apt-get update && \
    apt-get install -y curl sudo zlib1g-dev && \
    curl -Lo proxygen.tar.gz https://github.com/facebook/proxygen/archive/v${PROXYGEN_VERSION}.tar.gz && \
    tar -xzvpf proxygen.tar.gz && \
    cd proxygen-${PROXYGEN_VERSION}/proxygen && \
    ./deps.sh -j $(nproc) && \
    ./reinstall.sh

## Cmake 3.12
RUN cd /opt && \
    curl -Lo cmake.tar.gz https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz && \
    tar -xf cmake.tar.gz

## Marine Chart TileServer Build
COPY CMakeLists.txt /build/CMakeLists.txt
COPY src/ /build/src/
RUN cd build && \
    mkdir _build && \
    cd _build && \
    /opt/cmake-3.12.2-Linux-x86_64/bin/cmake .. && \
    make -j $(nproc) && \
    make install

CMD "/opt/mct/bin/marine_chart_tileserver"