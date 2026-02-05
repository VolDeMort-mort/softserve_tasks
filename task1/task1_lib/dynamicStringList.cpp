#include "pch.h"
#include "dynamicStringList.h"

static inline void innerStringListSetSize(char** list, size_t new_size) {
	*((size_t*)(list) - 2) = new_size;
}

static inline void innerStringListSetCapacity(char** list, size_t new_capacity) {
	*((size_t*)(list) - 1) = new_capacity;
}

static inline void StringListSetCapacity(char*** list, size_t new_capacity) {
	if (!list) return;

	if (!*list) {
		StringListInit(list, new_capacity);
		return;
	}

	if (new_capacity == 0) {
		StringListDestroy(list);
		return;
	}

	void* data = (void*)((size_t*)(*list) - 2);
	size_t old_capacity = StringListCapacity(*list);
	size_t old_size = StringListSize(*list);

	if (new_capacity < old_size) {
		for (size_t i = new_capacity; i < old_size; ++i) {
			if ((*list)[i]) {
				free((*list)[i]);
				(*list)[i] = NULL;
			}
		}
	}

	void* new_data = realloc(data, new_capacity * sizeof(char*) + 2 * sizeof(size_t));
	if (!new_data) {
		return;
	}

	*list = (char**)((size_t*)new_data + 2);

	if (new_capacity < old_size) innerStringListSetSize(*list, new_capacity);
	innerStringListSetCapacity(*list, new_capacity);

	if (new_capacity > old_size) {
		for (size_t i = old_size; i < new_capacity; ++i) {
			(*list)[i] = NULL;
		}
	}
}
//-----------------------------------------------------------------------------
/* 
StringListInit initializes dynamic list of char*
Before the list alloceted meta information ablout it: size and capacity   
*/
void StringListInit(char*** list, size_t init_capacity) {
	if (!list) return;

	void* data = malloc(init_capacity * sizeof(char*) + 2 * sizeof(size_t));
	if (!data) {
		*list = NULL;
		return;
	}

	*list = (char**)((size_t*)data + 2);

	innerStringListSetSize(*list, 0);
	innerStringListSetCapacity(*list, init_capacity);

	for (size_t i = 0; i < init_capacity; ++i) {
		(*list)[i] = NULL;
	}
}

void StringListDestroy(char*** list) {
	if (!list || !*list) return;

	size_t size = StringListSize(*list);
	char** arr = *list;

	for (size_t i = 0; i < size; ++i) {
		if (arr[i]) {
			free(arr[i]);
			arr[i] = NULL;
		}
	}

	void* data = (void*)((size_t*)(*list) - 2);
	free(data);
	*list = NULL;
}


	
void StringListAdd(char*** list, String str) {
	if (!list || !str || !*list) return;

	size_t capacity = StringListCapacity(*list);
	size_t size = StringListSize(*list);

	if (size == capacity) {
		StringListSetCapacity(list, capacity * 2);
		if (StringListCapacity(*list) == capacity) return;	
	}

	size_t len = strlen(str) + 1;
	char* listStr = (char*)malloc(len);
	if (!listStr) return;

	memcpy(listStr, str, len);
	(*list)[size] = listStr;

	innerStringListSetSize(*list, ++size);
}

void StringListRemove(char*** list, String str)
{
	if (!list || !str || !*list) return;

	char** arr = *list;
	size_t capacity = StringListCapacity(arr);
	size_t size = StringListSize(arr);

	size_t i = 0;
	for (size_t j = 0; j < size; ++j) {
		char* s = arr[j];
		if (s && strcmp(s, str) == 0) {
			free(s);
			arr[j] = NULL;
			continue;
		}
		arr[i] = arr[j];
		++i;
	}

	size_t new_size = i;
	innerStringListSetSize(*list, new_size);

	if (new_size < capacity / 4 && capacity > MIN_CAP) {
		size_t new_cap = capacity / 2;
		if (new_cap < MIN_CAP) new_cap = MIN_CAP;
		StringListSetCapacity(list, new_cap);

		arr = *list;
		if (StringListCapacity(arr) == capacity) return;
	}
}

size_t StringListSize(char** list) {
	return *((size_t*)(list) - 2);
}

size_t StringListCapacity(char** list) {
	return *((size_t*)(list) - 1);
}

// Returns max for size_t if not found
size_t StringListIndexOf(char** list, const char* str) {
	if (!list || !str) return -1;

	size_t size = StringListSize(list);
	for (size_t i = 0; i < size; ++i) {
		if (list[i] && strcmp(list[i], str) == 0) return i;
	}
	return -1;
}

void StringListRemoveDuplicates(char*** list) {
	if (!list) return;

	char** arr = *list;

	size_t size = StringListSize(arr);
	size_t write = 0;

	for (size_t read = 0; read < size; ++read) {
		char* s = arr[read];
		if (!s) continue;

		bool is_dup = false;
		for (size_t i = 0; i < write; ++i) {
			if (arr[i] && strcmp(arr[i], s) == 0) {
				is_dup = true;
				break;
			}
		}

		if (is_dup) {
			free(s);
			arr[read] = NULL;
			continue;
		}
		if (write != read) {
			arr[write] = arr[read];
			arr[read] = NULL;
		}
		++write;
	}

	innerStringListSetSize(arr, write);

	size_t capacity = StringListCapacity(arr);
	 
	if (write < capacity / 4 && capacity > MIN_CAP) {
		size_t new_cap = capacity / 2;
		if (new_cap < MIN_CAP) new_cap = MIN_CAP;
		StringListSetCapacity(list, new_cap);

		arr = *list;
		if (StringListCapacity(arr) == capacity) return;
	}
}

void StringListReplaceInStrings(char** list, const char* before, const char* after) {
	if (!list || !before || !after) return;

	size_t size_after = strlen(after) + 1;
	size_t size_list = StringListSize(list);

	for (size_t i = 0; i < size_list; ++i) {
		if (list[i] && strcmp(list[i], before) == 0) {
			char* listStr = (char*)malloc(size_after);
			if (!listStr) {
				return;
			}
			memcpy(listStr, after, size_after);
			free(list[i]);
			list[i] = listStr;

		}
	}
}

static int cmp_str(const void* a, const void* b) {
	const char** ca = (const char**)a;
	const char** cb = (const char**)b;

	if (*ca == NULL && *cb == NULL) return 0;
	if (*ca == NULL) return 1;
	if (*cb == NULL) return -1;
	return strcmp(*ca, *cb);
}


void StringListSort(char** list) {
	if (!list) return;

	size_t size = StringListSize(list);
	if (size <= 1) return;

	qsort(list, (size_t)size, sizeof(char*), cmp_str);
}