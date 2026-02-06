#include "StringList.h"
#include <stdlib.h>
#include <string.h>

static size_t* cap_ptr(char** handle) {
    return (size_t*)(void*)handle;
}
static size_t* size_ptr(char** handle) {
    return (size_t*)((char*)handle + sizeof(size_t));
}
static char*** items_ptr_addr(char** handle) {
    return (char*** )((char*)handle + 2 * sizeof(size_t));
}
static char* my_strdup_c(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char* p = (char*)malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n + 1);
    return p;
}

void StringListInit(char*** list) {
    if (!list) return;
    size_t init_cap = 4;
    
    void* header = malloc(2 * sizeof(size_t) + sizeof(char*));
    if (!header) { *list = NULL; return; }
    
    *cap_ptr((char**)header) = init_cap;
    *size_ptr((char**)header) = 0;

    char** items = (char**)malloc(init_cap * sizeof(char*));
    if (!items) { free(header); *list = NULL; return; }
  
    memset(items, 0, init_cap * sizeof(char*));
    
    *items_ptr_addr((char**)header) = items;
    
    *list = (char**)header;
}

void StringListDestroy(char*** list) {
    if (!list) return;
    char** handle = *list;
    if (!handle) { *list = NULL; return; }
    char** items = *items_ptr_addr(handle);
    size_t sz = *size_ptr(handle);
    size_t i;
    for (i = 0; i < sz; ++i) {
        if (items[i]) free(items[i]);
    }
    free(items);
    free(handle);
    *list = NULL;
}

static int ensure_capacity(char** handle, size_t need) {
    if (!handle) return -1;
    size_t cap = *cap_ptr(handle);
    if (cap >= need) return 0;
    size_t newcap = cap ? cap * 2 : 4;
    while (newcap < need) newcap *= 2;
    char** items = *items_ptr_addr(handle);
    char** newitems = (char**)realloc(items, newcap * sizeof(char*));
    if (!newitems) return -1;
    if (newcap > cap) memset(newitems + cap, 0, (newcap - cap) * sizeof(char*));
    *items_ptr_addr(handle) = newitems;
    *cap_ptr(handle) = newcap;
    return 0;
}

void StringListAdd(char** list, const char* str) {
    if (!list || !str) return;
    char** handle = list;
    size_t sz = *size_ptr(handle);
    if (ensure_capacity(handle, sz + 1) != 0) return;
    char** items = *items_ptr_addr(handle);
    char* copy = my_strdup_c(str);
    if (!copy) return;
    items[sz] = copy;
    *size_ptr(handle) = sz + 1;
}

void StringListRemove(char** list, const char* str) {
    if (!list || !str) return;
    char** handle = list;
    char** items = *items_ptr_addr(handle);
    size_t sz = *size_ptr(handle);
    size_t i;
    for (i = 0; i < sz; ) {
        if (items[i] && strcmp(items[i], str) == 0) {
            free(items[i]);
            if (i + 1 < sz) memmove(&items[i], &items[i+1], (sz - i - 1) * sizeof(char*));
            items[sz - 1] = NULL;
            --sz;
            *size_ptr(handle) = sz;
        } else ++i;
    }
}

int StringListSize(char** list) {
    if (!list) return 0;
    char** handle = list;
    if (!handle) return 0;
    return (int)(*size_ptr(handle));
}

int StringListIndexOf(char** list, const char* str) {
    if (!list || !str) return -1;
    char** handle = list;
    char** items = *items_ptr_addr(handle);
    size_t sz = *size_ptr(handle);
    size_t i;
    for (i = 0; i < sz; ++i) {
        if (items[i] && strcmp(items[i], str) == 0) return (int)i;
    }
    return -1;
}

void StringListRemoveDuplicates(char** list) {
    if (!list) return;
    char** handle = list;
    char** items = *items_ptr_addr(handle);
    size_t sz = *size_ptr(handle);
    size_t i, j;
    for (i = 0; i < sz; ++i) {
        if (!items[i]) continue;
        for (j = i + 1; j < sz; ) {
            if (items[j] && strcmp(items[i], items[j]) == 0) {
                free(items[j]);
                if (j + 1 < sz) memmove(&items[j], &items[j+1], (sz - j - 1) * sizeof(char*));
                items[sz - 1] = NULL;
                --sz;
                *size_ptr(handle) = sz;
            } else ++j;
        }
    }
}

void StringListReplaceInStrings(char** list, const char* before, const char* after) {
    if (!list || !before || !after) return;
    size_t blen = strlen(before);
    if (blen == 0) return; 
    size_t alen = strlen(after);
    char** handle = list;
    char** items = *items_ptr_addr(handle);
    size_t sz = *size_ptr(handle);
    size_t i;
    for (i = 0; i < sz; ++i) {
        char* s = items[i];
        if (!s) continue;

        size_t occ = 0;
        char* p = s;
        while ((p = strstr(p, before)) != NULL) { ++occ; p += blen; }
        if (occ == 0) continue;
        size_t orig_len = strlen(s);
        size_t new_len;
        if (alen >= blen) new_len = orig_len + occ * (alen - blen);
        else new_len = orig_len - occ * (blen - alen);
        char* nb = (char*)malloc(new_len + 1);
        if (!nb) continue; 
        char* dst = nb;
        char* src = s;
        char* found;
        while ((found = strstr(src, before)) != NULL) {
            size_t seg = (size_t)(found - src);
            memcpy(dst, src, seg); dst += seg;
            memcpy(dst, after, alen); dst += alen;
            src = found + blen;
        }
        size_t tail = strlen(src);
        memcpy(dst, src, tail); dst += tail;
        *dst = '\0';
        free(items[i]);
        items[i] = nb;
    }
}

void StringListSort(char** list) {
    if (!list) return;
    char** handle = list;
    size_t sz = *size_ptr(handle);
    if (sz <= 1) return;
    char** items = *items_ptr_addr(handle);
    
    for (size_t i = 0; i < sz - 1; i++) {
        for (size_t j = 0; j < sz - i - 1; j++) {
          
            int cmp;
            
            if (items[j] == NULL && items[j + 1] == NULL) {
                cmp = 0;  
            } else if (items[j] == NULL) {
                cmp = 1; 
            } else if (items[j + 1] == NULL) {
                cmp = -1; 
            } else {
                cmp = strcmp(items[j], items[j + 1]);
            }
            
            if (cmp > 0) {
                char* temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
}