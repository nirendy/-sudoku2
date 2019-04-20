#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

//void insertFirst(Node **head_ref, DataNode *new_data) {
//
//    Node *new_node = (Node *) malloc(sizeof(Node));
//    new_node->data = new_data;
//    new_node->next = (*head_ref);
//    new_node->prev = NULL;
//    if ((*head_ref) != NULL)
//        (*head_ref)->prev = new_node;
//    (*head_ref) = new_node;
//}

//void insertLast(Node **head_ref, DataNode *new_data) {
//
//    Node *new_node = (Node *) malloc(sizeof(Node));
//    Node *last = *head_ref;
//    new_node->data = new_data;
//    new_node->next = NULL;
//    if (*head_ref == NULL) {
//        new_node->prev = NULL;
//        *head_ref = new_node;
//        return;
//    }
//    while (last->next != NULL){
//        last = last->next;
//    }
//    last->next = new_node;
//    new_node->prev = last;
//
//}


void insertAfterNode( Node *curNode, DataNode *new_data) {
    Node *new_node;

    if (curNode == NULL) {
        printf("the given previous node cannot be NULL");
        return;
    }

    new_node = (Node *) malloc(sizeof(Node));
    new_node->currDataNode = new_data;
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL) {
        new_node->next->prev = new_node;
    }
}

void clearListFromNode(Node *curNode) {
    Node *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        freeData(&(curNode->currDataNode));
        free(curNode);
        curNode = nextNode;
    }

}

void freeData(DataNode **head_data_ref) {

    DataNode *curNode = *head_data_ref;
    DataNode *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }
}

DataNode *CreateFirstDataNode() {
    DataNode *new_node = (DataNode *) malloc(sizeof(DataNode));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;

}

Node *CreateFirstNode() {
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->currDataNode = NULL;
    return new_node;

}

void insertAfterDataNode(DataNode *curNode, Input redoInput, Input undoInput) {
    DataNode *new_node;

    if (curNode == NULL) {
        printf("the given previous node cannot be NULL");
        return;
    }

    new_node = (DataNode *) malloc(sizeof(DataNode));
    new_node->redoInput = redoInput;
    new_node->undoInput = undoInput;
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;
}

DataNode *getLastDataNode(DataNode *currDataNode) {
    while (currDataNode->next != NULL) {
        currDataNode = currDataNode->next;
    }
    return currDataNode;
}

DataNode *getFirstDataNode(DataNode *currDataNode) {
    while (currDataNode->isFirst == false) {
        currDataNode = currDataNode->prev;
    }
    return currDataNode;
}
