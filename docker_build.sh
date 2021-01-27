#!/bin/bash

IMAGE_NAME="gcc-clang:10"

docker build --build-arg X_USER=$USER \
             --build-arg X_UID=$(id -u) \
             --build-arg X_GID=$(id -g) \
             -t $IMAGE_NAME .

docker image ls
