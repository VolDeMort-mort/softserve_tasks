#include <iostream>
#include "string_list.h"

#define HEADER_SIZE (2 * sizeof(int))

int main() {
    char** myList = NULL;
    //init
    StringListInit(&myList);

    // adding
    StringListAdd(&myList, "Nazar");
    StringListAdd(&myList, "Nazar");
    StringListAdd(&myList, "Nazar");
    StringListAdd(&myList, "Nazar");
    StringListAdd(&myList, "Nazar");
    StringListAdd(&myList, "Mykola");
    StringListAdd(&myList, "Mykola");
    StringListAdd(&myList, "Mykola");
    StringListAdd(&myList, "Mykola");

    StringListPrint(&myList);
    int size = StringListSize(&myList);
    std::cout << "Size: " << size << std::endl;

    //get idex
    int index = StringListIndexOf(&myList, "Nazar");
    std::cout << "index: " << index << std::endl;

    //removing
    StringListRemove(&myList, "Nazar");
    size = StringListSize(&myList);
    std::cout << "Size: " << size << std::endl;

    //get idex
    index = StringListIndexOf(&myList, "Nazar");
    std::cout << "index: " << index << std::endl;

    //deleting duplicates
    StringListRemoveDuplicates(&myList);
    StringListPrint(&myList);

    // destroying
    StringListDestroy(&myList);
}

void StringListPrint(char*** list) {

    if (list == nullptr || *list == nullptr) return;
    char** str_list = *list;

    void* full_block = (char*)str_list - HEADER_SIZE;

    int* header = (int*)full_block;
    int size = header[0];

    for (size_t i = 0; i < size; i++)
    {
        std::cout << str_list[i] << std::endl;
    }
}
