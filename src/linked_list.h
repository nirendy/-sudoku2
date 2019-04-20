#ifndef SUDOKU2_LINKED_LIST_H
#define SUDOKU2_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"

struct DataNode {
    Bool isFirst;
    Input redoInput;
    Input undoInput;
    struct DataNode* next;
    struct DataNode* prev;
};



// A linked list node
struct Node {
    Bool isFirst;
    struct DataNode* currDataNode;
    struct Node* next;
    struct Node* prev;
};

void freeData(struct DataNode **data);

struct Node * CreateFirstNode();
struct DataNode * CreateFirstDataNode();
struct DataNode* getLastDataNode(struct DataNode *currDataNode);
struct DataNode* getFirstDataNode(struct DataNode *currDataNode);


#endif SUDOKU2_LINKED_LIST_H
