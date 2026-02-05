#include "string_list.h"
#include <stdio.h>
int main()
{
    char** list;
    string_list_init(&list);
    printf("%d\n", string_list_size(list));
    string_list_add(&list, "hello");
    string_list_add(&list, "world");
    string_list_add(&list, "meow");
    string_list_add(&list, "hello");
    printf("%d\n", string_list_size(list));
    string_list_remove(list, "meow");
    printf("%d\n", string_list_size(list));
    printf("%d\n", string_list_index_of(list, "meow"));
    string_list_destroy(&list);
}