#include <stdio.h>
#include <stdlib.h>

#include "arr.h"
#include "errors.h"

// Debug for tracking changes made to this library
void arr_lib_version() {
    float version = ARR_LIB_VERSION;
    printf("ArrLib\tVersion %f\n", version);
}

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

// Gets the vartype from a given string
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
        fprintf(stderr, "Error: invalid bool conversion: '%s'\n", strval->text);
        exit(1);
    }

}

// Initialises a var struct
Var* var_init(char* text, VarType type) {

    Var* var = (Var*)malloc(sizeof(Var));
    var->type = type;
    var->typeStr = get_type_str(var->type);
    var->strval = str_init(text);
    var->data.s = NULL;
    
    String* temp = str_init(var->strval->text);
    var_set_data(var, temp, false);
    str_free(temp);

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
        }

        free(var);
        var = NULL;
    }
}

// Prints a var struct
void var_print(Var* var) {
    printf("var: '%s'\ttype: %s\n", var->strval->text, var->typeStr->text);
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
            str_free(var->data.s);
        }   
    }
}

// Stores and evaluates the new string val
void var_evaluate(Var* var, String* new) {
    // str_clear(var->strval);
    // str_concat_text(var->strval, new->text);
    str_free(var->strval);
    var->strval = str_init(new->text);
        
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
    printf("arr: (%s) (size %ld)\n", temp->text, arr->size);
    for (size_t i = 0; i < arr->size; i++) {
        var_print(arr->data[i]);
    }
    str_free(temp);
}

// Adds a var to an array
void arr_add(Arr* arr, Var* var, bool matchType) {

    // Don't add if not correct type
    if (arr->type != T_ANY && matchType && arr->type != var->type) {
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

    String* out;
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


// Adds int value to array
void arr_add_int(Arr* arr, int i) {
    char temp[INT_STR_SIZE];
    sprintf(temp, "%d", i);
    arr_add(arr, var_init(temp, T_INT), false);
}

// Adds float value to array
void arr_add_float(Arr* arr, float f) {
    char temp[INT_STR_SIZE];
    sprintf(temp, "%f", f);
    arr_add(arr, var_init(temp, T_FLOAT), false);
}

// Adds strings value to array
void arr_add_string(Arr* arr, String* str) {
    arr_add(arr, var_init(str->text, T_STRING), false);
    str_free(str);
}

// Adds char value to array
void arr_add_char(Arr* arr, char c) {
    String* temp = str_init("");
    str_concat_char(temp, c);
    arr_add(arr, var_init(temp->text, T_CHAR), false);
    str_free(temp);
}

// Adds bool value to array
void arr_add_bool(Arr* arr, bool b) {
    String* temp;
    if (b) {
        temp = str_init("true");
    } else {
        temp = str_init("false");
    }
    arr_add(arr, var_init(temp->text, T_BOOL), false);
    str_free(temp);
}


int main() {

    Arr* temp = arr_init(T_ANY);

    for (int i = 0; i < 5; i++) {
        arr_add_int(temp, i);
    }

    printf("temp:\n");
    arr_print(temp);
    line_sep('-', 30);

    String* str = arr_join(temp, ',');
    printf("str: %s\n", str->text);
    str_free(str);

    arr_free(temp);
}