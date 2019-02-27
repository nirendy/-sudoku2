#!/bin/bash 
cd $(dirname $0)
cd src
make
cd ..

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

mkdir -p output
mkdir -p output/expected

for file in $(ls tests); do
    if [[ "$file" != "results_test15" ]]; then
        continue
    fi
    echo "Running test: $file"
     question/HW3 3 < tests/"$file" > output/expected/"$file".exp
     src/HW3 3 < tests/"$file" > output/"$file".out
    if diff output/"$file".out output/expected/"$file".exp; then
        echo -e "Result: ${GREEN}PASS${NC}"
    else
        echo -e "Result: ${RED}FAIL${NC}"
    fi
done
