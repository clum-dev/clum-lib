#ifndef __ARR_H__
#define __ARR_H__

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
void var_set_data(Var* var, String* strval, bool eval);
void var_set_data_text(Var* var, char* text, bool eval);

String* get_type_str(VarType type);
VarType get_data_type(String* strval);



#endif