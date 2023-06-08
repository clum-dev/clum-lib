#ifndef __DICT_H__
#define __DICT_H__

#include "strings.h"
#include "node.h"

//
typedef struct DictNode {

    size_t hashcode;
    String* key;

    Node* data;

} DictNode;

//
typedef struct Dict {

    DictNode** entries;
    size_t size;
    size_t max;

} Dict;


#endif