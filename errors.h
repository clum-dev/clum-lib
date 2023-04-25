#ifndef __ERRORS_H__
#define __ERRORS_H__

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
void error_msg(ErrorLvl lvl, int code, char* msg, bool quit);


#endif