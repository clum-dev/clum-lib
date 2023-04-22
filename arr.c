/**
 * TODO:
 *  -   like stringlist but for any data type
*/

#include <stdio.h>
#include <stdlib.h>

#include "arr.h"

//
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

//
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

//
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

//
Var* var_init(char* text, VarType type) {

    Var* var = (Var*)malloc(sizeof(Var));
    var->type = type;
    var->typeStr = get_type_str(var->type);
    var->strval = str_init(text);
    
    String* temp = str_init(var->strval->text);
    var_set_data(var, temp, false);
    str_free(temp);

    return var;
}

//
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

//
void var_print(Var* var) {
    printf("var: '%s'\ttype: %s\n", var->strval->text, var->typeStr->text);
}

//
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

//
void var_set_data(Var* var, String* strval, bool eval) {
    
    if (eval) {
        var_evaluate(var, strval);
    }

    switch (var->type) {
        case T_ANY:
            // var_evaluate(var, strval);
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

//
void var_set_data_text(Var* var, char* text, bool eval) {
    String* temp = str_init(text);
    var_set_data(var, temp, eval);
    str_free(temp);
}

//
Var* var_clone(Var* var) {
    Var* out = var_init(var->strval->text, var->type);
    return out;
}



//
bool arr_in_bounds(Arr* arr, size_t index) {
    if (index < 0 || index >= arr->size) {
        return false;
    }
    return true;
}

//
Arr* arr_init(VarType type) {
    
    Arr* arr = (Arr*)malloc(sizeof(Arr));
    arr->size = 0;
    arr->data = NULL;
    arr->type = type;

    return arr;

}

//
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

//
void arr_print(Arr* arr) {
    printf("arr: (size %ld)\n", arr->size);
    for (size_t i = 0; i < arr->size; i++) {
        var_print(arr->data[i]);
    }
}

//
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

//
int arr_index(Arr* arr, char* name) {
    for (size_t i = 0; i < arr->size; i++) {
        if (str_equals_text(arr->data[i]->strval, name, true)) {
            return (int)i;
        }
    }
    return -1;
}

//
bool arr_contains(Arr* arr, char* name) {
    return arr_index(arr, name) != -1;
}

//
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

//
Arr* arr_pluck(Arr* arr, char* name) {
    int index = arr_index(arr, name);
    if (index != -1) {
        return arr_remove(arr, (size_t)index);
    }
    return arr;
}

//
void arr_set(Arr* arr, size_t index, char* data, bool eval) {
    if (!arr_in_bounds(arr, index)) {
        return;
    }

    var_set_data_text(arr->data[index], data, eval);
}

Arr* arr_from_strlist(StringList* strlist) {

    Arr* out = arr_init(T_STRING);

    for (size_t i = 0; i < strlist->size; i++) {
        arr_add(out, var_init(strlist->strings[i]->text, T_STRING), false);
    }

    return out;
}


// //
// int main(int argc, char** argv) {
    
//     StringList* list = strlist_init();
//     strlist_add(list, str_init("hello"));
//     strlist_add(list, str_init("sneed"));
//     strlist_add(list, str_init("and chuck"));
//     strlist_print(list);

//     printf("---------------------\n");
    
//     Arr* arr = arr_from_strlist(list);    
//     arr_print(arr);
//     printf("---------------------\n");

//     arr = arr_pluck(arr, "sneed");
//     arr_print(arr);
//     printf("---------------------\n");


//     strlist_free(list);
//     arr_free(arr);

//     return 0;
// }