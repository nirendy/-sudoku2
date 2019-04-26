#ifndef SUDOKU2_LINKED_LIST_H
#define SUDOKU2_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"

/* Linked list module - responsible for handling the linked-list data structure that holds all the moves history */

/* Data Node Funcs */

Node *CreateFirstNode();

Node *insertAfterNode(Node *curNode);

Node *getFirstNode(Node *curNode);

void clearListFromNode(Node *curNode);

/* Data Node Funcs */

DataNode *insertAfterDataNode(DataNode *curNode, Input redoInput, Input undoInput);

DataNode *getFirstDataNode(DataNode *currDataNode);

DataNode *getLastDataNode(DataNode *currDataNode);

void clearWholeDataList(DataNode *curNode);


#endif

