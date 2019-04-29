#!/bin/bash 
cd $(dirname $0)/../src

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

mkdir -p ../output
mkdir -p ../output/valgrind_out

for file in $(ls ../tests); do
    echo "Running test: $file"
    valgrind --log-file="../output/valgrind_out/$file.val_out" sudoku-console < ../tests/"$file"
    sed -i 's/^.........//g' ../output/valgrind_out/"$file".val_out
done
