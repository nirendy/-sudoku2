#ifndef HW2_SOLVER_H
#define HW2_SOLVER_H

#include "game.h"
#include "main_aux.h"


int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues);

void generateGame(Game *game, int fixedAmount);

Bool solveBoard(const Board userBoard, Board toSolveBoard, Bool isDeterministic);

FinishCode countPossibleSolutions(Board board);

Bool isSolved(Game *game);

void updateAfterSetErrorMatrix(Game *game, Input input);

void updateAfterClearErrorMatrix(Game *game, Input input);

int getEmptyCells(Board board, Coordinate *emptyCells);

void updateWholeErrorMatrix(Game *game);

void clearBoolBoard(BoolBoard board);

void fillSolutionMatrix(Board board, Board solutionBoard);

void fillSolutionProbabiltyMatrix(Board board, ProbBoard probBoard, Board solutionBoard);

#endif

