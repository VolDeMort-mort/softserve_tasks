#define _CRT_SECURE_NO_WARNINGS
#define HEADER_SIZE (2 * sizeof(int))
#include <stdlib.h>
#include <string.h>
#include "string_list.h" 

static int* getHeader(char** list) {
    if (list == NULL) return NULL;
    return (int*)((char*)list - HEADER_SIZE);
}

void StringListInit(char*** list) {
    if (list == nullptr) return;

    int initial_capacity = 10;

    char** temp_block = (char**)malloc(HEADER_SIZE + sizeof(char*) * initial_capacity);

    if (temp_block == nullptr) return;

    int* header = (int*)temp_block;
    header[0] = 0;
    header[1] = initial_capacity;

    char** array_start = (char**)((char*)temp_block + HEADER_SIZE);
    for (int i = 0; i < initial_capacity; i++) {
        array_start[i] = nullptr;
    }

    *list = array_start;
}

void StringListDestroy(char*** list) {
    if (list == NULL || *list == NULL) return;

    char** str_list = *list;

    void* full_block = (char*)str_list - HEADER_SIZE;

    int* header = (int*)full_block;
    int size = header[0];

    for (int i = 0; i < size; i++) {
        if (str_list[i] != nullptr) {
            free(str_list[i]);
        }
    }

    free(full_block);

    *list = NULL;
}

void StringListAdd(char*** list, const char* str) {
    if (list == nullptr || *list == nullptr || str == nullptr) return;
    char** str_list = *list;

    void* full_block = (char*)str_list - HEADER_SIZE;

    int* header = (int*)full_block;
    int size = header[0];
    int capacity = header[1];

    if (size >= capacity) {
        int new_capacity = capacity + 10;
        size_t new_size_block = HEADER_SIZE + sizeof(char*) * new_capacity;

        void* temp = realloc(full_block, new_size_block);
        if (temp == nullptr)return;
        full_block = temp;

        header = (int*)full_block;
        header[1] = new_capacity;

        str_list = (char**)((char*)full_block + HEADER_SIZE);

        *list = str_list;
    }

    str_list[size] = _strdup(str);
    header[0] = size + 1;
}

int StringListSize(char*** list) {
    int* header = (list) ? getHeader(*list) : NULL;
    return (header) ? header[0] : 0;
}

void StringListRemove(char*** list, const char* str) {
    if (list == nullptr || *list == nullptr || str == nullptr) return;
    char** str_list = *list;

    void* full_block = (char*)str_list - HEADER_SIZE;

    int* header = (int*)full_block;
    int size = header[0];
    int capacity = header[1];

    for (size_t i = 0; i < size; i++)
    {
        if (strcmp(str, str_list[i]) == 0) {
            free(str_list[i]);
            for (size_t j = i; j < size - 1; j++)
            {
                str_list[j] = str_list[j + 1];
            }
            str_list[size] = nullptr;

            header[0]--;
            return;
        }
    }
}

int StringListIndexOf(char*** list, const char* str) {
    int size = StringListSize(list);
    char** str_list = *list;
    for (size_t i = 0; i < size; i++)
    {

        if (strcmp(str, str_list[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void StringListRemoveDuplicates(char*** list) {
    char** str_list = *list;
    int size = StringListSize(list);
    int* header = (int*)((char*)str_list - HEADER_SIZE);
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = i + 1; j < size; j++)
        {
            if (str_list[i] == NULL || str_list[j] == NULL) {
                continue;
            }
            if (strcmp(str_list[i], str_list[j]) == 0) {

                free(str_list[j]);

                for (int k = j; k < header[0] - 1; k++) {
                    str_list[k] = str_list[k + 1];
                }
                header[0]--;
                str_list[header[0]] = NULL;
                j--;
            }
        }
    }
}

void StringListSort(char*** list) {
    if (list == NULL || *list == NULL) return;

    char** str_list = *list;
    int* header = (int*)((char*)str_list - HEADER_SIZE);
    int size = header[0];

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {

            if (strcmp(str_list[j], str_list[j + 1]) > 0) {
                char* temp = str_list[j];
                str_list[j] = str_list[j + 1];
                str_list[j + 1] = temp;
            }
        }
    }
}


void StringListReplaceInStrings(char*** list, const char* before, const char* after) {
    if (list == nullptr || *list == nullptr || before == nullptr || after == nullptr) return;
    char** str_list = *list;

    void* full_block = (char*)str_list - HEADER_SIZE;

    int* header = (int*)full_block;
    int size = header[0];
    size_t beforeLen = strlen(before);
    size_t afterLen = strlen(after);

    for (size_t i = 0; i < size; i++)
    {
        char* currentStr = str_list[i];
        if (currentStr == NULL) continue;

        int count = 0;
        char* pos = currentStr;

        while ((pos = strstr(pos, before)) != NULL) {
            count++;
            pos += beforeLen;
        }

        if (count == 0) continue;

        size_t newLen = strlen(currentStr) + count * (afterLen - beforeLen);
        char* newStr = (char*)malloc(newLen + 1);
        if (newStr == NULL) continue;

        char* dest = newStr;
        char* src = currentStr;
        while ((pos = strstr(src, before)) != NULL) {
            size_t n = pos - src;
            memcpy(dest, src, n);
            dest += n;

            memcpy(dest, after, afterLen);
            dest += afterLen;

            src = pos + beforeLen;
        }
        strcpy(dest, src);

        free(str_list[i]);
        str_list[i] = newStr;
    }
}