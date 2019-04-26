#ifndef SUDOKU_CONSOLE_GAME_AUX_H
#define SUDOKU_CONSOLE_GAME_AUX_H

#include "main_aux.h"
#include "solver.h"
#include "lp_solver.h"
#include "linked_list.h"
#include "stdio.h"

/* Legal Checks related */
Bool isGameErroneous(Game *game);

/* Perform Commands Related*/

/* Edit related*/
void clearGame(Game *game);

/* Set related*/
void setCoordinate(Game *game, Input input);

/* Undo, Redo related */
void printChange(int i, int j, int value);/* Undo, Redo related */

/* Link list History related */
void updateRedoUndoInputsAfterSingleSet(Game *game, Input in, Input *redo, Input *undo);

void insertInputsToList(Input *redoInputs, Input *undoInputs, int numOfInputs);

void updateHistoryList(Game *game, Board final);

/* Validate related */
Bool isSolvableBoard(Board board);

/* Generate related */
int chooseCellsToFill(Board board, Coordinate *cellsToFill, int sizeToFill);

Bool fillXRandomCells(Board board, Coordinate *cellsToFill, int numToFill);

void chooseCellsToClear(Board board, Coordinate *cellsToClear, int numToClear);

void clearRandomCells(Board board, Coordinate *cellsToClear, int numToClear);

/* Autofill related */
void fillObviousValues(Board board);


#endif
