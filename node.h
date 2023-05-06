#ifndef __DATANODE_H__
#define __DATANODE_H__

#include "arr.h"


//
typedef enum NodeType {

    NODE_NULL,
    NODE_VAR,
    NODE_ARR,

} NodeType;

//
typedef union NodeData {
    
    Var* var;
    Arr* arr;
    
} NodeData;

//
typedef struct Node {

    NodeData nodedata;
    NodeType type;

} Node;


Node* node_init(NodeType type, void* data);
void node_free(Node* node);
void node_print(Node* node);
void node_print_data(Node* node);



#endif