#include "string_list.h"
#include <stdlib.h>
#include <string.h>

void string_list_init(char*** list) {
	if (list == nullptr) {
		return;
	}

	char** tmp = static_cast<char**>(malloc(sizeof(char*)));
	if (!tmp) {
		return;
	}
	*list = tmp;

	(*list)[0] = nullptr;
}

void string_list_destroy(char*** list) {
	if (list == nullptr || *list == nullptr) {
		return;
	}

	for (int i = 0; (*list)[i] != nullptr; i++) {
		free((*list)[i]);
	}
	free((*list));
	*list = nullptr;
}

void string_list_add(char*** list, const char* string) {
	if (list == nullptr || *list == nullptr || string == nullptr) {
		return;
	}
	
	int size = string_list_size(*list);

	char** tmp = static_cast<char**>(realloc(*list, (size + 2) * sizeof(char*)));
	if (!tmp) {
		return;
	}
	*list = tmp;

	(*list)[size] = static_cast<char*>(malloc(strlen(string) + 1));
	strcpy((*list)[size], string);

	(*list)[size + 1] = nullptr;
}

void string_list_remove(char*** list, const char* string) {
	if (list == nullptr || *list == nullptr || string == nullptr) {
		return;
	}

	int write = 0;
	for (int read = 0; (*list)[read] != nullptr; read++) {
		if (strcmp((*list)[read], string) == 0) {
			free((*list)[read]);
		}
		else {
			(*list)[write] = (*list)[read];
			write++;
		}
	}
	char** tmp = static_cast<char**>(realloc(*list, (write + 1) * sizeof(char*)));
	if (!tmp) {
		return;
	}
	*list = tmp;
	(*list)[write] = nullptr;
}

int string_list_size(char** list)
{
	int size = 0;
	if (list == nullptr) {
		return 0;
	}
	for (int i = 0; list[i] != nullptr; i++) {
		size++;
	}
	return size;
}

int string_list_index_of(char** list, const char* string)
{
	if (list == nullptr || string == nullptr) {
		return -1;
	}

	for (int i = 0; list[i] != nullptr; i++) {
		if (strcmp(list[i], string) == 0) {
			return i;
		}
	}
	return -1;
}

void string_list_remove_duplicates(char*** list){
	if (list == nullptr || *list == nullptr) {
		return;
	}

	for (int i = 0; (*list)[i] != nullptr; i++) {
		for (int j = i + 1; (*list)[j] != nullptr; ) {
			if (strcmp((*list)[i], (*list)[j]) == 0) {
				free((*list)[j]);
				for (int k = j; (*list)[k] != nullptr; k++) {
					(*list)[k] = (*list)[k+1];
				}
				continue;
			}
			j++;
		}
	}

	int size = string_list_size(*list);
	char** tmp = static_cast<char**>(realloc(*list, (size + 1) * sizeof(char*)));
	if (!tmp) {
		return;
	}
	*list = tmp;
}

void string_list_replace_in_strings(char** list, const char* before, const char* after){
	if (list == nullptr || before == nullptr || after == nullptr) {
		return;
	}

	for (int i = 0; list[i] != nullptr; i++) {
		if (strcmp(list[i], before) == 0) {
			free(list[i]);
			list[i] = static_cast<char*>(malloc(strlen(after) + 1));
			strcpy(list[i], after);
		}
	}
}

void string_list_sort(char** list){
	if (list == nullptr) {
		return;
	}

	int size = 0;
	size = string_list_size(list);
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			if (strcmp(list[j], list[j + 1]) > 0) {
				char* tmp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = tmp;
			}
		}
	}
}
