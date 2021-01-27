#!/bin/bash

IMAGE_NAME="gcc-clang:10"
REMOVE_CONTAINER="--rm"

ARG=$1

# if no arg is given
if [[ -z $ARG ]]
then
    # then run the script
    docker container run -u $USER:$(id -u) \
                         $REMOVE_CONTAINER \
                         -v "$PWD":/usr/src/concurrent-queue \
                         -w /usr/src/concurrent-queue \
                         $IMAGE_NAME \
                         ./run.sh
else
    # else run interactively
    docker container run -u $USER:$(id -u) \
                         $REMOVE_CONTAINER \
                         -v "$PWD":/usr/src/concurrent-queue \
                         -w /usr/src/concurrent-queue \
                         -it $IMAGE_NAME \
                         /bin/bash
fi
