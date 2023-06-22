#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "util.h"

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
        debug_newline(sep);
        head = head->next;
    }
}

// Prints only the data in a list struct
void list_print_data(List* list, bool sep) {
    
    printf("{");
    debug_newline(sep);

    ListNode* head = list->head;
    size_t index = 0;
    while (head != NULL) {
        if (sep) {
            debug_indent(1);
        }
        node_print_data(head->data, false);
        if (index != list->size - 1) {
            printf(", ");
        }
        debug_newline(sep);

        
        head = head->next;
        index++;
    }
    
    printf("}");
    debug_newline(sep);

}

// Adds a node to a list struct
void list_add(List* list, Node* data) {

    ListNode* head = list->head;
    ListNode* node = NULL;
    node = listnode_init(data);
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

// Checks whether a list contains a node with the given name
bool list_contains(List* list, char* name) {
    if (list_index(list, name) == -1) {
        return false;
    }
    return true;
}

// Clones a list struct
List* list_clone(List* list) {

    List* out = list_init();
    ListNode* head = list->head;
    while (head != NULL) {
        // list_add(out, head->data);
        list_add(out, node_clone(head->data));
        head = head->next;
    }

    return out;
}

// TODO
List* list_zip(List* a, List* b) {

    List* out = list_init();
    
    for (size_t i = 0; i < a->size; i++) {
        List* temp = list_init();
        ListNode* aItem = list_get(a, i);
        ListNode* bItem = list_get(b, i);

        if (aItem != NULL) {
            list_add(temp, node_clone(aItem->data));
        }
        if (bItem != NULL) {
            list_add(temp, node_clone(bItem->data));
        }

        if (temp->size == 2) {
            list_add_list(out, list_clone(temp));
        }
        list_free(temp);
    }

    return out;
}





// Adds a var to a list
void list_add_var(List* list, Var* var) {
    list_add(list, node_init(NODE_VAR, var, false));
}

// Adds an arr to a list
void list_add_arr(List* list, Arr* arr) {
    list_add(list, node_init(NODE_ARR, arr, false));
}

// Adds a list to a list
void list_add_list(List* list, List* add) {
    list_add(list, node_init(NODE_LIST, add, false));
}


// Adds an int var to a list
void list_add_int(List* list, int i) {
    list_add(list, node_init(NODE_VAR, var_from_int(i), false));
}

// Adds a float var to a list
void list_add_float(List* list, float f) {
    list_add(list, node_init(NODE_VAR, var_from_float(f), false));
}

// Adds a string var to a list
void list_add_string(List* list, String* str) {
    list_add(list, node_init(NODE_VAR, var_from_string(str), false));
}

// Adds a string var (from text) to a list
void list_add_text(List* list, char* txt) {
    String* temp = str_init(txt);
    list_add_string(list, temp);
}

// Adds a char var to a list
void list_add_char(List* list, char c) {
    list_add(list, node_init(NODE_VAR, var_from_char(c), false));
}

// Adds a bool var to a list
void list_add_bool(List* list, bool b) {
    list_add(list, node_init(NODE_VAR, var_from_bool(b), false));
}


// Gets the correct value to be summed
Var* get_sum_val(ListNode* head, bool strict) {
    if (head->data->type == NODE_VAR) {
        return var_clone(head->data->nodedata.var);
    } else if (head->data->type == NODE_LIST) {
        return list_sum(head->data->nodedata.list, strict);;
    } else if (head->data->type == NODE_ARR) {
        unimp("get sum val - arr TODO");
    }

    return NULL;
}

// Summation of list elements
Var* list_sum(List* input, bool strict) {

    Var* total = var_init(NULL, T_NULL);

    ListNode* head = input->head;
    VarType type = -1;
    bool typeConsistent = true;

    // Check list is all vars of same type
    while (head != NULL) {
        
        // Don't add together sub lists/arrs
        if (head->data->type != NODE_VAR) {
            // typeConsistent = false;
            // break;
        } else if (strict) {
            if (type == -1) {
                type = head->data->nodedata.var->type;
            } else if (type != head->data->nodedata.var->type) {
                typeConsistent = false;
                break;
            }
        }
        head = head->next;
    }

    // Concatenate or add
    if (typeConsistent) {
                
        ListNode* head = input->head;
        int index = 0;

        while (head != NULL) {
            // Set initial total to first value in list
            if (index == 0) {
                Var* sumval = get_sum_val(head, strict);
                // var_set_data_text(total, sumval->strval->text, true);
                var_set_data(total, sumval->strval, true);
                var_free(sumval);

            // Add total with next value in list
            } else {
                Var* sumval = get_sum_val(head, strict);
                Var* add = var_sum(total, sumval);
                var_set_data(total, add->strval, true);
                var_free(add);
                var_free(sumval);
            }

            head = head->next;
            index++;
        }
    
    } else {
        error_msg(E_ERROR, -1, "Invalid sum types - todo", false);
    }

    return total;
}