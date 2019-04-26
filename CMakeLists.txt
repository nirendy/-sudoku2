cmake_minimum_required(VERSION 2.8)
project(sudoku-console C)

set(CMAKE_C_STANDARD 90)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -ansi -O3 -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -ansi -Wall -Wextra -pedantic-errors")
add_executable(sudoku-console
        src/main.c
        src/main_aux.c
        src/main_aux.h
        src/solver.c
        src/solver.h
        src/game.c
        src/game.h
        src/parser.c
        src/parser.h
        src/file_handler.c
        src/file_handler.h
        src/linked_list.c
        src/linked_list.h
        src/gurobi_c2.c
        src/gurobi_c2.h
        src/lp_solver.c
        src/lp_solver.h
        src/game_aux.c src/game_aux.h)