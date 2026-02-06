#pragma once

#include <stdlib.h>
#include <string.h>

#pragma warning (disable:4996)


void SetMetadata(char** list, int capacity, int size);

void StringListInit(char*** list);

void StringListDestroy(char*** list);

void StringListAdd(char*** list, char* str);

void StringListRemoveDuplicates(char** list);

void StringListReplaceInStrings(char** list, char* before, char* after);

int StringListSize(char** list);

int StringListIndexOf(char** list, char* str);

void StringListRemove(char** list, char* str);

void StringListSort(char** list);