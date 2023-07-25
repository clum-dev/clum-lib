#ifndef __DATANODE_H__
#define __DATANODE_H__

#include "arr.h"

typedef struct List List;
typedef struct HashNode HashNode;

//
typedef enum NodeType {

    NODE_NULL,
    NODE_VAR,
    NODE_ARR,
    NODE_LIST,
    NODE_HASHNODE,

} NodeType;

//
typedef union NodeData {
    
    Var* var;
    Arr* arr;
    List* list;
    HashNode* hashnode;
    
} NodeData;

//
typedef struct Node {

    NodeData nodedata;
    NodeType type;

} Node;


Node* node_init(NodeType type, void* data, bool copy);
void node_free(Node* node);
void node_print(Node* node);
void node_print_data(Node* node, bool newline);
Node* node_clone(Node* node);

String* node_get_type(Node* node);

#endif