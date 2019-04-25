#!/bin/bash 

cd $(dirname $0)
pwd
./create_golden.sh

# cd $(dirname $0)/src
# make all
#
# cd $(dirname $0)
# ./create_expected.sh
#
# cd $(dirname $0)
# ./test.sh
