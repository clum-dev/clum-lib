#ifndef __FILE_H__
#define __FILE_H__

#define MAX_LINE_LENGTH 256

#include <stdbool.h>
#include <stdlib.h>

#include "strings.h"


typedef struct File {

    StringList* lines;
    size_t currentLine;

} File;


bool all_spaces(char* line);
FILE* open_file(char* filename, char* mode);
StringList* get_file_lines(FILE* file);
String* get_file_text(FILE* file);

String* dynamic_read(FILE* file, bool singleLine);

File* file_init();
void file_free(File* file);
File* file_read(char* path);
String* file_get_line(File* file, size_t index);
String* file_get_current_line(File* file);
bool file_has_next(File* file);
String* file_next(File* file);
void file_rewind(File* file);


#endif