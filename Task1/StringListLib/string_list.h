#pragma once

void string_list_init(char*** list);
void string_list_destroy(char*** list);
void string_list_add(char*** list, const char* string);
void string_list_remove(char*** list, const char* string);
int string_list_size(char** list);
int string_list_index_of(char** list, const char* string);
void string_list_remove_duplicates(char*** list);
void string_list_replace_in_strings(char** list, const char* before, const char* after);
void string_list_sort(char** list);
