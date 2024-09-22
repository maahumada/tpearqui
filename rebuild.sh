#!/bin/bash

CONTAINER_NAME=SOQEMU

sudo docker start ${CONTAINER_NAME}
sudo docker exec -ti ${CONTAINER_NAME} make clean -C/root/Toolchain
sudo docker exec -ti ${CONTAINER_NAME} make clean -C/root/
sudo docker exec -ti ${CONTAINER_NAME} make -C/root/Toolchain
sudo docker exec -ti ${CONTAINER_NAME} make -C/root/
sudo docker stop ${CONTAINER_NAME}