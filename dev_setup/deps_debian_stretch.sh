#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

PROXYGEN_VERSION="2018.08.20.00"
pushd $DIR/scratch

sudo apt-get update
sudo apt-get install -y curl sudo zlib1g-dev
curl -Lo proxygen.tar.gz https://github.com/facebook/proxygen/archive/v${PROXYGEN_VERSION}.tar.gz
tar -xzvpf proxygen.tar.gz
pushd proxygen-${PROXYGEN_VERSION}/proxygen
./deps.sh -j $(nproc)
./reinstall.sh
popd
popd
