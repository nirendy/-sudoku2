#!/bin/bash 

# ./create_golden.sh
# ./create_expected.sh

cd ../src
make clean
make all

cd ../scripts
./test.sh
