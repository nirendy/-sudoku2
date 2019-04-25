#!/bin/bash 
cd $(dirname $0)/../src

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

mkdir -p output
mkdir -p output/expected

for file in $(ls ../tests); do
    echo "Running test: $file"
    ../question/sudoku-console < ../tests/"$file" > ../output/expected/"$file".exp
done
