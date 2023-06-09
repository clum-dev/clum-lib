#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "file.h"


// Attempts to open a file with a given filename
FILE* open_file(char* filename, char* mode) {
    
    FILE* file;
	// file = fopen(filename, "r");
	file = fopen(filename, mode);
	if (file == NULL) {
		fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
		exit(1);
	}

    return file;
}

// Returs true if a line is all whitespace
bool all_spaces(char* line) {

	int spaceCount = 0;
	int len = (int)strlen(line);

	// Count all whitespace chars (including tab)
	for (int i = 0; i < len; i++) {
		if (isspace(line[i])) {
			spaceCount++;
		}
	}

	// printf("len: %d\tspaces: %d\n", len, spaceCount);

	if (spaceCount == len) {
		return true;
	}

	return false;
}

// Gets a stringlist representation of the lines of a file
StringList* get_file_lines(FILE* file) {

	String* raw = dynamic_read(file, false);
	StringList* out;

	// Return new list if empty
	if (raw->len == 0) {
		out = strlist_init();
	} else {
	 	out = str_split(raw, "\n");
	}

	str_free(raw);
	return out;
}


// Dynamically reads a given file pointer
// If singleLine is true, then only one line is read
String* dynamic_read(FILE* file, bool singleLine) {
    
	String* str = str_init(NULL);
	char ch;
	size_t index = 0;

	while ((ch = fgetc(file)) != EOF) {
		if (ch == '\n' && singleLine) {
			break;
		}

		if (index == 0) {
			char temp[1];
			temp[0] = ch;
			str_set(str, temp);			
		} else {
			str_concat_char(str, ch);
		}

		index++;
	}

	return str;
}