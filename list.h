#ifndef __LIST_H__
#define __LIST_H__

#include "node.h"

//
typedef struct ListNode {

    Node* data;
    struct ListNode* next;
    struct ListNode* prev;

} ListNode;

//
typedef struct List {

    ListNode* head;
    size_t size;

} List;


ListNode* listnode_init(Node* data);
void listnode_free(ListNode* node);
void listnode_print(ListNode* node, bool showNeighbours);
List* list_init();
void list_free(List* list);
void list_print(List* list, bool sep);
void list_print_data(List* list, bool sep);
void list_add(List* list, Node* data);
void list_remove(List* list, size_t index);
ListNode* list_get(List* list, size_t index);
int list_index(List* list, char* name);
ListNode* list_find(List* list, char* name);
void list_pluck(List* list, char* name);
bool list_contains(List* list, char* name);


#endif