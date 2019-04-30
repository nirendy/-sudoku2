#ifndef SUDOKU_CONSOLE_GAME_AUX_H
#define SUDOKU_CONSOLE_GAME_AUX_H

#include "main_aux.h"
#include "solver.h"
#include "lp_solver.h"
#include "linked_list.h"
#include "stdio.h"


/*Game Auxiliary module - responsible for aiding the game module with specific game-related functions
 *
 * isGameErroneous                          - Is game erroneous
 * actIfBoardFullAndValid                             - Checks if the the board is full and valid, and acts accordingly
 * clearGame                                - Clear game
 * setCoordinate                            - Set coordinate
 * printChange                              - Print change
 * updateRedoUndoInputsAfterSingleSet       - Update redo undo inputs after single set
 * insertInputsToList                       - Insert inputs to list
 * updateHistoryList                        - Update history list
 * isSolvableBoard                          - Is solvable board
 * chooseCellsToFill                        - Choose cells to fill
 * fillXRandomCells                         - Fill x random cells
 * chooseCellsToClear                       - Choose cells to clear
 * clearRandomCells                         - Clear random cells
 * fillObviousValues                        - Fill obvious values
 *
 * */

/* Legal Checks related */
Bool isGameErroneous(Game *game);

/* Perform Commands Related*/

/*board-changing commands related*/
void actIfBoardFullAndValid(Game *game);

/* Edit related*/
void clearGame(Game *game);

/* Set related*/
void setCoordinate(Game *game, Input input);

/* Undo, Redo related */
void printChange(int i, int j, int value);

/*      Link list History related       */
void updateRedoUndoInputsAfterSingleSet(Game *game, Input in, Input *redo, Input *undo);

void insertInputsToList(Input *redoInputs, Input *undoInputs, int numOfInputs);

int updateHistoryList(Game *game, Board final);

/* Validate related */
Bool isSolvableBoard(Board board);

/* Generate related */
int chooseCellsToFill(Board board, Coordinate *cellsToFill, int sizeToFill);

Bool fillXRandomCells(Board board, Coordinate *cellsToFill, int numToFill);

void chooseCellsToClear(Board board, Coordinate *cellsToClear, int numToClear);

void clearRandomCells(Board board, Coordinate *cellsToClear, int numToClear);

/* Autofill related */
void fillObviousValues(Game *game , Board board);


#endif
