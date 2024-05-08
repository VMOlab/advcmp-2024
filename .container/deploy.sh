#!/bin/bash
# Build Dockerfile and launch a container.
# VMO Lab, 2024

########### CONFIGURATIONS ###########

directory_name=advcmp
container_name=advcmp
image_name=advcmp

######################################

SCRIPT_DIR="$(dirname "$0")"
mkdir -p "$SCRIPT_DIR/../$directory_name"

if [ ! "$(docker ps -a -q -f name=$container_name)" ]; then
    if [[ "$(docker images -q $image_name 2> /dev/null)" == "" ]]; then
        if [ -f "$image_name.tar.gz" ]; then
            # Load pre-built image
            echo -n "Importing image..."
            docker image import $image_name.tar.gz $container_name
            echo "    [DONE]"
        else
            # Build container image
            echo -n "Building image..."
            docker build \
                --build-arg UID=$(id -u) \
                --build-arg GID=$(id -g) \
                -t $image_name $SCRIPT_DIR
            echo "Building image...    [DONE]"
        fi
    fi

    docker run \
        -v $(pwd)/$SCRIPT_DIR/../$directory_name/:/home/ubuntu/$directory_name \
        -h $container_name --name $container_name -it --user ubuntu -d \
        $image_name \
        tail -f /dev/null

else
    echo "$container_name is already running."
fi
