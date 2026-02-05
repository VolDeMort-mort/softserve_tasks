#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <stdexcept>
#include <iostream>

#define BUCKET_CAPACITY 5
#define PRINT_NTIMES(c, n) {for(size_t i = 0; i < n; i++){ std::cout << c;}}

void _swapPtr(void** a, void** b);

void _printInBox(const char* s, size_t n, size_t width, size_t padding = 1, bool arrow = true, const char side = '|', const char top = '-', const char corners = '+');

inline void _checkNullptr(void* el) {
	if (!el) throw std::runtime_error("Nullptr access denied!");
}

inline size_t _getNumLen(unsigned int n) {
	size_t res = 1;
	while (n /= 10) res++;
	return res;
}

void stringListPPrint(char** list, size_t width, size_t padding = 1, bool arrow = true, const char side = '|', const char top = '-', const char corners = '+');

void _decodeHeader(const char* buffer, size_t* len, size_t* last);
void _encodeHeader(char** buffer, size_t len, size_t last);

void _increaseCapacity(char*** list, size_t capacity = BUCKET_CAPACITY);
void _decreaseCapacity(char*** list, size_t capacity = BUCKET_CAPACITY);

void stringListInit(char*** list);

void stringListDestroy(char*** list);

void stringListAdd(char*** list, const char* text, size_t n = 0);

bool stringListRemoveString(char*** list, const char* str);
bool stringListRemoveAt(char*** list, size_t index);

size_t stringListSize(char** list);

int stringListFind(char** list, const char* pattern);

char* stringListGetAt(char** list, size_t index);

bool stringListRemoveDuplicates(char*** list);

bool stringListReplaceAll(char*** list, const char* pattern, const char* newStr);

bool stringListReplacePartAll(char*** list, const char* part, const char* newPart);

bool stringListSort(char*** list, bool ascending = true);

#endif // !STRINGLIST_H	
