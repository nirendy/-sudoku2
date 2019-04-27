#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/* Linked list module - responsible for handling the linked-list data structure that holds all the moves history */

/* Data Node Funcs */
DataNode *CreateFirstDataNode() {
    DataNode *new_node = (DataNode *) smartMalloc(sizeof(DataNode));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

DataNode *insertAfterDataNode(DataNode *curNode, Input redoInput, Input undoInput) {
    DataNode *new_node;

    if (curNode == NULL) {
        printError(ENullDataNode);
        return NULL;
    }

    new_node = (DataNode *) smartMalloc(sizeof(DataNode));
    new_node->redoInput = redoInput;
    new_node->undoInput = undoInput;
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL) {
        printError(EInsertionInMiddle);
    }

    return curNode->next;
}

DataNode *getFirstDataNode(DataNode *currDataNode) {
    if (currDataNode == NULL) {
        printError(ENullDataNode);
        return NULL;
    }

    while (currDataNode->isFirst == false) {
        currDataNode = currDataNode->prev;
    }
    return currDataNode;
}

DataNode *getLastDataNode(DataNode *currDataNode) {
    if (currDataNode == NULL) {
        printError(ENullDataNode);
        return NULL;
    }

    while (currDataNode->next != NULL) {
        currDataNode = currDataNode->next;
    }
    return currDataNode;
}

void clearWholeDataList(DataNode *curNode) {
    DataNode *nextNode;
    curNode = getFirstDataNode(curNode);
    while (curNode != NULL) {
        nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }
}

/* Data Node Funcs */
Node *CreateFirstNode() {
    Node *new_node = (Node *) smartMalloc(sizeof(Node));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->currDataNode = NULL;
    return new_node;

}

Node *insertAfterNode(Node *curNode) {
    Node *new_node;

    if (curNode == NULL) {
        printError(ENullNode);
        return NULL;
    }

    new_node = (Node *) smartMalloc(sizeof(Node));
    new_node->currDataNode = CreateFirstDataNode();
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL) {
        printError(EInsertionInMiddle);
    }

    return curNode->next;
}

Node *getFirstNode(Node *curNode) {
    if (curNode == NULL) {
        printError(ENullNode);
        return NULL;
    }

    while (curNode->isFirst == false) {
        curNode = curNode->prev;
    }
    return curNode;

}

/**
 * clear the list starting from the node given
 * @param curNode
 */
void clearListFromNode(Node *curNode) {
    Node *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        if (!curNode->isFirst) {   /*first node has no data to free*/
            clearWholeDataList(curNode->currDataNode);
        }
        free(curNode);
        curNode = nextNode;
    }

}
