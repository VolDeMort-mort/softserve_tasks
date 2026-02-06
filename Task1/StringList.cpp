#include "Stringlist.h"


#define HEADER_SIZE (2 * sizeof(int))
#define GET_CAPACITY(list)(((int*)( (char*)(list) - HEADER_SIZE ))[0])
#define GET_SIZE(list)(((int*)( (char*)(list) - HEADER_SIZE ))[1])

#define CAPACITY_KOEF 1.5
#define CAPACITY_INIT 5

void SetMetadata(char** list, int capacity, int size) {
     int* meta = (int*)((char*)list - HEADER_SIZE);
     meta[0] = capacity;
     meta[1] = size;
}

void StringListInit(char*** list) {
     int initialCapacity = CAPACITY_INIT;
     void* buffer = malloc(HEADER_SIZE + (sizeof(char*) * initialCapacity));
     if (!buffer) return;

     *list = (char**)((char*)buffer + HEADER_SIZE);
     SetMetadata(*list, initialCapacity, 0);
}

void StringListDestroy(char*** list) {
     if (!list || !*list) return;

     int size = GET_SIZE(*list);
     for (int i = 0; i < size; i++) {
          free((*list)[i]);
     }

     free((char*)(*list) - HEADER_SIZE);
     *list = NULL;
}

void StringListAdd(char*** list, char* str) {
     int size = GET_SIZE(*list);
     int capacity = GET_CAPACITY(*list);

     if (size >= capacity) {
          capacity *= CAPACITY_KOEF;
          void* buffer = realloc((char*)(*list) - HEADER_SIZE, HEADER_SIZE + (sizeof(char*) * capacity));
          if (!buffer) return;
          *list = (char**)((char*)buffer + HEADER_SIZE);
     }

     (*list)[size] = (char*)malloc(strlen(str) + 1);
     if ((*list)[size]) {
          strcpy((*list)[size], str);
          SetMetadata(*list, capacity, size + 1);
     }
}

void StringListRemoveDuplicates(char** list) {
     if (!list) return;
     int size = GET_SIZE(list);

     for (int i = 0; i < size; i++) {
          for (int j = i + 1; j < size; j++) {
               if (strcmp(list[i], list[j]) == 0) {

                    free(list[j]);
                    for (int k = j; k < size - 1; k++) {
                         list[k] = list[k + 1];
                    }
                    size--;
                    j--;
               }
          }
     }
     SetMetadata(list, GET_CAPACITY(list), size);
}

void StringListReplaceInStrings(char** list, char* before, char* after) {
     if (!list || !before || !after) return;
     
     int size = GET_SIZE(list);
     int beforeLen = strlen(before);
     int afterLen = strlen(after);

     for (int i = 0; i < size; i++) {
          char* current = list[i];
          char* pos = strstr(current, before);

          while (pos != NULL) {

               int currentLen = strlen(current);
               char* newStr = (char*)malloc(currentLen - beforeLen + afterLen + 1);

               // Constructing new string
               int prefixLen = pos - current;
               strncpy(newStr, current, prefixLen);
               strcpy(newStr + prefixLen, after);
               strcpy(newStr + prefixLen + afterLen, pos + beforeLen);

               free(current);
               list[i] = newStr;
               current = newStr;

               // Next case in the same string
               pos = strstr(current + prefixLen + afterLen, before);
          }
     }
}

int StringListSize(char** list) {
     return list ? GET_SIZE(list) : 0;
}

int StringListIndexOf(char** list, char* str) {
     if (!list) return -1;
     int size = GET_SIZE(list);
     for (int i = 0; i < size; i++) {
          if (strcmp(list[i], str) == 0) return i;
     }
     return -1;
}

void StringListRemove(char** list, char* str) {
     if (!list) return;
     int size = GET_SIZE(list);
     for (int i = 0; i < size; i++) {
          if (strcmp(list[i], str) == 0) {
               free(list[i]);
               for (int j = i; j < size - 1; j++) {
                    list[j] = list[j + 1];
               }
               size--;
               i--;
          }
     }
     SetMetadata(list, GET_CAPACITY(list), size);
}

void StringListSort(char** list) {
     if (!list) return;
     int size = GET_SIZE(list);
     for (int i = 0; i < size - 1; i++) {
          for (int j = 0; j < size - i - 1; j++) {
               if (strcmp(list[j], list[j + 1]) > 0) {
                    char* temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
               }
          }
     }
}