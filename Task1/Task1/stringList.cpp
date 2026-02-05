#include "stringList.h"
#include <stdlib.h>
#include <string>

void StringListInit(stringList* list)
{
	if (!list) return;

	*list = (stringList)malloc(sizeof(char*));

	if (!*list ) return;
	(*list)[0] = nullptr;

}
void StringListDestroy(stringList* list)
{
	if (!list || !*list) return;

	//free all str at first
	stringList strList = *list;
	
	int size = stringListGetSize(*list);
	for (int i = 0; i < size; ++i)
		free(strList[i]);

	//free array of str
	free(strList);
	*list = nullptr;
}
int stringListGetSize(const stringList list)
{
	if (!list) return 0;
	int n = 0;
	while (list[n] != nullptr)
	{
		++n;
	}
	return n;
}

void StringListAdd(stringList& list, const char* str)
{
	if (!list) return;
	int size = stringListGetSize(list);

	// (size + 2), because size - count of already exist strs, + 1 to new str, + 1 to new after str
	stringList reallocList = (stringList)realloc(list, (size + 2) * sizeof(char*));
	if (reallocList == nullptr)
		return;

	list = reallocList;

	// _strdup to malloc memory and copy str to list[count]
	list[size] = _strdup(str);

	list[size + 1] = nullptr;


}

// remove all occurrences of str in the list
void StringListRemove(stringList list, char* str)
{
	if (!list) return;
	for (int i = 0; list[i] != nullptr;)
	{
		if (std::strcmp(list[i], str) == 0) {
			free(list[i]);

			// "count - i", because after free str, address still being 
			int count = stringListGetSize(list) - i;

			// memmove, to shift the right side of str
			std::memmove(&list[i], &list[i + 1], count * sizeof(char*));
		}
		else
			++i;
	}
}

int StringListIndexOf(stringList list, const char* str)
{
	if (!list) return -1;
	for (int i = 0; list[i] != nullptr; ++i)
	{
		if (std::strcmp(list[i], str) == 0)
			return i;
	}
	return -1;
}

void stringListRemoveAt(stringList list, int index)
{
	if (!list) return;
	free(list[index]);
	int count = stringListGetSize(list) - index;
	std::memmove(&list[index], &list[index + 1], count * sizeof(char*));
}

void StringListRemoveDuplicates(stringList list)
{
	if (!list) return;
	int size = stringListGetSize(list);
	for (int i = 0; i < size; ++i)
	{
		for (int j = i + 1; j < size;) {
			if (std::strcmp(list[i], list[j]) == 0)
			{
				stringListRemoveAt(list, j);
				size--;
			}
			else
				j++;
		}
	}
}

// replace all "before" to "after" in list
void StringListReplaceInStrings(stringList list, const char* before, const char* after)
{
	if (!list || !before || !after) return;

	if (strcmp(before, after) == 0) return;
	int size = stringListGetSize(list);

	for (int i = 0; i < size; ++i) {
		if (list[i] == nullptr)
			continue;

		int line = 0;
		while (true)
		{
			char* p = strstr(list[i] + line, before); //point to first "before" str index
			if (!p) break; // if not found occurrence return original string, also break the cycle
			
			size_t left = p - list[i]; // num of the len of symbols before substr
			size_t right = strlen(p + strlen(before)); // len of the right side after "before"

			size_t lenAfter = strlen(after);
			size_t newLen = left + right + lenAfter + 1; // len of a new string + 1 null terminated symbol

			char* newString = (char*)malloc(newLen);
			if (!newString) return;
			memcpy(newString, list[i], left);
			memcpy(newString + left, after, lenAfter);
			memcpy(newString + left + lenAfter, p + strlen(before), right + 1);

			free(list[i]);
			list[i] = newString;

			line = left + lenAfter; // set a new enter position of list[i]
		}
	}
}

// sorting list using bubble sort 
void StringListSort(stringList list)
{
	if (!list ) return;
	int size = stringListGetSize(list);
	bool swapped;
	for (int i = 0; i < size - 1; ++i)
	{
		swapped = false;
		for (int j = 0; j < size - i - 1; ++j)
		{
			if (_stricmp(list[j], list[j + 1]) > 0) // compare strings, check if the left side bigger than right in alphabet, then swap
			{
				char* temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
				swapped = true;
			}

		}
		if (!swapped)
			break;
	}
}