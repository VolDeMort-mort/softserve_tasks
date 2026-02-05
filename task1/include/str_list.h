#pragma once
#include <iostream>
typedef const char* string;


void list_init(char*** list);
void string_add(char** list, string str);
void string_remove(char** list, string str);
int return_index_str(char** list, string str);
void remove_duplicates(char** list);
void replace_string(char** list, const char* before, const char* after);
void sort_list(char** list);
void remove_list(char*** list);
int comparer(const void* a, const void* b);
size_t get_capacity(char** list);
size_t get_size(char** list);