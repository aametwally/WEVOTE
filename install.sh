#!/bin/bash


VERSION="1.2.0"

if [[ ! -d bin ]];then
mkdir bin

fi
make -C src
make -C src install
