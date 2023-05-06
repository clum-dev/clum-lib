#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "file.h"

//
const char* ERROR_LVL[5] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

// Debug - unimplemented message
void unimp(char* msg) {
    fprintf(stderr, "[UNIMP]\t%s\n", msg);
}

// Debug - line of chars (to separate printouts)
void line_sep(char c, size_t amount) {
    for (size_t i = 0; i < amount; i++) {
        printf("%c", c);
    }
    printf("\n");
}

//
void error_msg(ErrorLvl lvl, int code, char* msg, bool quit) {
    fprintf(stderr, "[%s]\t\t%s\t", ERROR_LVL[lvl], msg);
    if (lvl == E_ERROR || lvl == E_FATAL) {
        fprintf(stderr, "(code: %d)", code);
    }
    fprintf(stderr, "\n");
    if (quit) {
        exit(code);
    }
}

// Prompts a user with a message and records their input as a string
String* prompt(char* msg) {
    printf("%s\n> ", msg);

    String* temp = dynamic_read(stdin, true);
    fflush(stdin);

    return temp;
}

// Prompts a user with a messange, records their input, 
// and checks against the given accept string
// Returns true if it matches
bool accept_prompt(char* msg, char* accept) {

    bool out = false;
    String* temp = prompt(msg);

    if (!strcmp(temp->text, accept)) {
        out = true;
    }
    str_free(temp);

    return out;
}

