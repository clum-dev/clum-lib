#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "errors.h"

//
ListNode* listnode_init(Node* data) {

    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

//
void listnode_free(ListNode* node) {

    if (node != NULL) {
        node_free(node->data);
        node->data = NULL;

        node->prev = NULL;
        node->next = NULL;

        free(node);
        node = NULL;
    }
}

//
void listnode_print(ListNode* node, bool showNeighbours) {

    if (node != NULL) {
        node_print(node->data);
    } else {
        printf("(null)\n");
        return;
    }

    if (showNeighbours) {

        printf("Prev: ");
        if (node->prev != NULL) {
            node_print(node->prev->data);
        } 
        
        printf("Next: ");
        if (node->next != NULL) {
            node_print(node->next->data);
        } 
    }

}

//
List* list_init() {

    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;

    return list;
}

// 
void list_free(List* list) {
    
    if (list != NULL) {

        ListNode* head = list->head;
        while (head != NULL) {
            
            ListNode* temp = head;
            head = head->next;
            
            if (temp != NULL) {
                listnode_free(temp);
            }
        }

        list->size = 0;
        
        free(list);
        list = NULL;
    }
}

//
void list_print(List* list) {
    
    printf("List: (size = %ld)\n", list->size);

    ListNode* head = list->head;
    while (head != NULL) {
        node_print(head->data);
        head = head->next;
    }
}

//
void list_add(List* list, Node* data) {

    ListNode* head = list->head;
    ListNode* node = listnode_init(data);
    list->size++;

    // add head
    if (head == NULL) {
        list->head = node;
        list->head->prev = NULL;
        list->head->next = NULL;

    // add tail
    } else {
        while (head->next != NULL) {
            head = head->next;
        }
        head->next = node;
        node->prev = head;
    }

}


//
int main() {

    List* list = list_init();
    // list_print(list);
    // line_sep('-', 30);

    list_add(list, node_init(NODE_VAR, var_init("1234", T_STRING)));

    
    Arr* arr = arr_init(T_ANY);
    arr_add_string(arr, str_init("sneed"));
    arr_add_int(arr, 123);
    
    list_add(list, node_init(NODE_ARR, arr));

    printf("%s\n", list->head->next->data->nodedata.arr->data[0]->strval->text);


    // list_print(list);
    line_sep('-', 30);

    list_free(list);

    return 0;
}