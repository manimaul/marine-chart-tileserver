#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

pushd $DIR/..

DOCKER_TAG=manimaul/mct:git-$(git rev-parse --short=8 HEAD)
echo $DOCKER_TAG

docker build -f docker/mct/Dockerfile -t $DOCKER_TAG .
docker push $DOCKER_TAG

popd