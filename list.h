#ifndef __LIST_H__
#define __LIST_H__

#include "arr.h"
#include "node.h"

//
typedef struct ListNode {

    Node* data;
    struct ListNode* next;
    struct ListNode* prev;

} ListNode;

//
struct List {

    ListNode* head;
    size_t size;

};


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
List* list_clone(List* list);
List* list_zip(List* a, List* b);

void list_add_var(List* list, Var* var);
void list_add_arr(List* list, Arr* arr);
void list_add_list(List* list, List* add);

void list_add_int(List* list, int i);
void list_add_float(List* list, float f);
void list_add_string(List* list, String* str);
void list_add_text(List* list, char* txt);
void list_add_char(List* list, char c);
void list_add_bool(List* list, bool b);

Var* list_sum(List* input, bool strict);


#endif