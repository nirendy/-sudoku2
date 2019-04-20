//
// Created by Allen on 19/04/2019.
//

#ifndef SUDOKU2_LINKED_LIST_H
#define SUDOKU2_LINKED_LIST_H

#endif //SUDOKU2_LINKED_LIST_H
#include "stdlib.h"

struct Data {

};


#include <stdio.h>
#include <stdlib.h>

// A linked list node
struct Node {
    struct Data data;
    struct Node* next;
    struct Node* prev;
};

void freeData(struct Data *data);