#include "stringList.h"

#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void _swapPtr(void** a, void** b) {
	void* temp = *a;
	*a = *b;
	*b = temp;
}
void _printInBox(const char* s, size_t n, size_t width, size_t padding, bool arrow, const char side, const char top, const char corners) {
	int l = (width - n - 2) / 2;
	// top border
	std::cout << corners;
	PRINT_NTIMES(top, width - 2);
	std::cout << corners << "\n";

	for (size_t i = 0; i < padding; i++) {
		std::cout << side; PRINT_NTIMES(' ', width - 2); std::cout << side << "\n";
	}


	// content
	std::cout << side; PRINT_NTIMES(' ', l);
	std::cout << s;
	PRINT_NTIMES(' ', width - n - l - 2); std::cout << side << "\n";

	for (size_t i = 0; i < padding; i++) {
		std::cout << side; PRINT_NTIMES(' ', width - 2); std::cout << side << "\n";
	}

	// bottom border
	std::cout << corners; 
	PRINT_NTIMES(top, width - 2); 
	std::cout << corners << "\n";

	if (arrow) {
		PRINT_NTIMES(' ', width / 2 - 1);
		std::cout << "||\n";
		PRINT_NTIMES(' ', width / 2 - 1);
		std::cout << "\\/\n";
	}
}
void stringListPPrint(char** list, size_t width, size_t padding, bool arrow, const char side, const char top, const char corners){
	_checkNullptr((void*)list);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(*list, &len, &last);

	for (int i = 0; i <= len; i++) {
		if(!list[i]) _printInBox("*EMPTY*", 7, width, padding, arrow, side, top, corners);
		else _printInBox(list[i], strlen(list[i]), width, padding, arrow, side, top, corners);
	}
	_printInBox("***END***", 9, width, padding, false, side, top, corners);
}

void _decodeHeader(const char* buffer, size_t* len, size_t* last) {
	_checkNullptr((void*)buffer);
	_checkNullptr((void*)len);
	_checkNullptr((void*)last);

	*len = 0;
	*last = 0;

	while (*buffer >= '0' && *buffer <= '9') {
		*len = *len * 10 + (*buffer - '0');
		buffer++;
	}	

	buffer++;

	while (*buffer >= '0' && *buffer <= '9') {
		*last = *last * 10 + (*buffer - '0');
		buffer++;
	}

}

void _encodeHeader(char** list, size_t len, size_t last) {
	_checkNullptr((void*)list);

	size_t l = _getNumLen(len);
	size_t r = _getNumLen(last);
	size_t headerLen = l + r + 2;


	char* newHeader = (char*)calloc(headerLen, sizeof(char));
	_checkNullptr((void*)newHeader);

	if (list[0]) free(list[0]);
	list[0] = newHeader;

	char* cur = *list + l;
	*cur = '|';
	cur--;

	do{
		*cur = (len % 10) + '0';
		len /= 10;
		cur--;
	} while (len > 0);
	
	cur = *list + headerLen - 2;
	do {
		*cur = (last % 10) + '0';
		last /= 10;
		cur--;
	} while (last > 0);

}

void _increaseCapacity(char*** list, size_t capacity){
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);

	size_t newLen = len + capacity + 1;
	char** newList = (char**)calloc(newLen, sizeof(char*));
	_checkNullptr(newList);

	for (size_t i = 0; i <= last; i++) {
		newList[i] = (*list)[i];
	}
	free(*list);
	*list = newList;

	_encodeHeader(*list, newLen-1, last);
}

void _decreaseCapacity(char*** list, size_t capacity) {
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);

	if (len - last < capacity * 2) return;

	size_t newLen = len - capacity + 1;
	char** newList = (char**)calloc(newLen, sizeof(char*));
	_checkNullptr(newList);

	for (size_t i = 0; i <= last; i++) {
		newList[i] = (*list)[i];
	}

	for (size_t i = last + 1; i <= len; i++) {
		if ((*list)[i] != NULL) free((*list)[i]);
	}

	free(*list);
	*list = newList;
	_encodeHeader(*list, newLen - 1, last);

}

void _compressList(char*** list) {
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);
	size_t cur = 1;
	for (size_t i = 1; i <= last; i++) {
		if ((*list)[i] != nullptr) {
			_swapPtr((void**)&(*list)[i], (void**)&(*list)[cur]);
			cur++;
		}
	}
	_encodeHeader(*list, len, cur - 1);
}

void stringListInit(char*** list) {
	_checkNullptr(list);
	*list = (char**)calloc(BUCKET_CAPACITY + 1, sizeof(char*));
	_checkNullptr(*list);
	_encodeHeader(*list, BUCKET_CAPACITY, 0);
}

void stringListDestroy(char*** list) {
	_checkNullptr((void*)list);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);

	for (size_t i = 0; i <= len; i++) {
		if ((*list)[i] != nullptr) {
			free((*list)[i]);
		}
	}
	free(*list);
	*list = nullptr;
}

void stringListAdd(char*** list, const char* text, size_t n) {
	_checkNullptr((void*)list);

	if (n == 0) n = strlen(text);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);

	if (len == last) {
		_increaseCapacity(list, BUCKET_CAPACITY);
		_decodeHeader(**list, &len, &last);
	}

	(*list)[last + 1] = (char*)calloc(n + 1, sizeof(char));
	memcpy((*list)[last + 1], text, n);

	_encodeHeader(*list, len, last + 1);
}

bool stringListRemoveString(char*** list, const char* str) {
	_checkNullptr((void*)list);
	_checkNullptr((void*)str);

	size_t len = 0;
	size_t last = 0;
	bool hasChanges = false;
	_decodeHeader(**list, &len, &last);

	for (size_t i = 1; i <= last; i++) {
		if (!(*list)[i]) continue;
		if (strcmp((*list)[i], str) == 0) {
			free((*list)[i]);
			(*list)[i] = nullptr;
			hasChanges = true;
		}
			
	}

	if (!hasChanges) return hasChanges;

	_compressList(list);

	_decreaseCapacity(list, BUCKET_CAPACITY);

	return hasChanges;
}

bool stringListRemoveAt(char*** list, size_t index) {
	_checkNullptr((void*)list);

	size_t len = 0;
	size_t last = 0;
	bool hasChanges = false;
	index++;
	_decodeHeader(**list, &len, &last);
	if (index < 1 || index > last) throw std::out_of_range("Index out of range!");
	if ((*list)[index] != nullptr) {
		free((*list)[index]);
		(*list)[index] = nullptr;
		hasChanges = true;
	}

	_compressList(list);
	_decreaseCapacity(list, BUCKET_CAPACITY);

	return hasChanges;
}

size_t stringListSize(char** list)
{
	_checkNullptr((void*)list);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(*list, &len, &last);
	return last;
}

int stringListFind(char** list, const char* pattern) {
	_checkNullptr((void*)list);
	_checkNullptr((void*)pattern);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(*list, &len, &last);
	for (size_t i = 1; i <= last; i++) {
		if (strcmp(list[i], pattern) == 0) {
			return (int)(i-1);
		}
	}

	return -1;
}

char* stringListGetAt(char** list, size_t index) {
	_checkNullptr((void*)list);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(*list, &len, &last);
	if (index < 0 || index >= last) throw std::out_of_range("Index out of range!");

	return list[index + 1];
}

bool stringListRemoveDuplicates(char*** list) {
	_checkNullptr((void*)list);

	char** newList = nullptr;
	stringListInit(&newList);

	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);

	for (size_t i = 1; i <= last; i++) {
		if (stringListFind(newList, (*list)[i]) == -1) {
			stringListAdd(&newList, (*list)[i]);
		}
	}

	size_t newLast = stringListSize(newList);

	stringListDestroy(list);
	*list = newList;

	return newLast != last;
}

bool stringListReplaceAll(char*** list, const char* pattern, const char* newStr) {
	_checkNullptr((void*)list);
	_checkNullptr((void*)pattern);
	_checkNullptr((void*)newStr);

	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);
	bool hasChanges = false;

	for (size_t i = 1; i <= last; i++) {
		if (strcmp((*list)[i], pattern) == 0) {
			size_t n = strlen(newStr);
			char* newString = (char*)calloc(n + 1, sizeof(char));
			_checkNullptr((void*)newString);

			memcpy(newString, newStr, n);

			free((*list)[i]);
			(*list)[i] = newString;

			hasChanges = true;
		}
	}
	
	return hasChanges;
}

bool stringListReplacePartAll(char*** list, const char* part, const char* newPart) {
	_checkNullptr((void*)list);
	_checkNullptr((void*)part);
	_checkNullptr((void*)newPart);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);
	bool hasChanges = false;

	size_t partLen = strlen(part);
	size_t newPartLen = strlen(newPart);



	for (size_t i = 1; i <= last; i++) {
		char* pos = strstr((*list)[i], part);
		if (pos != nullptr) {
			size_t oldLen = strlen((*list)[i]);
			size_t newLen = oldLen - partLen + newPartLen;

			char* newString = (char*)calloc(newLen + 1, sizeof(char));
			_checkNullptr((void*)newString);

			size_t prefixLen = pos - (*list)[i];

			memcpy(newString, (*list)[i], prefixLen);
			memcpy(newString + prefixLen, newPart, newPartLen);
			memcpy(newString + prefixLen + newPartLen, pos + partLen, oldLen - prefixLen - partLen);

			free((*list)[i]);
			(*list)[i] = newString;
			hasChanges = true;
		}
	}

	return hasChanges;
}

bool stringListSort(char*** list, bool ascending) {
	_checkNullptr((void*)list);
	size_t len = 0;
	size_t last = 0;
	_decodeHeader(**list, &len, &last);
	bool hasChanges = false;

	for (size_t i = 1; i < last; i++) {
		for (size_t j = 1; j <= last - i; j++) {
			int cmp = strcmp((*list)[j], (*list)[j + 1]);
			if ((ascending && cmp > 0) || (!ascending && cmp < 0)) {
				_swapPtr((void**)&(*list)[j], (void**)&(*list)[j + 1]);
				hasChanges = true;
			}
		}
	}

	return hasChanges;
}

