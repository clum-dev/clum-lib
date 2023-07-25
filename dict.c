#include <stdio.h>
#include <stdlib.h>

#include "sha1.h"

#include "util.h"
#include "dict.h"
#include "list.h"

// Returns the next greatest prime from input
size_t nearest_prime(size_t input) {

    size_t primes[200] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 
                        41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 
                        97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 
                        149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 
                        197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 
                        257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 
                        313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 
                        379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 
                        439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 
                        499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 
                        571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 
                        631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 
                        691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 
                        761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 
                        829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 
                        907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 
                        977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 
                        1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 
                        1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 
                        1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223};

    for (size_t i = 0; i < 100; i++) {
        size_t prime = primes[i];
        if (prime >= input) {
            return prime;
        }
    }

    return primes[99];
}



// Hashing function from:
// https://stackoverflow.com/questions/21001659
unsigned int crc32b(const char* str) {
    unsigned int byte, crc, mask;
    int i = 0, j;
    crc = 0xFFFFFFFF;

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
    // DEPRECATED (using SHA1) - remove todo
    // size_t out = (size_t)crc32b(key->text);  

    SHA1_CTX sha;
    SHA1Init(&sha);
    uint8_t results[20];
    SHA1Update(&sha, (const unsigned char*)key->text, key->len);
    SHA1Final(results, &sha);

    size_t out = 0;
    for (size_t i = 0; i < 20; i++) {
        out += results[i];
    }
    return out % tableSize;
}

// Hash index from key (char*)
size_t gen_hash_text(size_t tableSize, char* text) {
    String* temp = str_init(text);
    size_t out = gen_hash(tableSize, temp);
    str_free(temp);
    return out;
}


//
HashNode* hashnode_init(String* key, Node* data) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    node->key = key;
    node->hashcode = 0;
    node->data = data;

    return node;
}

//
void hashnode_free(HashNode* node) {
    if (node != NULL) {
        if (node->data != NULL) {
            node_free(node->data);
            node->data = NULL;
        }
        if (node->key != NULL) {
            str_free(node->key);
            node->key = NULL;
        }
        node->hashcode = 0;
        free(node);
        node = NULL;
    }
}

//
void hashnode_print(HashNode* node) {
    printf("Hashnode: {%s, ", node->key->text);
    node_print_data(node->data, false);
    printf("}");
}

//
HashNode* hashnode_clone(HashNode* node) {
    HashNode* out = hashnode_init(str_clone(node->key), node_clone(node->data));
    out->hashcode = node->hashcode;
    return out;
}


// Initialises a dictnode struct
DictNode* dictnode_init(String* key, Node* data) {
    DictNode* node = (DictNode*)malloc(sizeof(DictNode));
    node->values = list_init();

    // // debug
    // String* type = node_get_type(data);
    // printf("dictnode init: type=%s\tdata:\n", type->text);
    // str_free(type);
    // node_print(data);

    Node* temp = node_init(NODE_HASHNODE, hashnode_init(key, data), false);
    temp->nodedata.hashnode->key = key;
    
    list_add(node->values, temp);
        
    return node;
}

// Frees a dictnode struct
void dictnode_free(DictNode* node) {
    if (node != NULL) {
        if (node->values != NULL) {
            list_free(node->values);
        }

        free(node);
        node = NULL;
    }
}

// Prints a dictnode struct
void dictnode_print(DictNode* node) {
    if (node->values->size == 0) {
        printf("{}\n");
        return;
    }
    printf("{%s, ", list_get(node->values, 0)->data->nodedata.hashnode->key->text);
    list_print_data(node->values, false);
    printf("}\n");
}


// Initialises a dict struct
Dict* dict_init(size_t max, bool usePrimes) {

    if (usePrimes) {
        max = nearest_prime(max);
    }

    Dict* dict = (Dict*)malloc(sizeof(Dict));
    dict->entries = (DictNode**)malloc(sizeof(DictNode*) * max);
    for (size_t i = 0; i < max; i++) {
        dict->entries[i] = NULL;
    }

    dict->max = max;
    dict->size = 0;

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
bool dict_add(Dict* dict, String* key, Node* data) {

    // Don't add if dict filled or if key already exists
    if (dict->size + 1 > dict->max || dict_contains(dict, key)) {
    // if (dict->size + 1 > dict->max) {

        String* msg = str_init("dict: cannot add key '");
        str_concat_str(msg, key);
        str_concat_text(msg, "' (dict is full or already contains key)");
        error_msg(E_WARN, -1, msg->text, false);
        str_free(msg);
        
        str_free(key);
        node_free(data);
        return false;
    }

    // Get hash (index)
    size_t index = gen_hash(dict->max, key);

    // Collision -> add to list
    if (dict->entries[index] != NULL) {

        // error_msg(E_DEBUG, -1, "dict: collision: adding to list", false);
        
        Node* temp = node_init(NODE_HASHNODE, hashnode_init(key, data), false);
        temp->nodedata.hashnode->key = key;
        
        list_add(dict->entries[index]->values, temp);


    // No collision -> add normally
    } else {

        // error_msg(E_DEBUG, -1, "dict: adding to enties normally", false);
        
        dict->size++;
        dict->entries[index] = dictnode_init(key, data);
    }

    return true;
}

// Checks if a key is in a dict
bool dict_contains(Dict* dict, String* key) {  
    size_t index = gen_hash_text(dict->max, key->text);
    return dict->entries[index] != NULL && dict->entries[index]->values != NULL
        && list_contains(dict->entries[index]->values, key->text);
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
ListNode* dict_get(Dict* dict, String* key) {
    ListNode* out = NULL;
    if (dict_contains(dict, key)) {
        out = list_find(dict->entries[gen_hash(dict->max, key)]->values, key->text);
    }
    return out;
}

// Gets the list of all keys in a dict
List* dict_get_keys(Dict* dict) {
    List* out = list_init();
    for (size_t i = 0; i < dict->max; i++) {
        if (dict->entries[i] != NULL) {
            for (size_t j = 0; j < dict->entries[i]->values->size; j++) {
                list_add_string(out, str_clone(
                    list_get(dict->entries[i]->values, j)->data->nodedata.hashnode->key));
            }
        }
    }

    return out;
}


// int main() {

//     Dict* dict = dict_init(2, false);

//     dict_add(dict, str_init("keytest"), node_init(NODE_VAR, var_from_int(1), false));
//     dict_add(dict, str_init("keytest"), node_init(NODE_VAR, var_from_int(123), false));

//     line_sep('-', 30);
//     List* keys = dict_get_keys(dict);
//     list_print(keys, false);
//     list_free(keys);

//     line_sep('-', 30);
//     dict_print(dict, false);
    
//     line_sep('-', 30);
//     printf("dict size: %ld\n", dict->size);
//     printf("dict entry 0 size: %ld\n", dict->entries[1]->values->size);
//     list_print(dict->entries[1]->values, true);

//     line_sep('-', 30);
//     String* key = str_init("keytest");
//     ListNode* test = dict_get(dict, key);
//     if (test != NULL) {
//         int idata = test->data->nodedata.hashnode->data->nodedata.var->data.i;
//         printf("data: %d\n", idata);
//     }

//     str_free(key);

//     dict_free(dict);

//     return 0;
// }