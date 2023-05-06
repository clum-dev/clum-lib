#include <stdio.h>
#include <stdlib.h>

#include "node.h"

//
Node* node_init(NodeType type, void* nodedata) {

    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    
    if (type == NODE_VAR) {
        node->nodedata.var = var_clone((Var*)nodedata);

    } else if (type == NODE_ARR) {
        node->nodedata.arr = arr_clone((Arr*)nodedata);
    }
    
    return node;
}

//
void node_free(Node* node) {

    if (node != NULL) {

        if (node->type == NODE_VAR) {
            var_free(node->nodedata.var);
            node->nodedata.var = NULL;
            node->type = NODE_NULL;

        } else if (node->type == NODE_ARR) {
            arr_free(node->nodedata.arr);
            node->nodedata.arr = NULL;
            node->type = NODE_NULL;
        }

        free(node);
        node = NULL;

    }
}

//
void node_print(Node* node) {
    if (node->type == NODE_NULL) {
        printf("(null node)\n");
    } else if (node->type == NODE_VAR) {
        var_print(node->nodedata.var);
    } else if (node->type == NODE_ARR) {
        arr_print(node->nodedata.arr);
    }
}

//
void node_print_data(Node* node) {
    if (node->type == NODE_NULL) {
        printf("(null node)\n");
    } else if (node->type == NODE_VAR) {
        var_print_data(node->nodedata.var);
    } else if (node->type == NODE_ARR) {
        arr_print_data(node->nodedata.arr);
    }
}

