#ifndef HW2_PARSER_H
#define HW2_PARSER_H

#include <stdio.h>
#include <string.h>
#include "SPBufferset.h"
#include "main_aux.h"

#define MAX_STRING_LEN 1024
#define MAX_COMMAND_LEN 20

void printBoard(Board matrix, BoolBoard fixed_matrix);
void initInput(Input *returnedInput);
float getFloat(char *str);

FinishCode parseHintsAmount(int *hintsAmount);
FinishCode parseCommand(Input *input);


#endif

