#include <stdlib.h>
#include <string.h>
#include "mylist.h"

void StringListInit(char ***list)
{
    *list = (char **)malloc((LIST_SIZE + 1) * sizeof(char *));
    if (!*list)
        return;

    for (int i = 0; i != LIST_SIZE; i++)
    {
        (*list)[i] = (char *)malloc(STRING_SIZE * sizeof(char));
        if (!(*list)[i])
        {
            for (int j = 0; j < i; j++)
            {
                free((*list)[j]);
            }
            free(*list);
            *list = NULL;
            return;
        }
        strcpy((*list)[i], EMPTY);
    }

    (*list)[LIST_SIZE] = NULL;
}

void StringListDestroy(char ***list)
{
    if (!list || !*list)
        return;

    int counter = 0;
    while ((*list)[counter])
    {
        free((*list)[counter]);
        counter++;
    }

    free(*list);
    *list = NULL;
}

void reallocate_list(char ***list, int size)
{
    int new_size = size * 2 + 1;
    char **old = *list;

    char **temp = (char **)realloc(old, new_size * sizeof(char *));
    if (!temp)
        return;

    *list = temp;

    for (int i = size; i < new_size - 1; i++)
    {
        (*list)[i] = (char *)malloc(STRING_SIZE * sizeof(char));
        strcpy((*list)[i], EMPTY);
    }

    (*list)[new_size - 1] = NULL;
}

void StringListAdd(char ***list, const char *str)
{
    if (!list || !*list)
        return;

    int size = StringListSize(list);

    if ((*list)[size] == NULL)
    {
        reallocate_list(list, size);
    }

    int needed = strlen(str) + 1;

    if (strlen((*list)[size]) < needed)
    {
        char *tmp = (char*)realloc((*list)[size], needed);
        if (!tmp)
            return;
        (*list)[size] = tmp;
    }

    strcpy((*list)[size], str);
}

void StringListRemove(char ***list, const char *str)
{
    if (!list || !*list)
        return;

    char **arr = *list;
    int read = 0;
    int write = 0;

    while (arr[read] != NULL)
    {
        if (strcmp(arr[read], str) != 0)
        {
            if (write != read)
                strcpy(arr[write], arr[read]);
            write++;
        }
        read++;
    }

    while (arr[write] != NULL)
    {
        strcpy(arr[write], EMPTY);
        write++;
    }
}

int StringListSize(char ***list)
{
    if (!list || !*list)
        return -1;

    int counter = 0;
    while ((*list)[counter] != NULL &&
           strcmp((*list)[counter], EMPTY) != 0)
    {
        counter++;
    }

    return counter;
}

int StringListIndexOf(char ***list, const char *str)
{
    if (!list || !*list)
        return -1;

    int counter = 0;
    while ((*list)[counter] != NULL)
    {
        if (strcmp((*list)[counter], EMPTY) != 0 &&
            strcmp((*list)[counter], str) == 0)
        {
            return counter;
        }
        counter++;
    }

    return -1;
}

void StringListRemoveDuplicates(char ***list)
{
    if (!list || !*list)
        return;

    char **arr = *list;
    int read = 0;
    int write = 0;

    while (arr[read] != NULL && strcmp(arr[read], EMPTY) != 0)
    {
        int seen = 0;
        for (int i = 0; i < write; i++)
        {
            if (strcmp(arr[read], arr[i]) == 0)
            {
                seen = 1;
                break;
            }
        }

        if (!seen)
        {
            if (write != read)
                strcpy(arr[write], arr[read]);
            write++;
        }

        read++;
    }

    while (arr[write] != NULL)
    {
        strcpy(arr[write], EMPTY);
        write++;
    }
}

void StringListReplaceInStrings(char ***list, const char *before, const char *after)
{
    if (!list || !*list || !before || before[0] == '\0')
        return;

    int counter = 0;
    while ((*list)[counter] != NULL &&
           strcmp((*list)[counter], EMPTY) != 0)
    {
        char *old = (*list)[counter];
        int old_len = strlen(old);
        int before_len = strlen(before);
        int after_len = strlen(after);

        int count = 0;
        for (char *p = old; (p = strstr(p, before)) != nullptr; p += before_len)
            count++;

        if (count == 0) {
            counter++;
            continue;
        }

        int new_len = old_len + count * (after_len - before_len);
        char *new_string = (char *)malloc(new_len + 1);
        if (!new_string)
            return;

        char *dst = new_string;
        char *src = old;

        while (*src)
        {
            char *pos = strstr(src, before);
            if (!pos)
            {
                strcpy(dst, src);
                dst += strlen(src);
                break;
            }

            int prefix_len = pos - src;
            memcpy(dst, src, prefix_len);
            dst += prefix_len;

            memcpy(dst, after, after_len);
            dst += after_len;

            src = pos + before_len;
        }
        *dst = '\0';

        free(old);
        (*list)[counter] = new_string;
        counter++;
    }
}

void swap(char **a, char **b)
{
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(char **list, int low, int high)
{
    char *pivot = list[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (strcmp(list[j], pivot) <= 0)
        {
            i++;
            swap(&list[i], &list[j]);
        }
    }

    swap(&list[i + 1], &list[high]);
    return i + 1;
}

void quicksort(char **list, int low, int high)
{
    if (low < high)
    {
        int pi = partition(list, low, high);
        quicksort(list, low, pi - 1);
        quicksort(list, pi + 1, high);
    }
}

void StringListSort(char ***list)
{
    if (!list || !*list)
        return;

    int size = StringListSize(list);
    if (size <= 1)
        return;

    quicksort(*list, 0, size - 1);
}
