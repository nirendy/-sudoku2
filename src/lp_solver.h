#ifndef SUDOKU_CONSOLE_LP_SOLVER_H
#define SUDOKU_CONSOLE_LP_SOLVER_H

#include "main_aux.h"

/* LP-Solver module - responsible for LP nad ILP board solutions
 *
 * fillSolutionMatrix           - Fill solution matrix
 * guessHint                    - Guess hint by printing the options with their probabilities
 * guessBoard                   - Guess board
 *
 * function return a Boolean specifies if the command successfully completed
 * */

Bool fillSolutionMatrix(Board board, Board solutionBoard);

void guessHint(Board board, Coordinate coordinate);

Bool guessBoard(Board board, Board solutionBoard, double threshold);

#endif
