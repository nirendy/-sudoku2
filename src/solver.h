#ifndef HW2_SOLVER_H
#define HW2_SOLVER_H

#include "game.h"
#include "main_aux.h"


int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues);

void generateGame(Game *game, int fixedAmount);

Bool solveBoard(const Board userBoard, Board toSolveBoard, Bool isDeterministic);

FinishCode countPossibleSolutions(Board board);

Bool isSolved(Game *game);

void updateErrorMatrix(Game *game, Input input);

#endif

