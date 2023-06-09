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

Dict* dict_init(size_t max);
void dict_free(Dict* dict);
void dict_print(Dict* dict, bool showNulls);

bool dict_add(Dict* dict, String* key, Node* data, bool linearProbe);
bool dict_contains(Dict* dict, String* key);
bool dict_contains_text(Dict* dict, char* key);
bool dict_remove(Dict* dict, String* key);

DictNode* dict_get(Dict* dict, String* key);


#endif