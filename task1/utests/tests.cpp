#include <gtest/gtest.h>
#include "str_list.h"

TEST(ListInit, CreatesEmptyList)
{
    char** list = nullptr;

    list_init(&list);

    ASSERT_NE(list, nullptr);
    EXPECT_EQ(get_size(list), 0);
    EXPECT_EQ(get_capacity(list), 10);

    remove_list(&list);
}

TEST(StringAdd, AddsSingleString)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "hello");

    EXPECT_EQ(get_size(list), 1);
    EXPECT_STREQ(list[0], "hello");

    remove_list(&list);
}


TEST(StringAdd, ResizesWhenCapacityExceeded)
{
    char** list = nullptr;
    list_init(&list);

    for (int i = 0; i < 11; ++i)
        string_add(list, "x");

    EXPECT_EQ(get_size(list), 11);
    EXPECT_EQ(get_capacity(list), 20);

    remove_list(&list);
}

TEST(ReturnIndex, FindsCorrectIndex)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "one");
    string_add(list, "two");
    string_add(list, "three");

    EXPECT_EQ(return_index_str(list, "two"), 1);
    EXPECT_EQ(return_index_str(list, "absent"), -1);

    remove_list(&list);
}


TEST(StringRemove, RemovesElement)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "a");
    string_add(list, "b");
    string_add(list, "c");

    string_remove(list, "b");

    EXPECT_EQ(get_size(list), 2);
    EXPECT_STREQ(list[0], "a");
    EXPECT_STREQ(list[1], "c");

    remove_list(&list);
}


TEST(RemoveDuplicates, RemovesAllDuplicates)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "x");
    string_add(list, "y");
    string_add(list, "x");
    string_add(list, "x");

    remove_duplicates(list);

    EXPECT_EQ(get_size(list), 2);
    EXPECT_STREQ(list[0], "x");
    EXPECT_STREQ(list[1], "y");

    remove_list(&list);
}

TEST(ReplaceString, ReplacesSubstring)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "hello world");

    replace_string(list, "world", "C++");

    EXPECT_STREQ(list[0], "hello C++");

    remove_list(&list);
}

TEST(SortList, SortsAlphabetically)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "banana");
    string_add(list, "apple");
    string_add(list, "cherry");

    sort_list(list);

    EXPECT_STREQ(list[0], "apple");
    EXPECT_STREQ(list[1], "banana");
    EXPECT_STREQ(list[2], "cherry");

    remove_list(&list);
}

TEST(ListInit, HandlesDoubleInitGracefully)
{
    char** list = nullptr;
    list_init(&list);

    list_init(&list);

    EXPECT_NE(list, nullptr);
    EXPECT_EQ(get_size(list), 0);

    remove_list(&list);
}

TEST(StringAdd, HandlesNullAndEmptyStrings)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "");
    EXPECT_EQ(get_size(list), 1);
    EXPECT_STREQ(list[0], "");


    string_add(list, "   ");
    EXPECT_STREQ(list[1], "   ");

    remove_list(&list);
}

TEST(StringAdd, HandlesVeryLongStrings)
{
    char** list = nullptr;
    list_init(&list);

    const char* long_str = "This is a very long string that should be handled properly "
        "without any buffer overflow issues or memory problems.";

    string_add(list, long_str);

    EXPECT_EQ(get_size(list), 1);
    EXPECT_STREQ(list[0], long_str);

    remove_list(&list);
}

TEST(ReturnIndex, CaseSensitiveSearch)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "Hello");
    string_add(list, "WORLD");
    string_add(list, "test");

    EXPECT_EQ(return_index_str(list, "hello"), -1);
    EXPECT_EQ(return_index_str(list, "Hello"), 0);
    EXPECT_EQ(return_index_str(list, "WORLD"), 1);
    EXPECT_EQ(return_index_str(list, "world"), -1);

    remove_list(&list);
}

TEST(ReplaceString, BasicReplacement)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "hello world");
    string_add(list, "goodbye world");

    replace_string(list, "world", "universe");

    EXPECT_EQ(get_size(list), 2);
    if (get_size(list) > 1) {
        EXPECT_STREQ(list[0], "hello universe");
        EXPECT_STREQ(list[1], "goodbye universe");
    }

    remove_list(&list);
}

TEST(SortList, SortsReverseOrder)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "cherry");
    string_add(list, "banana");
    string_add(list, "apple");

    sort_list(list);

    EXPECT_EQ(get_size(list), 3);
    if (get_size(list) > 2) {
        EXPECT_STREQ(list[0], "apple");
        EXPECT_STREQ(list[1], "banana");
        EXPECT_STREQ(list[2], "cherry");
    }

    remove_list(&list);
}

TEST(SortList, SortsAlreadySorted)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "apple");
    string_add(list, "banana");
    string_add(list, "cherry");

    sort_list(list);

    EXPECT_EQ(get_size(list), 3);
    if (get_size(list) > 2) {
        EXPECT_STREQ(list[0], "apple");
        EXPECT_STREQ(list[1], "banana");
        EXPECT_STREQ(list[2], "cherry");
    }

    remove_list(&list);
}

TEST(ListMemory, RemovesEmptyList)
{
    char** list = nullptr;
    list_init(&list);

    remove_list(&list);

    EXPECT_EQ(list, nullptr);
}

TEST(RemoveDuplicates, RemovesConsecutiveDuplicates)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "x");
    string_add(list, "x");
    string_add(list, "y");
    string_add(list, "y");
    string_add(list, "z");

    remove_duplicates(list);

    EXPECT_EQ(get_size(list), 3);
    if (get_size(list) > 2) {
        EXPECT_STREQ(list[0], "x");
        EXPECT_STREQ(list[1], "y");
        EXPECT_STREQ(list[2], "z");
    }

    remove_list(&list);
}

TEST(StringRemove, RemovesLastElement)
{
    char** list = nullptr;
    list_init(&list);

    string_add(list, "a");
    string_add(list, "b");
    string_add(list, "c");

    string_remove(list, "c");

    EXPECT_EQ(get_size(list), 2);
    if (get_size(list) > 1) {
        EXPECT_STREQ(list[0], "a");
        EXPECT_STREQ(list[1], "b");
    }

    remove_list(&list);
}