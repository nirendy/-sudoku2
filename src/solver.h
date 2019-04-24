#ifndef HW2_SOLVER_H
#define HW2_SOLVER_H

#include "game.h"
#include "main_aux.h"


int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues);

void old_generateGame(Game *game, int fixedAmount);

Bool solveBoard(const Board userBoard, Board toSolveBoard, Bool isDeterministic);

FinishCode countPossibleSolutions(Board board);

Bool isFullUserBoard(Game *game);

void updateAfterSetErrorMatrix(Game *game, Input input);

void updateAfterClearErrorMatrix(Game *game, Input input);

int getEmptyCells(Board board, Coordinate *emptyCells);

int getFilledCells(Board board, Coordinate *emptyCells);

void updateWholeErrorMatrix(Game *game);

void clearBoolBoard(BoolBoard board);

Bool fillSolutionMatrix(Board board, Board solutionBoard);

void guessBoard(Board board, Board solutionBoard, double threshold);

void guessHint(Board board, Coordinate coordinate);

#endif

