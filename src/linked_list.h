#ifndef SUDOKU2_LINKED_LIST_H
#define SUDOKU2_LINKED_LIST_H

struct Data {
    int foo; /*TODO: change*/
};


/* A linked list node*/
struct Node {
    struct Data data;
    struct Node* next;
    struct Node* prev;
};

void freeData(struct Data *data);

#endif
