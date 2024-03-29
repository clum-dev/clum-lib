#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arr.h"
#include "util.h"


// Gets the string representation of a vartype
String* get_type_str(VarType type) {
    switch (type) {
        case T_ANY:
            return str_init("T_ANY");
        case T_INT:
            return str_init("T_INT");
        case T_FLOAT:
            return str_init("T_FLOAT");
        case T_STRING:
            return str_init("T_STRING");
        case T_CHAR:
            return str_init("T_CHAR");
        case T_BOOL:
            return str_init("T_BOOL");
        case T_NULL:
            return str_init("T_NULL");
        default:
            fprintf(stderr, "Error: unhandled type %d\n", type);
    }

    return str_init("T_NULL");
}

// Infers the vartype from a given string
VarType get_data_type(String* strval) {
    
    if (strval == NULL || strval->text == NULL || strval->len == 0) {
        return T_NULL;
    }

	int ignore_i;
	float ignore_f;
	char c;

	VarType type;

	// Check int
	if ((sscanf(strval->text, "%d %c", &ignore_i, &c)) == 1) {
		type = T_INT;
	
	// Check float
	} else if ((sscanf(strval->text, "%f %c", &ignore_f, &c)) == 1) { 
		type = T_FLOAT;

	// Check string
	} else if (strval->len > 1) {
		type = T_STRING;

	// Check char
	} else if (strval->len == 1) {
		type = T_CHAR;

	// Unknown data type
	} else {
		fprintf(stderr, "Error: unknown type for var '%s'\n", strval->text);
        type = T_NULL;
	}

    if (type == T_STRING && (str_equals_text(strval, "true", true) || 
            str_equals_text(strval, "false", true))) {
        type = T_BOOL;
    }

	return type;
}

// Gets the boolean version of a string
// ("true" or "false")
bool get_bool(String* strval) {

    if (str_equals_text(strval, "true", true)) {
        return true;
    } else if (str_equals_text(strval, "false", true)) {
        return false;
    } else {
        String* msg = str_init("Invalid bool conversion from: '");
        str_concat_str(msg, strval);
        str_concat_char(msg, '\'');
        error_msg(E_ERROR, -1, msg->text, true);
    }

    return false;
}

// Initialises a var struct
Var* var_init(char* text, VarType type) {

    Var* var = (Var*)malloc(sizeof(Var));
    var->type = type;
    var->typeStr = get_type_str(var->type);
    var->strval = str_init(text);
    var->data.s = NULL;
    // var->data.s = str_init(NULL);
    
    // String* strval = str_init(var->strval->text);
    var_set_data(var, var->strval, false);
    // if (var->type != T_STRING) {
    //     str_free(strval);
    // }

    return var;
}

// Frees a var struct
void var_free(Var* var) {

    if (var != NULL) {
        if (var->strval != NULL) {
            str_free(var->strval);
        }

        if (var->typeStr != NULL) {
            str_free(var->typeStr);
        }
      
        if (var->type == T_STRING && var->data.s != NULL) {
            str_free(var->data.s);
            var->data.s = NULL;
        }

        free(var);
        var = NULL;
    }
}

// Prints a var struct
void var_print(Var* var) {
    printf("Var: '%s'\ttype: %s\n", var->strval->text, var->typeStr->text);
}

// Prints only the data in a var struct
void var_print_data(Var* var, bool newline) {
    printf("%s", var->strval->text);
    if (newline) {
        printf("\n");
    }
}

// Clears all info in a var struct (does not free)
void var_clear(Var* var) {
    if (var != NULL) {
        if (var->strval != NULL) {
            str_free(var->strval);
        }

        if (var->typeStr != NULL) {
            str_free(var->typeStr);
        }

        if (var->type == T_STRING && var->data.s != NULL) {
            printf(">>>freeing data string: %s\n", var->data.s->text);
            str_free(var->data.s);
        }   
    }
}

// Set a var from another var
void var_set_var(Var* dest, Var* src) {
    
    if (dest->type == T_STRING) {
        if (dest->data.s != NULL) {
            str_free(dest->data.s);
            dest->data.s = NULL;
        }
    }
    
    str_set(dest->strval, src->strval->text);
    str_set(dest->typeStr, src->typeStr->text);
    dest->type = src->type;

    // Copy union data
    switch (src->type) {
        case T_INT:
            dest->data.i = src->data.i;
            break;
        case T_FLOAT:
            dest->data.f = src->data.f;
            break;
        case T_STRING:
            dest->data.s = str_init(src->data.s->text);
            if (src->data.s != NULL) {
                str_free(src->data.s);
                src->data.s = NULL;
            }
            break;
        case T_CHAR:
            dest->data.c = src->data.c;
            break;
        case T_BOOL:
            dest->data.b = src->data.c;
            break;
        case T_NULL:
            dest->data.n = src->data.n;
            break;

        default:
            fprintf(stderr, "Error: unhandled type %d\n", src->type);
    }
}

// Stores and evaluates the new string val
void var_evaluate(Var* var, String* newVar) {
    str_free(var->strval);
    var->strval = str_init(newVar->text);
        
    var->type = get_data_type(var->strval);
    str_free(var->typeStr);
    var->typeStr = get_type_str(var->type);
}

// Sets variable data from a given string (if eval, update var type)
void var_set_data(Var* var, String* strval, bool eval) {
    
    // Free current stored string
    if (var->type == T_STRING) {
        if (var->data.s != NULL) {
            str_free(var->data.s);
            var->data.s = NULL;
        }
    }

    // Evaluate type
    if (eval) {
        var_evaluate(var, strval);
    }

    switch (var->type) {
        case T_ANY:
            var_set_data(var, strval, true);
            break;
        case T_INT:
            var->data.i = atoi(strval->text);
            break;
        case T_FLOAT:
            var->data.f = atof(strval->text);
            break;
        case T_STRING:
            var->data.s = str_init(strval->text);
            break;
        case T_CHAR:
            var->data.c = strval->text[0];
            break;
        case T_BOOL:
            var->data.b = get_bool(strval);
            break;
        case T_NULL:
            var->data.n = NULL;
            break;

        default:
            fprintf(stderr, "Error: unhandled type %d\n", var->type);

    }

}

// Sets variable data (from char*)
void var_set_data_text(Var* var, char* text, bool eval) {
    String* temp = str_init(text);
    var_set_data(var, temp, eval);
    str_free(temp);
}

// Clones a variable struct
Var* var_clone(Var* var) {
    Var* out = var_init(var->strval->text, var->type);
    return out;
}


// MOVE THESE OPERATIONS TO THEIR OWN FILE??


// Variable summation: a + b
Var* var_add(Var* a, Var* b) {

    Var* out = var_init(NULL, T_STRING);
    VarType aType = a->type;
    VarType bType = b->type;

    // If one var is a string, concatenate
    if (aType == T_STRING || bType == T_STRING) {
        String* cat = str_init(a->strval->text);
        str_concat_text(cat, b->strval->text);
        var_set_data(out, cat, false);
        str_free(cat);
    
    // Sum ints
    } else if (aType == T_INT && bType == T_INT) {
        Var* temp = var_from_int(a->data.i + b->data.i);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Sum floats
    } else if (aType == T_FLOAT && bType == T_FLOAT) {
        Var* temp = var_from_float(a->data.f + b->data.f);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Sum chars
    } else if (aType == T_CHAR && bType == T_CHAR) {
        Var* temp = var_from_char(a->data.c + b->data.c);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Invalid sum op
    } else {
        error_msg(E_ERROR, -1, "Invalid summation opearation - todo", false);
    }
    
    return out;
}

// Variable subtraction: a - b
Var* var_sub(Var* a, Var* b) {

    Var* out = var_init(NULL, T_STRING);
    VarType aType = a->type;
    VarType bType = b->type;

    // If a and b are strings, subtract (whole) substrings of b from a
    if (aType == T_STRING && bType == T_STRING) {
        // String* found = str_init("");
        // for (size_t i = 0; i < a->data.s->len; i++) {
        //     // If out of bounds, append rest of chars
        //     if (i + b->data.s->len <= a->data.s->len) {
        //         String* slice = str_slice(a->data.s, i, i + b->data.s->len - 1);

        //         // If matched, move index forward (skip substring)
        //         if (str_equals(slice, b->data.s, false)) {
        //             i += b->data.s->len;
        //         }
        //         str_free(slice);
        //     }
        //     str_concat_char(found, a->data.s->text[i]);
        // }

        // (moved above code to str_reap)
        // TODO TEST THIS
        String* found = str_clone(a->data.s);
        str_reap(found, b->data.s);

        Var* temp = var_from_string(found);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // If a is string and b is int, shorten a by b chars
    } else if (aType == T_STRING && bType == T_INT) {
        String* slice = str_slice(a->data.s, 0, -b->data.i);
        Var* temp = var_from_string(slice);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Sub ints
    } else if (aType == T_INT && bType == T_INT) {
        Var* temp = var_from_int(a->data.i - b->data.i);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Sub floats  
    } else if (aType == T_FLOAT && bType == T_FLOAT) {
        Var* temp = var_from_float(a->data.f - b->data.f);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Sub chars
    } else if (aType == T_CHAR && bType == T_CHAR) {
        Var* temp = var_from_char(a->data.c - b->data.c);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Invalid sub op
    } else {
        error_msg(E_ERROR, -1, "Invalid subtraction operation - todo", false);
    }

    return out;
}

// Variable multiplication: a * b
Var* var_mult(Var* a, Var* b) {
    
    Var* out = var_init(NULL, T_STRING);
    VarType aType = a->type;
    VarType bType = b->type;

    // If a is string and b is int, repeat a (b times)
    if (aType == T_STRING && bType == T_INT) {
        if (b->data.i <= 0) {
            error_msg(E_ERROR, -1, "Invalid string multiplication - todo", false);
        } else {
            String* temp = str_init("");
            for (size_t i = 0; i < b->data.i; i++) {
                str_concat_str(temp, a->data.s);
            }
            var_set_data(out, temp, true);
            str_free(temp);
        }

    // Mult ints
    } else if (aType == T_INT && bType == T_INT) {
        Var* temp = var_from_int(a->data.i * b->data.i);
        var_set_data(out, temp->strval, true);
        var_free(temp);
    
    // Mult floats
    } else if (aType == T_FLOAT && bType == T_FLOAT) {
        Var* temp = var_from_float(a->data.f * b->data.f);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Mult float/int combo
    } else if ((aType == T_INT || aType == T_FLOAT) && (bType == T_INT || bType == T_FLOAT)) {
        Var* temp;
        if (a->type == T_INT && b->type == T_FLOAT) {
            temp = var_from_float(a->data.i * b->data.f);
        } else if (a->type == T_FLOAT && bType == T_INT) {
            temp = var_from_float(a->data.f * b->data.i);
        }
        var_set_data(out, temp->strval, true);
        var_free(temp);     

    // Invalid mult op
    } else {
        error_msg(E_ERROR, -1, "Invalid multilication operation - todo", false);
    }

    return out;
}

// Variable division: a / b
Var* var_div(Var* a, Var* b) {

    Var* out = var_init(NULL, T_STRING);
    VarType aType = a->type;
    VarType bType = b->type;

    // Div ints
    if (aType == T_INT && bType == T_INT) {
        Var* temp = var_from_int(a->data.i / b->data.i);
        var_set_data(out, temp->strval, true);
        var_free(temp);
    
    // Div floats
    } else if (aType == T_FLOAT && bType == T_FLOAT) {
        Var* temp = var_from_float(a->data.f / b->data.f);
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Div float/int combo
    } else if ((aType == T_INT || aType == T_FLOAT) && (bType == T_INT || bType == T_FLOAT)) {
        Var* temp;
        if (a->type == T_INT && bType == T_FLOAT) {
            temp = var_from_float(a->data.i / b->data.f);
        } else if (a->type == T_FLOAT && bType == T_INT) {
            temp = var_from_float(a->data.f / b->data.f);
        }
        var_set_data(out, temp->strval, true);
        var_free(temp);

    // Invlid div op
    } else {
        error_msg(E_ERROR, -1, "Invalid division operation - todo", false);
    }

    return out;
}

// Variable modulo: a % b
Var* var_mod(Var* a, Var* b) {

    Var* out = var_init(NULL, T_INT);
    VarType aType = a->type;
    VarType bType = b->type;

    // Modulo only on ints
    if (aType == T_INT && bType == T_INT) {
        Var* temp = var_from_int(a->data.i % b->data.i);
        var_set_data(out, temp->strval, true);
        var_free(temp);
    } else {
        error_msg(E_ERROR, -1, "Invalid modulo operation - todo", false);
    }

    return out;
}

// Variable power: a ** b
Var* var_pow(Var* a, Var* b) {
    unimp("var power op");
    return NULL;
}

// Cast a var to a given type
void var_cast(Var* var, VarType type) {
    if (var->type == type) {
        return;
    }

    Var* out = NULL;
    int temp_i = 0;
    bool temp_b = false; 

    switch (type) {
        case T_ANY:
            break;
        case T_INT:
            if (var->type == T_BOOL && var->data.b) {
                temp_i = 1;
            } else {
                temp_i = atoi(var->strval->text);
            }
            out = var_from_int(temp_i);
            break;
        case T_FLOAT:
            out = var_from_float(atof(var->strval->text));
            break;
        case T_STRING:
            out = var_from_string(str_init(var->strval->text));
            break;
        case T_CHAR:
            out = var_from_char(var->strval->text[0]);
            break;
        case T_BOOL:
            if (var->type == T_INT) {
                temp_b = (var->data.i != 0);
            } else {
                temp_b = get_bool(var->strval);
            }
            out = var_from_bool(temp_b);
            break;
        case T_NULL:
            out = var_init(NULL, T_NULL);
            printf("TESTING NULL CAST\n");
            break;

        default:
            fprintf(stderr, "Error: unhandled type: %d\n", type);
    }

    if (out == NULL) {
        var_init(NULL, T_NULL);
    }

    var_set_var(var, out);
    var_free(out);
    out = NULL;
}


// Checks array bounds
bool arr_in_bounds(Arr* arr, size_t index) {
    if (index < 0 || index >= arr->size) {
        error_msg(E_ERROR, 0, "Array access out of bounds", false);
        return false;
    }
    return true;
}


// Initialises an array struct
Arr* arr_init(VarType type) {
    
    Arr* arr = (Arr*)malloc(sizeof(Arr));
    arr->size = 0;
    arr->data = NULL;
    arr->type = type;

    return arr;

}

// Frees an array struct
void arr_free(Arr* arr) {

    if (arr != NULL) {
        if (arr->data != NULL) {
            for (size_t i = 0; i < arr->size; i++) {
                var_free(arr->data[i]);
            }
            free(arr->data);
            arr->data = NULL;
        }

        arr->size = 0;
        free(arr);
        arr = NULL;
    }

}

// Clears all info in a arr struct (does not free)
void arr_clear(Arr* arr) {
    if (arr != NULL) {
        if (arr->data != NULL) {
            for (size_t i = 0; i < arr->size; i++) {
                var_free(arr->data[i]);
            }
            free(arr->data);
            arr->data = NULL;
        }

        arr->size = 0;
    }
}

// Prints an array struct
void arr_print(Arr* arr) {
    String* temp = get_type_str(arr->type);
    printf("Arr: (%s) (size %ld)\n", temp->text, arr->size);
    for (size_t i = 0; i < arr->size; i++) {
        var_print(arr->data[i]);
    }
    str_free(temp);
}

// Prints only the data in an array struct
void arr_print_data(Arr* arr) {
    String* temp = get_type_str(arr->type);
    printf("[");
    for (size_t i = 0; i < arr->size; i++) {
        var_print_data(arr->data[i], false);
        if (i != arr->size - 1) {
            printf(", ");
        }
    }
    printf("]");
    str_free(temp);
}

// Adds a var to an array
void arr_add(Arr* arr, Var* var, bool matchType) {

    // Don't add if not correct type
    if (arr->type != T_ANY && matchType && arr->type != var->type) {
        String* msg = str_init("Cannot add var of type '");
        str_concat_str(msg, var->typeStr);
        str_concat_text(msg, "' to arr of type '");
        String* arrType = get_type_str(arr->type);
        str_concat_str(msg, arrType);
        str_concat_char(msg, '\'');
        error_msg(E_WARN, -1, msg->text, false);
        str_free(arrType);
        str_free(msg);
        
        var_free(var);
        return;
    }

    if (arr->size == 0) {
        arr->data = (Var**)malloc(sizeof(Var*) * ++arr->size);
    } else {
        arr->data = (Var**)realloc(arr->data, sizeof(Var*) * ++arr->size);
    }

    arr->data[arr->size - 1] = var;
}

// Gets the index of a var in an array (returns -1 if not found)
int arr_index(Arr* arr, char* name, bool caseSensitive) {
    for (size_t i = 0; i < arr->size; i++) {
        if (str_equals_text(arr->data[i]->strval, name, caseSensitive)) {
            return (int)i;
        }
    }
    return -1;
}

// Checks whether a var exists in an array
bool arr_contains(Arr* arr, char* name, bool caseSensitive) {
    return arr_index(arr, name, caseSensitive) != -1;
}

// Removes a var at a given index in an array
Arr* arr_remove(Arr* arr, size_t index) {

    if (!arr_in_bounds(arr, index)) {
        return arr;
    }
    
    Arr* out = arr_init(arr->type);
    out->type = arr->type;

    for (size_t i = 0; i < arr->size; i++) {
        if (i != index) {
            arr_add(out, var_clone(arr->data[i]), false);
        }
    }

    arr_free(arr);
    return out;

}

// Searches and removes a var in an array
Arr* arr_pluck(Arr* arr, char* name) {
    int index = arr_index(arr, name, true);
    if (index != -1) {
        return arr_remove(arr, (size_t)index);
    }
    return arr;
}

// Sets the variable data at a given index in an array
void arr_set(Arr* arr, size_t index, char* data, bool eval) {
    if (!arr_in_bounds(arr, index)) {
        return;
    }

    var_set_data_text(arr->data[index], data, eval);
}

// Clones an array struct
Arr* arr_clone(Arr* arr) {
    Arr* out = arr_init(arr->type);

    for (size_t i = 0; i < arr->size; i++) {
        arr_add(out, var_clone(arr->data[i]), false);
    }
    
    return out;
}

// Joins two arrays
Arr* arr_cat(Arr* a1, Arr* a2) {
    
    Arr* out = arr_init(T_ANY);

    for (size_t i = 0; i < a1->size; i++) {
        arr_add(out, var_clone(a1->data[i]), false);
    }

    for (size_t i = 0; i < a2->size; i++) {
        arr_add(out, var_clone(a2->data[i]), false);
    }

    return out;
}

// Creates an array from a given string list
Arr* arr_from_strlist(StringList* strlist) {

    Arr* out = arr_init(T_STRING);

    for (size_t i = 0; i < strlist->size; i++) {
        arr_add(out, var_init(strlist->strings[i]->text, T_STRING), false);
    }

    return out;
}

// Returns a new subarray of the original array
Arr* arr_slice(Arr* arr, size_t lower, size_t upper) {

    if (!arr_in_bounds(arr, lower) || !arr_in_bounds(arr, lower)) {
        return arr;
    }

    Arr* out = arr_init(arr->type);

    for (size_t i = lower; i <= upper; i++) {
        arr_add(out, var_clone(arr->data[i]), false);
    }

    return out;
}

// Reverses the elements of an array
void arr_reverse(Arr* arr) {
    Arr* temp = arr_clone(arr);
    for (size_t i = temp->size; i > 0; i--) {
        arr_set(arr, arr->size - i, temp->data[i - 1]->strval->text, true);
    }
    arr_free(temp);
}

// Joins an array (by given separators) into a single string
String* arr_join(Arr* arr, char sep) {

    String* out = str_init("");
    for (size_t i = 0; i < arr->size; i++) {
        if (i == 0) {
            out = str_init(arr->data[i]->strval->text);
        } else {
            str_concat_text(out, arr->data[i]->strval->text);
        }
        
        if (sep != '\0' && i != arr->size - 1) {
            str_concat_char(out, sep);
        }
    }

    return out;
}


// Initialises a var from an int
Var* var_from_int(int i) {
    char temp[INT_STR_SIZE];    // TODO redo with dynamic input???
    sprintf(temp, "%d", i);
    return var_init(temp, T_INT);
}

// Initialises a var from a float
Var* var_from_float(float f) {
    char temp[INT_STR_SIZE];
    sprintf(temp, "%f", f);
    return var_init(temp, T_FLOAT);
}

// Initialises a var from a string
Var* var_from_string(String* str) {
    Var* out = var_init(str->text, T_STRING);
    str_free(str);
    str = NULL;
    return out;
}

// Initialises a var from text
Var* var_from_text(char* txt) {
    String* temp = str_init(txt);
    Var* out = var_from_string(temp);
    return out;
}

// Initialises a var from a char
Var* var_from_char(char c) {
    String* temp = str_init("");
    str_concat_char(temp, c);
    Var* out = var_init(temp->text, T_CHAR);
    str_free(temp);
    return out;
}

// Initialises a var from a bool
Var* var_from_bool(bool b) {
    String* temp;
    if (b) {
        temp = str_init("true");
    } else {
        temp = str_init("false");
    }
    Var* out = var_init(temp->text, T_BOOL);
    str_free(temp);
    return out;
}


// Adds int value to array
void arr_add_int(Arr* arr, int i) {
    arr_add(arr, var_from_int(i), false);
}

// Adds float value to array
void arr_add_float(Arr* arr, float f) {
    arr_add(arr, var_from_float(f), false);
}

// Adds string value to array
void arr_add_string(Arr* arr, String* str) {
    arr_add(arr, var_from_string(str), false);
}

// Adds text value to array (as string)
void arr_add_text(Arr* arr, char* txt) {
    String* temp = str_init(txt);
    arr_add_string(arr, temp);
}

// Adds char value to array
void arr_add_char(Arr* arr, char c) {
    arr_add(arr, var_from_char(c), false);
}

// Adds bool value to array
void arr_add_bool(Arr* arr, bool b) {
    arr_add(arr, var_from_bool(b), false);
}

