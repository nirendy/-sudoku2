#ifndef SUDOKU_CONSOLE_LP_SOLVER_H
#define SUDOKU_CONSOLE_LP_SOLVER_H

#include "main_aux.h"

/* LP-Solver module - responsible for LP nad ILP board solutions
 *
 * fillSolutionMatrix           - Fill solution matrix
 * guessHint                    - Guess hint
 * guessBoard                   - Guess board
 * */

Bool fillSolutionMatrix(Board board, Board solutionBoard);

void guessHint(Board board, Coordinate coordinate);

Bool guessBoard(Board board, Board solutionBoard, double threshold);

#endif
