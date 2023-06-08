/**
 * TODO:
 *  -   dictionary (key value pair)
 *  -   
*/

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "dict.h"
#include "list.h"


// Hashing function from:
// https://stackoverflow.com/questions/21001659
unsigned int crc32b(const char* str) {
    unsigned int byte, crc, mask;
    int i = 0, j;
    crc = 0xFEED5EED;

    while (str[i] != 0) {
        byte = str[i];
        crc ^= byte;
        for (j = 7; j >= 0; j--) {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
}

// Hash index from key
size_t gen_hash(size_t tableSize, String* key) {
    size_t out = (size_t)crc32b(key->text);
    return out % tableSize;
}

// Hash index from key (char*)
size_t gen_hash_text(size_t tableSize, char* text) {
    String* temp = str_init(text);
    size_t out = gen_hash(tableSize, temp);
    str_free(temp);
    return out;
}


// Initialises a dictnode struct
DictNode* dictnode_init(String* key, Node* data) {

    DictNode* node = (DictNode*)malloc(sizeof(DictNode));
    node->key = key;
    node->data = data;

    node->hashcode = 0;

    return node;
}

// Frees a dictnode struct
void dictnode_free(DictNode* node) {

    if (node != NULL) {

        if (node->data != NULL) {
            node_free(node->data);
        }

        if (node->key != NULL) {
            str_free(node->key);
        }

        free(node);
        node = NULL;
    }
}

// Prints a dictnode struct
void dictnode_print(DictNode* node) {
    printf("{%s,\t", node->key->text);
    node_print_data(node->data, false);
    printf("}\n");
}

// Initialises a dict struct
Dict* dict_init(size_t max) {

    Dict* dict = (Dict*)malloc(sizeof(Dict));
    dict->entries = (DictNode**)malloc(sizeof(DictNode*) * max);
    for (size_t i = 0; i < max; i++) {
        dict->entries[i] = NULL;
    }

    dict->size = 0;
    dict->max = max;

    return dict;
}

// Frees a dict struct
void dict_free(Dict* dict) {
    if (dict != NULL) {
        for (size_t i = 0; i < dict->max; i++) {
            if (dict->entries[i] != NULL) {
                dictnode_free(dict->entries[i]);
            }
        }
        free(dict->entries);
        dict->entries = NULL;

        dict->size = 0;
        dict->max = 0;

        free(dict);
        dict = NULL;
    }
}

// Prints a dict struct
void dict_print(Dict* dict, bool showNulls) {
    printf("Dict (max: %ld\tsize: %ld):\n", dict->max, dict->size);
    for (size_t i = 0; i < dict->max; i++) {
        if (dict->entries[i] != NULL) {
            printf("%ld\t", i);
            dictnode_print(dict->entries[i]);
        } else if (showNulls) {
            printf("%ld\t", i);
            printf("NULL");
            printf("\n");
        }
    }
}

// Adds a new entry to a dict (returns false if key already exists)
bool dict_add(Dict* dict, String* key, Node* data, bool linearProbe) {

    // Don't add if dict filled
    if (dict->size + 1 >= dict->max) {
        str_free(key);
        node_free(data);
        return false;
    }

    // Linear probing (avoid collisions)
    size_t hash = gen_hash(dict->max, key);
    size_t index = hash;
    while (linearProbe) {
        if (index >= dict->max - 1) {
            index = 0;
        }
        if (dict->entries[index] != NULL) {
            index++;
        } else {
            break;;
        }
    }

    if (dict->entries[index] != NULL) {
        String* msg = str_init("Hashtable collision: '");
        str_concat_text(msg, key->text);
        str_concat_char(msg, '\'');
        str_concat_text(msg, "\t(Existing key: '");
        str_concat_text(msg, dict->entries[index]->key->text);
        str_concat_text(msg, "')");

        error_msg(E_WARN, -1, msg->text, false);
        str_free(msg);

        str_free(key);
        node_free(data);
        return false;

    } else {
        dict->entries[index] = dictnode_init(key, data);
        dict->entries[index]->hashcode = index;

        dict->size++;
    }

    return true;
}

// Checks if a key is in a dict
bool dict_contains(Dict* dict, String* key) {
    return dict->entries[gen_hash_text(dict->max, key->text)] != NULL;
}

// Checks if a key is in a dict (char*)
bool dict_contains_text(Dict* dict, char* key) {
    String* temp = str_init(key);
    bool out = dict_contains(dict, temp);
    str_free(temp);
    return out;
}

// Removes a dict entry (returns false if key cannot be found)
bool dict_remove(Dict* dict, String* key) {

    bool out = false;
    if (dict_contains(dict, key)) {
        size_t hash = gen_hash(dict->max, key);
        dictnode_free(dict->entries[hash]);
        dict->entries[hash] = NULL;
        out = true;
    }

    str_free(key);
    return out;
}

// Gets the dictnode for a given key (returns NULL if key cannot be found)
DictNode* dict_get(Dict* dict, String* key) {
    DictNode* out = NULL;
    if (dict_contains(dict, key)) {
        out = dict->entries[gen_hash(dict->max, key)];
    }
    return out;
}

