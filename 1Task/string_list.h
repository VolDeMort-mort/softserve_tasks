#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <stdlib.h>
#include <string.h>


void ListCreate(char**& list);

void ListDestroy(char**& list);

void ListAdd(char**& list, const char* str);

void ListEraseValue(char** list, const char* str);

int& ListSize(char** list);

int ListFindIndex(char** list, const char* str);

void ListRemoveDups(char** list);

void ListSort(char** list);

void StringListReplaceInStrings(char** list, const char* before, const char* after);


#endif
