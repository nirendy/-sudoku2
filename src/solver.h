#ifndef HW2_SOLVER_H
#define HW2_SOLVER_H

#include "game.h"
#include "main_aux.h"

/* Solver module - responsible for all the board solving logic -
 * contains the exhaustive backtracking logic and all the board-solving aiding functions
 *
 * getEmptyCells                        - Get empty cells.
 * getFilledCells                       - Get filled cells.
 * hasEmptyCellWithNoPossibleValues     - Has empty cell with no possible values.
 * getPossibleValues                    - Get possible values.
 * isPossibleValue                      - Is value possible.
 * isBoardFull                          - Is board full.
 * clearBoard                           - Clearing board.
 * clearBoolBoard                       - Clearing bool board.
 * isBoardErroneous                     - Is board erroneous.
 * isBoardComplete                      - Is board complete.
 *
 * */

/* Cell Functions*/

int getEmptyCells(Board board, Coordinate *emptyCells);

int getFilledCells(Board board, Coordinate *emptyCells);

Bool hasEmptyCellWithNoPossibleValues(Board board);

/* Values Functions*/

int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues);

Bool isPossibleValue(Board board, Coordinate coordinate, int value);

/* Board Functions*/

Bool isBoardFull(Board board);

void clearBoard(Board board);

void clearBoolBoard(BoolBoard board);

Bool isBoardErroneous(Board board);

Bool isBoardComplete(Board board);


/* Error Matrix Functions*/

void updateAfterClearErrorMatrix(Game *game, Input input);

void updateAfterSetErrorMatrix(Game *game, Input input);

void updateWholeErrorMatrix(Game *game);

/* Count solutions */

/* Stack Node */

int getNumOfPossibleSolutions(Board board);


#endif

