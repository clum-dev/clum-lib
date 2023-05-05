#ifndef __ARR_H__
#define __ARR_H__

#define ARR_LIB_VERSION 0.1
#define INT_STR_SIZE 12

#include <stdlib.h>

#include "strings.h"

//
typedef enum VarType {

    T_ANY,
    T_INT,
    T_FLOAT,
    T_STRING,
    T_CHAR,
    T_BOOL,
    T_NULL,

} VarType;

//
typedef union Data {
    
    int i;
    float f;
    String* s;
    char c;
    bool b;
    void* n;

} Data;

//
typedef struct Var {

    VarType type;
    String* typeStr;

    Data data;
    String* strval;

} Var;

//
typedef struct Arr {

    VarType type;
    
    size_t size;
    Var** data;
    
} Arr;


Var* var_init(char* text, VarType type);
void var_free(Var* var);
void var_print(Var* var);
void var_clear(Var* var);
void var_evaluate(Var* var, String* newVar);
void var_set_data(Var* var, String* strval, bool eval);
void var_set_data_text(Var* var, char* text, bool eval);
Var* var_clone(Var* var);

String* get_type_str(VarType type);
VarType get_data_type(String* strval);

Arr* arr_init(VarType type);
void arr_free(Arr* arr);
void arr_clear(Arr* arr);
void arr_print(Arr* arr);
void arr_add(Arr* arr, Var* var, bool matchType);
int arr_index(Arr* arr, char* name, bool caseSensitive);
bool arr_contains(Arr* arr, char* name, bool caseSensitive);
Arr* arr_remove(Arr* arr, size_t index);
Arr* arr_pluck(Arr* arr, char* name);
void arr_set(Arr* arr, size_t index, char* data, bool eval);
Arr* arr_clone(Arr* arr);
Arr* arr_cat(Arr* a1, Arr* a2);
Arr* arr_from_strlist(StringList* strlist);
Arr* arr_slice(Arr* arr, size_t lower, size_t upper);
void arr_reverse(Arr* arr);
String* arr_join(Arr* arr, char sep);

void arr_add_int(Arr* arr, int i);
void arr_add_float(Arr* arr, float f);
void arr_add_string(Arr* arr, String* str);
void arr_add_char(Arr* arr, char c);
void arr_add_bool(Arr* arr, bool b);



#endif