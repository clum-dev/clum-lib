#ifndef __ERRORS_H__
#define __ERRORS_H__

#include "strings.h"

//
typedef enum ErrorLvl {
    
    E_DEBUG,
    E_INFO,
    E_WARN,
    E_ERROR,
    E_FATAL,

} ErrorLvl;


void unimp(char* msg);
void line_sep(char c, size_t amount);
void debug_indent(size_t tabs);
void debug_newline(bool show);
void error_msg(ErrorLvl lvl, int code, char* msg, bool quit);

String* prompt(char* msg);
bool accept_prompt(char* msg, char* accept);


#endif