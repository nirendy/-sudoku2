cmake_minimum_required(VERSION 3.12)
project(sudoku2 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic")
add_executable(sudoku2 src/main.c src/main_aux.c src/main_aux.h src/solver.c src/solver.h src/game.c src/game.h src/parser.c src/parser.h)