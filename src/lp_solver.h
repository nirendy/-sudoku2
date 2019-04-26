#ifndef SUDOKU_CONSOLE_LP_SOLVER_H
#define SUDOKU_CONSOLE_LP_SOLVER_H

#include "main_aux.h"

Bool fillSolutionMatrix(Board board, Board solutionBoard);

void guessHint(Board board, Coordinate coordinate);

Bool guessBoard(Board board, Board solutionBoard, double threshold);

#endif
