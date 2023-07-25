#ifndef __DICT_H__
#define __DICT_H__

#include "list.h"
#include "strings.h"
#include "node.h"

//
struct HashNode {
    
    /**
     * TODO: add node_type for node data
    */
    
    size_t hashcode;
    String* key;
    Node* data;
};

//
typedef struct DictNode {
    List* values;
} DictNode;

//
typedef struct Dict {

    DictNode** entries;
    size_t size;
    size_t max;

} Dict;


HashNode* hashnode_init(String* key, Node* data);
void hashnode_free(HashNode* node);
void hashnode_print(HashNode* node);
HashNode* hashnode_clone(HashNode* node);

Dict* dict_init(size_t max, bool usePrimes);
void dict_free(Dict* dict);
void dict_print(Dict* dict, bool showNulls);

bool dict_add(Dict* dict, String* key, Node* data);
bool dict_contains(Dict* dict, String* key);
bool dict_contains_text(Dict* dict, char* key);
bool dict_remove(Dict* dict, String* key);

ListNode* dict_get(Dict* dict, String* key);



size_t gen_hash(size_t tableSize, String* key);

#endif