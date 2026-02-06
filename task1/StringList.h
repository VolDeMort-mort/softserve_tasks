#ifndef STRINGLIST_H
#define STRINGLIST_H

#ifdef __cplusplus
extern "C" {
#endif

void stringListInit(char*** list);
void stringListDestroy(char*** list);
void stringListAdd(char*** list, const char* str);
void stringListRemove(char*** list, const char* str);
int stringListSize(char** list);
int stringListIndexOf(char** list, const char* str);

void stringListRemoveDuplicates(char*** list);
void stringListReplaceInStrings(char** list, const char* before, const char* after);
void stringListSort(char** list);

#ifdef __cplusplus
}
#endif

#endif // STRINGLIST_H
