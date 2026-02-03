#ifndef STRING_LIST_H
#define STRING_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

void StringListInit(char*** list);
void StringListDestroy(char*** list);

void StringListAdd(char*** list, const char* str);
void StringListRemove(char*** list, const char* str);

int StringListSize(char** list);
int StringListIndexOf(char** list, const char* str);

void StringListRemoveDuplicates(char*** list);
void StringListReplaceInStrings(char** list, const char* before, const char* after);
void StringListSort(char** list);

#ifdef __cplusplus
}
#endif

#endif
