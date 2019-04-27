#!/bin/bash 

# chmod 700 ./create_golden.sh
# chmod 700 ./create_expected.sh
chmod 700 ./valgrind-test.sh
# chmod 700 ./test.sh

# ./create_golden.sh
# ./create_expected.sh

cd ../src
make clean
make all

cd ../scripts

# ./test.sh
./valgrind-test.sh