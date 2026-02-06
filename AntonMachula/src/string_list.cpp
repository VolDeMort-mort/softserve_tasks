#include "string_list.h"

#include <cstdlib>
#include <cstring>

#define INITIAL_CAPACITY 4

#define LIST_CAPACITY(list) (((size_t *)(list))[-2])
#define LIST_SIZE(list) (((size_t *)(list))[-1])
#define LIST_BASE(list) ((size_t *)(list) - 2)

void StringListInit(char ***list) {
  auto base = static_cast<size_t *>(
      malloc(2 * sizeof(size_t) + INITIAL_CAPACITY * sizeof(char *)));
  base[0] = INITIAL_CAPACITY;
  base[1] = 0;
  *list = (char **)(base + 2);
}

void StringListDestroy(char ***list) {
  auto size = LIST_SIZE(*list);
  for (size_t i = 0; i < size; i++) {
    free((*list)[i]);
  }
  free(LIST_BASE(*list));
  *list = nullptr;
}

void StringListAdd(char ***list, const char *str) {
  auto size = LIST_SIZE(*list);
  auto capacity = LIST_CAPACITY(*list);

  if (size == capacity) {
    auto new_capacity = capacity * 2;
    auto old_base = LIST_BASE(*list);
    auto new_base = static_cast<size_t *>(
        realloc(old_base, 2 * sizeof(size_t) + new_capacity * sizeof(char *)));
    *list = reinterpret_cast<char **>(new_base + 2);
    LIST_CAPACITY(*list) = new_capacity;
  }

  (*list)[size] = static_cast<char *>(malloc(strlen(str) + 1));
  strcpy((*list)[size], str);
  LIST_SIZE(*list) = size + 1;
}

void StringListRemove(char ***list, const char *str) {
  auto size = LIST_SIZE(*list);
  size_t write = 0;

  for (size_t read = 0; read < size; read++) {
    if (strcmp((*list)[read], str) == 0) {
      free((*list)[read]);
    } else {
      (*list)[write] = (*list)[read];
      write++;
    }
  }

  LIST_SIZE(*list) = write;
}

int StringListSize(char **list) { return (int)LIST_SIZE(list); }

int StringListIndexOf(char **list, const char *str) {
  auto size = LIST_SIZE(list);
  for (size_t i = 0; i < size; i++) {
    if (strcmp(list[i], str) == 0) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void StringListRemoveDuplicates(char ***list) {
  auto size = LIST_SIZE(*list);
  size_t write = 0;

  for (size_t read = 0; read < size; read++) {
    bool is_duplicate = false;
    for (size_t i = 0; i < write; i++) {
      if (strcmp((*list)[i], (*list)[read]) == 0) {
        is_duplicate = true;
        break;
      }
    }

    if (is_duplicate) {
      free((*list)[read]);
    } else {
      (*list)[write] = (*list)[read];
      write++;
    }
  }

  LIST_SIZE(*list) = write;
}

void StringListReplaceInStrings(char **list, const char *before,
                                const char *after) {
  auto size = LIST_SIZE(list);
  auto before_len = strlen(before);
  auto after_len = strlen(after);

  if (before_len == 0)
    return;

  for (size_t i = 0; i < size; i++) {
    size_t count = 0;
    auto *p = list[i];
    while ((p = strstr(p, before)) != nullptr) {
      count++;
      p += before_len;
    }

    if (count == 0)
      continue;

    auto old_len = strlen(list[i]);
    auto new_len = old_len + count * (after_len - before_len);
    auto new_str = static_cast<char *>(malloc(new_len + 1));

    auto *src = list[i];
    auto *dst = new_str;
    while ((p = strstr(src, before)) != nullptr) {
      auto chunk_len = static_cast<size_t>(p - src);
      memcpy(dst, src, chunk_len);
      dst += chunk_len;
      memcpy(dst, after, after_len);
      dst += after_len;
      src = p + before_len;
    }
    strcpy(dst, src);

    free(list[i]);
    list[i] = new_str;
  }
}

static int string_compare(const void *a, const void *b) {
  auto str_a = *static_cast<const char *const *>(a);
  auto str_b = *static_cast<const char *const *>(b);
  return strcmp(str_a, str_b);
}

void StringListSort(char **list) {
  auto size = LIST_SIZE(list);
  qsort(list, size, sizeof(char *), string_compare);
}
