
#include "linked_list.h"

//void insertFirst(struct Node **head_ref, struct DataNode *new_data) {
//
//    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
//    new_node->data = new_data;
//    new_node->next = (*head_ref);
//    new_node->prev = NULL;
//    if ((*head_ref) != NULL)
//        (*head_ref)->prev = new_node;
//    (*head_ref) = new_node;
//}

//void insertLast(struct Node **head_ref, struct DataNode *new_data) {
//
//    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
//    struct Node *last = *head_ref;
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


void insertAfterNode(struct Node *curNode, struct DataNode *new_data) {
    struct Node *new_node;

    if (curNode == NULL) {
        printf("the given previous node cannot be NULL");
        return;
    }

    new_node = (struct Node *) malloc(sizeof(struct Node));
    new_node->currDataNode = new_data;
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;
}

void clearListFromNode(struct Node *curNode) {
    struct Node *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        freeData(&(curNode->currDataNode));
        free(curNode);
        curNode = nextNode;
    }

}

void freeData(struct DataNode **head_data_ref) {

    struct DataNode *curNode = *head_data_ref;
    struct DataNode *nextNode;
    while (curNode != NULL) {
        nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }
}

struct DataNode *CreateFirstDataNode() {
    struct DataNode *new_node = (struct DataNode *) malloc(sizeof(struct DataNode));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;

}

struct Node *CreateFirstNode() {
    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
    new_node->isFirst = true;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->currDataNode = NULL;
    return new_node;

}

void insertAfterDataNode(struct DataNode *curNode, Input redoInput, Input undoInput) {
    struct DataNode *new_node;

    if (curNode == NULL) {
        printf("the given previous node cannot be NULL");
        return;
    }

    new_node = (struct DataNode *) malloc(sizeof(struct DataNode));
    new_node->redoInput = redoInput;
    new_node->undoInput = undoInput;
    new_node->isFirst = false;
    new_node->next = curNode->next;
    curNode->next = new_node;
    new_node->prev = curNode;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;
}

struct DataNode *getLastDataNode(struct DataNode *currDataNode) {
    while (currDataNode->next != NULL) {
        currDataNode = currDataNode->next;
    }
    return currDataNode;
}

struct DataNode *getFirstDataNode(struct DataNode *currDataNode) {
    while (currDataNode->isFirst == false) {
        currDataNode = currDataNode->prev;
    }
    return currDataNode;
}
