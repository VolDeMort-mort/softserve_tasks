#pragma once

void string_list_init(char*** list);
void string_list_destroy(char*** list);
void string_list_add(char*** list, char* string);
void string_list_remove(char** list, char* string);
int string_list_size(char** list);
int string_list_index_of(char** list, char* string);