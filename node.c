#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "list.h"
#include "dict.h"

//
Node* node_init(NodeType type, void* nodedata, bool copy) {

    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    
    if (type == NODE_VAR) {
        if (copy) {
            node->nodedata.var = var_clone((Var*)nodedata);
        } else {
            node->nodedata.var = (Var*)nodedata;
        }
    
    } else if (type == NODE_ARR) {
        if (copy) {
            node->nodedata.arr = arr_clone((Arr*)nodedata);
        } else {
            node->nodedata.arr = (Arr*)nodedata;
        }

    } else if (type == NODE_LIST) {
        if (copy) {
            node->nodedata.list = list_clone((List*)nodedata);
        } else {
            node->nodedata.list = (List*)nodedata;
        }

    } else if (type == NODE_HASHNODE) {
        if (copy) {
            node->nodedata.hashnode = hashnode_clone((HashNode*)nodedata);
        } else {
            node->nodedata.hashnode = (HashNode*)nodedata;
        }
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

        } else if (node->type == NODE_LIST) {
            list_free(node->nodedata.list);
            node->nodedata.list = NULL;
            node->type = NODE_NULL;
        
        } else if (node->type == NODE_HASHNODE) {
            hashnode_free(node->nodedata.hashnode);
            node->nodedata.hashnode = NULL;
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
    } else if (node->type == NODE_LIST) {
        list_print(node->nodedata.list, false);
    } else if (node->type == NODE_HASHNODE) {
        hashnode_print(node->nodedata.hashnode);
    }
}

//
void node_print_data(Node* node, bool newline) {
    if (node->type == NODE_NULL) {
        printf("(null node)");
        if (newline) {
            printf("\n");
        }
    } else if (node->type == NODE_VAR) {
        var_print_data(node->nodedata.var, newline);
    } else if (node->type == NODE_ARR) {
        arr_print_data(node->nodedata.arr);
        if (newline) {
            printf("\n");
        }
    } else if (node->type == NODE_LIST) {
        list_print_data(node->nodedata.list, newline);
    } else if (node->type == NODE_HASHNODE) {
        hashnode_print(node->nodedata.hashnode);
    }

}

//
Node* node_clone(Node* node) {

    Node* out = (Node*)malloc(sizeof(Node));
    out->type = node->type;

    if (node->type == NODE_VAR) {
        out->nodedata.var = var_clone(node->nodedata.var);
    } else if (node->type == NODE_ARR) {
        out->nodedata.arr = arr_clone(node->nodedata.arr);
    } else if (node->type == NODE_LIST) {
        out->nodedata.list = list_clone(node->nodedata.list);
    }

    return out;
}

//
String* node_get_type(Node* node) {
    switch (node->type) {
        case NODE_NULL:
            return str_init("NODE_NULL");
        case NODE_VAR:
            return str_init("NODE_VAR");
        case NODE_ARR:
            return str_init("NODE_ARR");
        case NODE_LIST:
            return str_init("NODE_LIST");

        default:
            return str_init(NULL);
    }
}