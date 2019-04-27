#ifndef SUDOKU2_FILE_HANDLER_H
#define SUDOKU2_FILE_HANDLER_H


#include <stdio.h>
#include "main_aux.h"

/*File handling module - responsible for dealing with saving|loading files
 *
 * saveGameToFile              - Save game to file
 * setDimensionsFromFile       - Set game dimensions from file
 * generateGameFromFile        - Generate game from file
 *
 * */

FinishCode saveGameToFile(char *filePath, Game *game);

FinishCode setDimensionsFromFile(char *filePath);

FinishCode generateGameFromFile(char *filePath, Game *game);

#endif
