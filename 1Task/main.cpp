#include "string_list.h"
#include <iostream>


void PrintList(char** list) {
	if(!list) { return; }
    int size = ListSize(list);
    std::cout << "List (" << size << " items):\n";
    for (int i = 1; i <= size; ++i)
        std::cout << "  [" << i << "] " << list[i] << '\n';
	std::cout << "\n\n";
}

int main() {

	char** list = nullptr;

	ListCreate(list);

	ListAdd(list, "one");
    ListAdd(list, "apple");
    ListAdd(list, "banana");
    ListAdd(list, "banana");
    ListAdd(list, "apple");
    ListAdd(list, "one two one");
    ListAdd(list, "apple");
    ListAdd(list, "orange");
    ListAdd(list, "banana");
    ListAdd(list, "one apple one");

	std::cout << "Adding strings:\n";
	PrintList(list);

	std::cout << "Size: " << ListSize(list) << "\n\n";

	std::cout << "Index of \"kiwi\": " << ListFindIndex(list, "kiwi") << '\n';
	std::cout << "Index of \"apple\": " << ListFindIndex(list, "apple") << "\n\n";

	ListEraseValue(list, "apple");
	ListEraseValue(list, "orange");
	std::cout << "Removing \"apple\" and \"orange\":\n";
	PrintList(list);

	ListRemoveDups(list);
	std::cout << "Removing duplicates:\n";
	PrintList(list);

    StringListReplaceInStrings(list, "one", "seven");
    std::cout << "Replace 'one' -> 'seven':\n";
    PrintList(list);

    ListSort(list);
    std::cout << "Sorting:\n";
    PrintList(list);

	ListDestroy(list);
    return 0;
}

