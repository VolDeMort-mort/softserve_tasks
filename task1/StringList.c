#include "StringList.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4

static size_t* getRaw(char** list) {
    return (size_t*)list - 2;
}

static int getSize(char** list) {
    return (int)(((size_t*)list)[-2]);
}

static int getCapacity(char** list) {
    return (int)(((size_t*)list)[-1]);
}

static void setSize(char** list, int size) {
    ((size_t*)list)[-2] = (size_t)size;
}

static void setCapacity(char** list, int capacity) {
    ((size_t*)list)[-1] = (size_t)capacity;
}

static int compareStrings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void stringListInit(char*** list) {
    size_t* raw = malloc(2 * sizeof(size_t) + INITIAL_CAPACITY * sizeof(char*));
    raw[0] = 0;
    raw[1] = INITIAL_CAPACITY;
    *list = (char**)(raw + 2);
}

void stringListDestroy(char*** list) {
    if (*list == NULL) {
        return;
    }

    int size = getSize(*list);
    for (int i = 0; i < size; i++) {
        free((*list)[i]);
    }

    free(getRaw(*list));
    *list = NULL;
}

void stringListAdd(char*** list, const char* str) {
    int size = getSize(*list);
    int capacity = getCapacity(*list);

    if (size >= capacity) {
        int newCapacity = capacity * 2;
        size_t* raw = getRaw(*list);
        size_t* newRaw = realloc(raw, 2 * sizeof(size_t) + newCapacity * sizeof(char*));
        *list = (char**)(newRaw + 2);
        setCapacity(*list, newCapacity);
    }

    (*list)[size] = strdup(str);
    setSize(*list, size + 1);
}

void stringListRemove(char*** list, const char* str) {
    int size = getSize(*list);
    char** arr = *list;
    int i = 0;

    while (i < size) {
        if (strcmp(arr[i], str) == 0) {
            free(arr[i]);
            for (int j = i; j < size - 1; j++) {
                arr[j] = arr[j + 1];
            }
            size--;
            setSize(*list, size);
        } else {
            i++;
        }
    }
}

int stringListSize(char** list) {
    if (list == NULL) {
        return 0;
    }
    return getSize(list);
}

int stringListIndexOf(char** list, const char* str) {
    int size = getSize(list);
    for (int i = 0; i < size; i++) {
        if (strcmp(list[i], str) == 0) {
            return i;
        }
    }
    return -1;
}

void stringListRemoveDuplicates(char*** list) {
    int size = getSize(*list);
    char** arr = *list;

    for (int i = 0; i < size; i++) {
        int j = i + 1;
        while (j < size) {
            if (strcmp(arr[i], arr[j]) == 0) {
                free(arr[j]);
                for (int k = j; k < size - 1; k++) {
                    arr[k] = arr[k + 1];
                }
                size--;
                setSize(*list, size);
            } else {
                j++;
            }
        }
    }
}

void stringListReplaceInStrings(char** list, const char* before, const char* after) {
    int size = getSize(list);
    size_t beforeLen = strlen(before);
    size_t afterLen = strlen(after);

    for (int i = 0; i < size; i++) {
        char* pos = strstr(list[i], before);
        if (pos != NULL) {
            int count = 0;
            char* temp = list[i];
            while ((temp = strstr(temp, before)) != NULL) {
                count++;
                temp += beforeLen;
            }

            size_t oldLen = strlen(list[i]);
            size_t newLen = oldLen + count * (afterLen - beforeLen);

            char* newStr = malloc(newLen + 1);
            char* src = list[i];
            char* dst = newStr;

            while ((pos = strstr(src, before)) != NULL) {
                size_t prefixLen = pos - src;
                memcpy(dst, src, prefixLen);
                dst += prefixLen;

                memcpy(dst, after, afterLen);
                dst += afterLen;

                src = pos + beforeLen;
            }

            strcpy(dst, src);

            free(list[i]);
            list[i] = newStr;
        }
    }
}

void stringListSort(char** list) {
    int size = getSize(list);
    qsort(list, size, sizeof(char*), compareStrings);
}
