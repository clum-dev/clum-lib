#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct String {
    
    char* text;
    size_t len;

} String;

//
typedef struct StringList {
    
    String** strings;
    size_t size;

} StringList;


String* str_init(char* text);
void str_free(String* str);
void str_print(String* str, bool newline);
void str_set(String* str, char* text);
void str_clear(String* str);
void str_concat_text(String* str, char* text);
void str_concat_char(String* str, char chr);
void str_concat_str(String* str, String* text);
String* str_to_lower(String* str);
String* str_to_upper(String* str);
bool str_equals(String* str1, String* str2, bool caseSensitive);
bool str_equals_text(String* str1, char* str2, bool caseSensitive);
String* str_clone(String* str);
void str_reverse(String* str);
String* str_slice(String* str, int start, int end);
void str_trim(String* str, int chars);
void str_reap(String* str, String* match);
size_t str_count(String* str, String* match);

StringList* strlist_init();
void strlist_free(StringList* list);
void strlist_print(StringList* list);

void strlist_add(StringList* list, String* text);
StringList* str_split(String* src, char* delims);
String* strlist_join(StringList* list, char separator);
StringList* strlist_clone(StringList* src);
bool strlist_contains(StringList* list, String* str, bool caseSensitive);


#endif