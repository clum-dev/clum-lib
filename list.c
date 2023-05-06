#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "errors.h"

// Initialises a listnode struct
ListNode* listnode_init(Node* data) {

    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

// Frees a listnode struct
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

// Prints a listnode struct
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

// Clones a listnode struct
ListNode* listnode_clone(ListNode* node) {

    ListNode* new = listnode_init(node->data);
    new->prev = node->prev;
    new->next = node->next;

    return new;
}

// Initialises a list struct
List* list_init() {

    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;

    return list;
}

// Frees a list struct
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

// Prints a list struct
void list_print(List* list, bool sep) {
    
    printf("List: (size = %ld)\n", list->size);

    ListNode* head = list->head;
    while (head != NULL) {
        node_print(head->data);
        if (sep) {
            printf("\n");
        }
        head = head->next;
    }
}

// Prints only the data in a list struct
void list_print_data(List* list, bool sep) {
    
    printf("List: (size = %ld)\n", list->size);

    ListNode* head = list->head;
    while (head != NULL) {
        node_print_data(head->data);
        if (sep) {
            printf("\n");
        }
        head = head->next;
    }
}

// Adds a node to a list struct
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

// Removes a node at a given index from a list
void list_remove(List* list, size_t index) {

    if (index > list->size) {
        return;
    }

    ListNode* head = list->head;
    int i = 0;
    while (head != NULL) {
        if (i == index) {

            // remove head
            if (head->prev == NULL) {
                list->head = head->next;
                if (list->head != NULL) {
                    list->head->prev = NULL;
                }
            
            // otherwise redirect pointers
            } else {
                head->prev->next = head->next;
            }
            
            // otherwise redirect pointers
            if (head->next != NULL) {
                head->next->prev = head->prev;
            }

            listnode_free(head);
            list->size--;
            break;
        }

        head = head->next;
        i++;
    }
}

// Gets the node at a given index from a list
ListNode* list_get(List* list, size_t index) {

    ListNode* head = list->head;
    size_t i = 0;
    while (head != NULL) {
        if (i == index) {
            return head;
        }
        head = head->next;
        i++;
    }
    
    return NULL;
}

// Gets the index of a node with a given name from a list
// (returns -1 if not found)
int list_index(List* list, char* name) {
    ListNode* head = list->head;
    int i = 0;
    while (head != NULL) {
        if (str_equals_text(head->data->nodedata.var->strval, name, true)) {
            return i;        
        }
        head = head->next;
        i++;
    }
    return -1;
}

// Gets the node from a list matching the given name
// (returns null if not found)
ListNode* list_find(List* list, char* name) {

    ListNode* head = list->head;
    while (head != NULL) {
        if (str_equals_text(head->data->nodedata.var->strval, name, true)) {
            return head;        
        }
        head = head->next;
    }
    
    return NULL;
}

// Searches and removes a node in a list
void list_pluck(List* list, char* name) {
    ListNode* head = list->head;
    size_t i = 0;
    while (head != NULL) {
        if (str_equals_text(head->data->nodedata.var->strval, name, true)) {
            list_remove(list, i);
            break;
        }
        head = head->next;
        i++;
    }
}

// CHecks whether a list contains a node with the given name
bool list_contains(List* list, char* name) {
    if (list_index(list, name) == -1) {
        return false;
    }
    return true;
}

