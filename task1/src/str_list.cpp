#include <stdlib.h>
#include <string.h>
#include "str_list.h"


void list_init(char*** list)
{
	if (list == nullptr)
		return;

	std::size_t SIZE = 0;
	std::size_t CAPACITY = 10;

	std::size_t* raw_list = (size_t*)malloc(2 * sizeof(std::size_t) + CAPACITY * sizeof(char*));

	if (raw_list == nullptr)
	{
		*list = NULL;
		return;
	}

	raw_list[0] = SIZE;
	raw_list[1] = CAPACITY;

	char** list_with_data = (char**)(raw_list + 2);

	for (std::size_t i = 0; i < CAPACITY; ++i)
	{
		list_with_data[i] = nullptr;
	}

	*list = list_with_data;
}


std::size_t get_capacity(char** list)
{
	if (list == nullptr)
		return 0;

	return ((std::size_t*)list)[-1];
}


std::size_t get_size(char** list)
{
	if (list == nullptr)
		return 0;

	return ((std::size_t*)list)[-2];
}


void string_add(char** list, string str)
{
	if (list == nullptr)
		return;

	std::size_t capacity = get_capacity(list);
	std::size_t size = get_size(list);

	std::size_t* raw_list = (std::size_t*)list - 2;

	if (size == capacity)
	{
		capacity *= 2;
		raw_list = (std::size_t*)realloc(raw_list, 2 * sizeof(std::size_t) + capacity * sizeof(char*));
		if (!raw_list)
			return;

		raw_list[1] = capacity;

		list = (char**)(raw_list + 2);

		for (std::size_t i = size; i < capacity; ++i)
		{
			list[i] = nullptr;
		}
	}

	list[size] = _strdup(str);
	((std::size_t*)list)[-2] = size + 1;
}


void string_remove(char** list, string str)
{
	if (list == nullptr || str == nullptr)
		return;

	std::size_t size = get_size(list);
	std::size_t new_index = 0;

	for (std::size_t i = 0; i < size; ++i)
	{
		if (list[i] != nullptr && strcmp(list[i], str) == 0)
		{
			free(list[i]);
			list[i] = nullptr;
		}
		else
		{
			if (i != new_index)
			{
				list[new_index] = list[i];
			}
			++new_index;
		}
	}

	for (std::size_t j = new_index; j < size; ++j)
	{
		list[j] = nullptr;
	}

	((std::size_t*)list)[-2] = new_index;
}


int return_index_str(char** list, string str)
{
	if (list == nullptr || str == nullptr)
		return -1;

	std::size_t size = get_size(list);

	for (std::size_t i = 0; i < size; ++i)
	{
		if (strcmp(str, list[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}



void remove_duplicates(char** list)
{
	if (list == nullptr)
		return;

	std::size_t size = get_size(list);
	std::size_t new_size = 0;

	for (std::size_t i = 0; i < size; ++i)
	{
		if (list[i] == nullptr)
			continue;

		bool duplicate = false;

		for (std::size_t j = 0; j < new_size; ++j)
		{

			if (list[j] != nullptr && strcmp(list[j], list[i]) == 0)
			{
				duplicate = true;
				break;
			}
		}
		if (duplicate)
		{
			free(list[i]);
			list[i] = nullptr;
		}
		else
		{
			if (new_size != i)
				list[new_size] = list[i];

			++new_size;
		}
	}

	for (std::size_t j = new_size; j < size; ++j)
		list[j] = nullptr;

	((std::size_t*)list)[-2] = new_size;
}



void replace_string(char** list, const char* before, const char* after)
{
	if (list == nullptr || after == nullptr || before == nullptr)
		return;

	std::size_t size = get_size(list);
	for (std::size_t i = 0; i < size; ++i)
	{
		if (list[i] == nullptr)
			continue;

		char* current = list[i];

		std::size_t before_size = strlen(before);
		std::size_t after_size = strlen(after);

		std::size_t count = 0;
		for (char* tmp = current; (tmp = strstr(tmp, before)) != nullptr; tmp += before_size)
		{
			++count;
		}
		if (count == 0)
			continue;

		std::size_t old_len = strlen(current);
		std::size_t new_len = old_len + count * (after_size - before_size);

		char* new_str = (char*)malloc(new_len + 1);
		if (!new_str)
			return;

		char* write_ptr = new_str;
		char* read_ptr = current;

		while (*read_ptr)
		{
			char* pos = strstr(read_ptr, before);

			if (pos == nullptr)
			{
				strcpy(write_ptr, read_ptr);
				break;
			}

			std::size_t n = pos - read_ptr;
			memcpy(write_ptr, read_ptr, n);
			write_ptr += n;

			memcpy(write_ptr, after, after_size);
			write_ptr += after_size;


			read_ptr = pos + before_size;
		}

		*write_ptr = '\0';

		free(list[i]);
		list[i] = new_str;
	}
}


int comparer(const void* a, const void* b)
{
	if (a == nullptr || b == nullptr)
		return -1;

	char* const* a_arg = (char* const*)(a);
	char* const* b_arg = (char* const*)(b);
	return strcmp(*a_arg, *b_arg);
}


void sort_list(char** list)
{
	if (list == nullptr)
		return;

	std::size_t size = get_size(list);
	qsort(list, size, sizeof(char*), comparer);
}


void remove_list(char*** list)
{
	if (!list || !*list) return;
	std::size_t size = get_size(*list);
	for (std::size_t i = 0; i < size; ++i)
	{
		free((*list)[i]);
	}
	std::size_t* raw_list = (std::size_t*)(*list) - 2;
	free(raw_list);
	*list = nullptr;
}