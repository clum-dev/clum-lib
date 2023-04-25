#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "errors.h"

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
