#!/bin/bash 
cd $(dirname $0)/..

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

mkdir -p output
mkdir -p output/expected

for file in $(ls tests); do
    echo "Running test: $file"
     src/HW3 3 < tests/"$file" > output/"$file".out
    if diff output/"$file".out output/expected/"$file".exp; then
        echo -e "Result: ${GREEN}PASS${NC}"
    else
        echo -e "Result: ${RED}FAIL${NC}"
    fi
done
