#pragma once

#include <stdlib.h>
#include <string.h>

void StringListInit(char*** list);

void StringListDestroy(char*** list);

void StringListAdd(char*** list, char* str);

void StringListRemove(char** list, char* str);

int StringListSize(char** list);

int StringListIndexOf(char** list, char* str);

void StringListRemoveDuplicates(char** list);

void StringListReplaceInStrings(char** list, char* one, char* anotherone);

void StringListSort(char** list);
