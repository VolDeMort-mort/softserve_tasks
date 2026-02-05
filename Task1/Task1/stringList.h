#pragma once

typedef char** stringList;

void printList(stringList list);

void StringListInit(stringList* list);
void StringListDestroy(stringList* list);
int stringListGetSize(const stringList list);
void StringListAdd(stringList& list, const char* str);
void StringListRemove(stringList list, char* str);
int StringListIndexOf(stringList list, const char* str);

void stringListRemoveAt(stringList list, int index);

void StringListRemoveDuplicates(stringList list);

void StringListReplaceInStrings(stringList list, const char* before, const char* after);

void StringListSort(stringList list);