#include <stdio.h>
#include "StringList.h"

void printList(char** list) {
    int size = stringListSize(list);
    for (int i = 0; i < size; i++) {
        printf("%d) addr = %p, \"%s\"\n", i, (void*)list[i], list[i]);
    }
}

int main(void) {
    char** list;
    stringListInit(&list);
    printf("Init: list = %p, size = %d\n\n", (void*)list, stringListSize(list));

    printf("Adding...\n");
    stringListAdd(&list, "Hello from the other side");
    stringListAdd(&list, "I must have called a thousand times");
    stringListAdd(&list, "Never mind I'll find someone like you");
    stringListAdd(&list, "We could have had it all");
    printf("After 4 adds: size = %d, list = %p\n", stringListSize(list), (void*)list);

    stringListAdd(&list, "Rolling in the deep");
    printf("After 5th add (realloc): size = %d, list = %p\n", stringListSize(list), (void*)list);

    stringListAdd(&list, "Set fire to the rain");
    stringListAdd(&list, "Hello from the other side");
    stringListAdd(&list, "Chasing pavements");

    printf("\nCurrent list (size = %d, list at %p):\n", stringListSize(list), (void*)list);
    printList(list);

    printf("\nLooking up index\n");
    int idx = stringListIndexOf(list, "Rolling in the deep");
    printf("stringListIndexOf(\"Rolling in the deep\") = %d\n", idx);
    printf("Is correct? list[%d] at %p = \"%s\"\n", idx, (void*)list[idx], list[idx]);

    printf("\nRemoving duplicates/n");
    printf("Before: size = %d\n", stringListSize(list));
    stringListRemoveDuplicates(&list);
    printf("After:  size = %d\n", stringListSize(list));
    printList(list);

    printf("\nRemoving a string/n");
    printf("Removing \"Chasing pavements\"\n");
    printf("Before: size = %d\n", stringListSize(list));
    stringListRemove(&list, "Chasing pavements");
    printf("After:  size = %d\n", stringListSize(list));
    printList(list);

    printf("\nReplacing a substring\n");
    printf("Replacing \"the\" with \"da\"\n");
    stringListReplaceInStrings(list, "the", "da");
    printList(list);

    printf("\nAdding then sorting\n");
    stringListAdd(&list, "Easy on me");
    stringListAdd(&list, "Skyfall");
    printf("Before sort:\n");
    printList(list);
    stringListSort(list);
    printf("After sort:\n");
    printList(list);

    printf("\nDone!\n");
    printf("Before destroy: list = %p, size = %d\n", (void*)list, stringListSize(list));
    stringListDestroy(&list);
    printf("After destroy:  list = %p\n", (void*)list);

    return 0;
}
