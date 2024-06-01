#!/bin/bash

sudo docker start TPE
sudo docker exec -ti TPE make clean -C/root/Toolchain
sudo docker exec -ti TPE make clean -C/root/
# sudo docker exec -ti TPE make -C/root/Toolchain
# sudo docker exec -ti TPE make -C/root/
sudo docker stop TPE