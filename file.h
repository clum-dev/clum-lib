#ifndef __FILE_H__
#define __FILE_H__

#define MAX_LINE_LENGTH 256

#include <stdbool.h>
#include <stdlib.h>

#include "strings.h"


bool all_spaces(char* line);
FILE* open_file(char* filename, char* mode);
StringList* get_file_lines(FILE* file);

String* dynamic_read(FILE* file, bool singleLine);


#endif