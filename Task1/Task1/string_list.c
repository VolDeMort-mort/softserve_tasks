#include "string_list.h"
#include <stdlib.h>
#include <string.h>

void string_list_init(char*** list){
	*list = malloc(sizeof(char*));
	if (*list == NULL) {
		exit(1);
	}
	(*list)[0] = NULL;
}

void string_list_destroy(char*** list){
	if (*list == NULL) {
		exit(1);
	}
	for (int i = 0; (*list)[i] != NULL; i++) {
		free((*list)[i]);
	}
	free((*list));
	*list = NULL;
}

void string_list_add(char*** list, char* string){
	int size = string_list_size(*list);

	*list = realloc(*list, (size + 2) * sizeof(char*));
	if (*list == NULL) {
		exit(1);
	}

	(*list)[size] = malloc(strlen(string) + 1);
	strcpy((*list)[size], string);

	(*list)[size + 1] = NULL;
}

void string_list_remove(char** list, char* string){
	int write = 0;
	for (int read = 0; list[read] != NULL; read++) {
		if (strcmp(list[read], string) == 0) {
			free(list[read]);
		}
		else {
			list[write] = list[read];
			write++;
		}
	}
	list[write] = NULL;
}

int string_list_size(char** list)
{
	int size = 0;
	if (list == NULL) {
		return 0;
	}
	for (int i = 0; list[i] != NULL; i++) {
		size++;
	}
	return size;
}

int string_list_index_of(char** list, char* string)
{
	if (list == NULL) {
		return -1;
	}
	for (int i = 0; list[i] != NULL; i++) {
		if (strcmp(list[i], string) == 0) {
			return i;
		}
	}
	return -1;
}


