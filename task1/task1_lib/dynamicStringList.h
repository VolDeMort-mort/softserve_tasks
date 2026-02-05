#pragma once
#include <stdlib.h>
#include <string.h>

#define MIN_CAP 16

using String = const char*;

/* Initializes list */
void StringListInit(char*** list, const size_t init_capacity = MIN_CAP);
/* Destroy list and set pointer to NULL. */
void StringListDestroy(char*** list);

/* Inserts value at the end of the list. */
void StringListAdd(char*** list, String str);
/* Removes all occurrences of str in the list. */
void StringListRemove(char*** list, String str);

/* Returns the number of items in the list. */
size_t StringListSize(char** list);
/* Returns the max number of items that can hold the list without reallocation*/
size_t StringListCapacity(char** list);
/* Returns the index position of the first occurrence of str in the list. */
size_t StringListIndexOf(char** list, const char* str);
	
/* Removes duplicate entries from the list. */
void StringListRemoveDuplicates(char*** list);
/* Replaces every occurrence of the before, in each of the string lists's strings, with after. */
void StringListReplaceInStrings(char** list, const char* before, const char* after);
/* Sorts the list of strings in ascending order */
void StringListSort(char** list);

