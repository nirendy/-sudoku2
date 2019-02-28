#ifndef HW2_PARSER_H
#define HW2_PARSER_H

#include <stdio.h>
#include <string.h>
#include "SPBufferset.h"
#include "main_aux.h"


void printBoard(Board matrix, BoolBoard fixed_matrix);

FinishCode parseDimension();
FinishCode parseHintsAmount(int *hintsAmount);

FinishCode parseCommand(Input *input);


#endif

