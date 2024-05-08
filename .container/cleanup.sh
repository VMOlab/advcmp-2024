#! /bin/bash
# Remove a container and its image.
# VMO Lab, 2024

container_name=advcmp-dev
image_name=advcmp-dev
directory_name=advcmp

docker rm -f $container_name
docker rmi $image_name
rm -rf ../$directory_name/build