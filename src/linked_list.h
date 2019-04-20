//
// Created by Allen on 19/04/2019.
//

#ifndef SUDOKU2_LINKED_LIST_H
#define SUDOKU2_LINKED_LIST_H

#endif //SUDOKU2_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"

struct DataNode {
    Input input;
    struct DataNode* next;
    struct DataNode* prev;
};



// A linked list node
struct Node {
    struct DataNode* data;
    struct Node* next;
    struct Node* prev;
};

void freeData(struct DataNode **data);
