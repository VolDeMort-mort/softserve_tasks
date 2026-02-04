#include "string_list.h"


void ListCreate(char**& list) {
	list = static_cast<char**>(malloc(sizeof(char*)));

	if(!list) {
		return;
	}

	list[0] = static_cast<char*>(malloc(sizeof(int)));
	
	if(!list[0]) {
		free(list);
		list = nullptr;
		return;
	}

	ListSize(list) = 0;
}


void ListDestroy(char**& list) {
	if(!list) { return; }
	int size = ListSize(list);

	for(int i = 1; i <= size; ++i) {
		free(list[i]);
	}

	free(list[0]);
	free(list);
	list = nullptr;
}


void ListAdd(char**& list, const char* str) {
	int size = ListSize(list);

	char** tmp = static_cast<char**>(realloc(list, (size + 2) * sizeof(char*)));

	if(!tmp) { return; } 

	list = tmp;

	list[size + 1] = static_cast<char*>(malloc(strlen(str) + 1));
	if(!list[size + 1]) { return; }

	strcpy(list[size + 1], str);

	++ListSize(list);
}


void ListEraseValue(char** list, const char *str) {
	int size = ListSize(list);
	int write = 1;

	for(int read = 1; read <= size; ++read) {
		if(strcmp(list[read], str) != 0) {
			list[write++] = list[read];
		} else {
			free(list[read]);
		}
	}
	ListSize(list) = write - 1;
}


int& ListSize(char** list) {
    return *reinterpret_cast<int*>(list[0]);
}


int ListFindIndex(char** list, const char *str) {
	if(!list || !str){ return -1; }
	int size = ListSize(list);

	for(int i = 1; i <= size; ++i) {
		if(strcmp(list[i], str) == 0) { return i; } 
	}

	return -1;
}


void ListRemoveDups(char** list) {
	if(!list) { return; }
	int size = ListSize(list);

	for(int i = 1; i <= size; ++i) {
		int write = i + 1;
		for(int read = i + 1; read <= size; ++read) {
			if(strcmp(list[i], list[read]) != 0) {
				list[write++] = list[read];
			} else {
				free(list[read]);
			}
		}
		size = write - 1;
	}
	ListSize(list) = size;
}


void ListSort(char** list) {
	if(!list) { return; }
	int size = ListSize(list);

	for(int i = 1; i < size; ++i) {
		int minIndex = i;
		for(int j = i + 1; j <= size; ++j) {
			if(strcmp(list[j], list[minIndex]) < 0) {
				minIndex = j;
			}
		}
		if(minIndex != i) {
			char* tmp = list[i];
			list[i] = list[minIndex];
			list[minIndex] = tmp;
		}
	}
}


void StringListReplaceInStrings(char** list, const char* before, const char* after) {
	if(!list || !before || !after) { return; }

    int size = ListSize(list);
    int beforeLen = strlen(before);
    int afterLen  = strlen(after);

    if(beforeLen == 0) { return; }

    for(int i = 1; i <= size; ++i) {
        char* src = list[i];

        int count = 0;
        char* p = src;
        while((p = strstr(p, before))) {
            ++count;
            p += beforeLen;
        }

        if(count == 0) { continue; }

        int newLen = strlen(src) + count * (afterLen - beforeLen);
        char* dst = static_cast<char*>(malloc(newLen + 1));

        char* d = dst;
        char* s = src;

        while((p = strstr(s, before))) {
            int len = p - s;
            memcpy(d, s, len);
            d += len;

            memcpy(d, after, afterLen);
            d += afterLen;

            s = p + beforeLen;
        }

        strcpy(d, s);

        free(list[i]);
        list[i] = dst;
    }
}


