#ifndef SUDOKU2_LINKED_LIST_H
#define SUDOKU2_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"

void freeData(DataNode **data);

Node * CreateFirstNode();
DataNode * CreateFirstDataNode();
DataNode* getLastDataNode(DataNode *currDataNode);
DataNode* getFirstDataNode(DataNode *currDataNode);


#endif
