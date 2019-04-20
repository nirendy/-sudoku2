
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


void insertAfter(struct Node *curNode, struct DataNode *new_data) {
    struct Node *new_node;

    if (curNode == NULL) { printf("the given previous node cannot be NULL"); return; }

    new_node = (struct Node *) malloc(sizeof(struct Node));
    new_node->data = new_data;
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
        freeData(&(curNode->data));
        free(curNode);
        curNode = nextNode;
    }

}

void clearWholeList(struct Node **head_ref) {
    struct Node *curNode = *head_ref;
    clearListFromNode(curNode);
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

struct DataNode ** CreateNewDataNode(Input input){
    struct DataNode *new_node = (struct DataNode *) malloc(sizeof(struct DataNode));
    struct DataNode **head_data_ref = &new_node;

    new_node->input = input;
    return head_data_ref;

}

struct Node ** CreateNewNode( struct DataNode* data){
    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
    struct Node **head_ref = &new_node;
    new_node->data = data;
    return head_ref;

}
