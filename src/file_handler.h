#ifndef SUDOKU2_FILE_HANDLER_H
#define SUDOKU2_FILE_HANDLER_H


#include <stdio.h>
#include "main_aux.h"

FinishCode saveFile(char *filePath, Game *game);

FinishCode setDimentiosFromFile(char *filePath);

FinishCode generateGameFromFile(char *filePath, Game *game);

#endif //SUDOKU2_FILE_HANDLER_H
