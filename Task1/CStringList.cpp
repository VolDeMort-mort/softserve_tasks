#define _CRT_SECURE_NO_WARNINGS
#include "CStringList.h"

static const int CSTRINGLIST_INITIAL_CAPACITY = 1;
static const int CSTRINGLIST_INITIAL_SIZE = 0;
static const int CSTRINGLIST_STORE_SIZE_INDEX = 0;
static const int CSTRINGLIST_STORE_CAPACITY_INDEX = 1;
static const int CSTRINGLIST_RESERVED_BLOCK_AMMOUNT = 2;

#define CSTRING_SIZE(list) (*((int*)(list) - 2))
#define CSTRING_CAPACITY(list) (*((int*)(list) - 1))

void StringListInit(char*** list) {
	if (!list) return;
	int* block = (int*)malloc(sizeof(int) * 2 + sizeof(char*) * CSTRINGLIST_INITIAL_CAPACITY);
	if (!block) return;
	block[CSTRINGLIST_STORE_SIZE_INDEX] = CSTRINGLIST_INITIAL_SIZE;
	block[CSTRINGLIST_STORE_CAPACITY_INDEX] = CSTRINGLIST_INITIAL_CAPACITY;
	*list = (char**)(block + CSTRINGLIST_RESERVED_BLOCK_AMMOUNT);
}

void StringListDestroy(char*** list) {
	if (!list || !*list) return;
	int size = CSTRING_SIZE(*list);
	for (int i = 0; i < size; i++) {
		free((*list)[i]);
	}
	free((int*)(*list) - CSTRINGLIST_RESERVED_BLOCK_AMMOUNT);
	*list = NULL;
}

void StringListAdd(char*** list, char* str) {
	if (!list || !*list || !str) return;
	int size = CSTRING_SIZE(*list);
	int capacity = CSTRING_CAPACITY(*list);

	if (size >= capacity) {
		int newCapacity = capacity * 2;
		int* block = (int*)realloc((int*)(*list) - CSTRINGLIST_RESERVED_BLOCK_AMMOUNT,
			sizeof(int) * CSTRINGLIST_RESERVED_BLOCK_AMMOUNT + sizeof(char*) * newCapacity);
		if (!block) return;

		*list = (char**)(block + CSTRINGLIST_RESERVED_BLOCK_AMMOUNT);
		CSTRING_CAPACITY(*list) = newCapacity;
	}

	char* newStr = (char*)malloc(strlen(str) + 1);
	if (!newStr) return;
	strcpy(newStr, str);

	(*list)[size] = newStr;
	CSTRING_SIZE(*list)++;
}

void StringListRemove(char** list, char* str) {
	if (!list || !str) return;
	int size = CSTRING_SIZE(list);
	int j = 0;
	for (int i = 0; i < size; i++) {
		if (strcmp(list[i], str) == 0) {
			free(list[i]);
		}
		else {
			list[j++] = list[i];
		}
	}
	CSTRING_SIZE(list) = j;
}

int StringListSize(char** list) {
	if (!list) return 0;
	return CSTRING_SIZE(list);
}

int StringListIndexOf(char** list, char* str) {
	if (!list || !str) return -1;
	int size = CSTRING_SIZE(list);
	for (int i = 0; i < size; i++) {
		if (strcmp(list[i], str) == 0) return i;
	}
	return -1;
}

void StringListRemoveDuplicates(char** list) {
	if (!list) return;
	int size = CSTRING_SIZE(list);
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (strcmp(list[i], list[j]) == 0) {
				free(list[j]);
				for (int k = j; k < size - 1; k++) {
					list[k] = list[k + 1];
				}
				size--;
				CSTRING_SIZE(list) = size;
				j--;
			}
		}
	}
}

void StringListReplaceInStrings(char** list, char* before, char* after) {
	if (!list || !before || !after) return;
	int beforeLen = strlen(before);
	int afterLen = strlen(after);
	int size = CSTRING_SIZE(list);

	for (int i = 0; i < size; i++) {
		char* str = list[i];
		char* pos = str;
		int occurrences = 0;

		while ((pos = strstr(pos, before)) != NULL) {
			occurrences++;
			pos += beforeLen;
		}

		if (occurrences == 0) continue;

		int oldLen = strlen(str);
		int newLen = oldLen + occurrences * (afterLen - beforeLen);
		char* newStr = (char*)malloc(newLen + 1);
		if (!newStr) continue;

		char* src = str;
		char* dest = newStr;
		while ((pos = strstr(src, before)) != NULL) {
			int len = pos - src;
			memcpy(dest, src, len);
			dest += len;
			memcpy(dest, after, afterLen);
			dest += afterLen;
			src = pos + beforeLen;
		}
		strcpy(dest, src);

		free(list[i]);
		list[i] = newStr;
	}
}

void StringListSort(char** list) {
	if (!list) return;
	int size = CSTRING_SIZE(list);
	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (strcmp(list[i], list[j]) > 0) {
				char* tmp = list[i];
				list[i] = list[j];
				list[j] = tmp;
			}
		}
	}
}