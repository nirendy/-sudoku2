#ifndef HW2_SOLVER_H
#define HW2_SOLVER_H

#include "game.h"
#include "main_aux.h"

/*Solver module - responsible for board solving using the backtracking algorithm*/

int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues);

FinishCode countPossibleSolutions(Board board);

Bool isFullUserBoard(Game *game);

void updateAfterSetErrorMatrix(Game *game, Input input);

void updateAfterClearErrorMatrix(Game *game, Input input);

int getEmptyCells(Board board, Coordinate *emptyCells);

int getFilledCells(Board board, Coordinate *emptyCells);

void updateWholeErrorMatrix(Game *game);

void clearBoolBoard(BoolBoard board);

Bool fillSolutionMatrix(Board board, Board solutionBoard);

Bool guessBoard(Board board, Board solutionBoard, double threshold);

void guessHint(Board board, Coordinate coordinate);

void clearBoard(Board board);

#endif

