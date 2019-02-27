#ifndef HW2_PARSER_H
#define HW2_PARSER_H

#include <stdio.h>
#include <string.h>
#include "SPBufferset.h"
#include "main_aux.h"


void printSepRow(int size);

void printBoard(const Board matrix, const BoolBoard fixed_matrix , int m, int n);

int parseHintsAmount();

Input parseCommand();

int ClassifyCommand(char *token , Input *returnedInputP);

#endif

