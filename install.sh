#!/bin/bash


VERSION="1.5.0"

if [[ ! -d bin ]];then
mkdir bin

fi
make -C src
make -C src install
