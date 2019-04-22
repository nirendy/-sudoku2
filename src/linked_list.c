#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/*
void insertFirst(Node **head_ref, DataNode *new_data) {

    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = (*head_ref);
    new_node->prev = NULL;
    if ((*head_ref) != NULL)
        (*head_ref)->prev = new_node;
    (*head_ref) = new_node;
}
*/

/*
void insertLast(Node **head_ref, DataNode *new_data) {

    Node *new_node = (Node *) malloc(sizeof(Node));
    Node *last = *head_ref;
    new_node->data = new_data;
    new_node->next = NULL;
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL){
        last = last->next;
    }
    last->next = new_node;
    new_node->prev = last;

}
*/

/* Data Node Funcs */

Node *CreateFirstNode() {
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->currDataNode = NULL;
    return new_node;

}

Node *insertAfterNode(Node *curNode) {
    Node *new_node;

    if (curNode == NULL) {
        printError(ENullNode,0);
        return NULL;
    }

    new_node = (Node *) malloc(sizeof(Node));
    new_node->currDataNode = CreateFirstDataNode();
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL) {
        printError(EInsertionInMiddle, 0);
    }

    return curNode->next;
}

void clearListFromNode(Node *curNode) {
    Node *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        clearDataListFromNode(curNode->currDataNode);
        free(curNode);
        curNode = nextNode;
    }

}

/* Data Node Funcs */

DataNode *CreateFirstDataNode() {
    DataNode *new_node = (DataNode *) malloc(sizeof(DataNode));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;

}

DataNode *insertAfterDataNode(DataNode *curNode, Input redoInput, Input undoInput) {
    DataNode *new_node;

    if (curNode == NULL) {
        printError(ENullDataNode, 0);
        return NULL;
    }

    new_node = (DataNode *) malloc(sizeof(DataNode));
    new_node->redoInput = redoInput;
    new_node->undoInput = undoInput;
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL){
        printError(EInsertionInMiddle, 0);
    }

    return curNode->next;
}

DataNode *getFirstDataNode(DataNode *currDataNode) {
    if (currDataNode== NULL) {
        printError(ENullDataNode, 0);
        return NULL;
    }

    while (currDataNode->isFirst == false) {
        currDataNode = currDataNode->prev;
    }
    return currDataNode;
}

DataNode *getLastDataNode(DataNode *currDataNode) {
    if (currDataNode== NULL) {
        printError(ENullDataNode, 0);
        return NULL;
    }

    while (currDataNode->next != NULL) {
        currDataNode = currDataNode->next;
    }
    return currDataNode;
}

void clearDataListFromNode(DataNode *curNode) {

    DataNode *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }
}
