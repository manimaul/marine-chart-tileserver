FROM debian:stretch-slim
MAINTAINER manimaul@gmail.com
ENV PROXYGEN_VERSION="2018.08.20.00"

## Cmake 3.12
RUN apt-get update && \
    apt-get install -y curl sudo zlib1g-dev && \
    cd /opt && \
    curl -Lo cmake.tar.gz https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz && \
    tar -xf cmake.tar.gz

ENV PATH="/opt/cmake-3.12.2-Linux-x86_64/bin/:$PATH"

## Proxygen Build
RUN curl -Lo proxygen.tar.gz https://github.com/facebook/proxygen/archive/v${PROXYGEN_VERSION}.tar.gz && \
    tar -xzvpf proxygen.tar.gz && \
    cd proxygen-${PROXYGEN_VERSION}/proxygen && \
    ./deps.sh -j $(nproc) && \
    ./reinstall.sh

CMD "/bin/bash"