#!/bin/bash 
cd $(dirname $0)/../src
make all
mv sudoku-console ../question/sudoku-console
make clean