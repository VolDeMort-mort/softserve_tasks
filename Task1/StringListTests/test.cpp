#include "pch.h"
#include "../StringListLib/string_list.h"

TEST(StringListTests, InitEmptyList) {
    char** list;

    string_list_init(&list);

    EXPECT_NE(list, nullptr);
    EXPECT_EQ(list[0], nullptr);

    string_list_destroy(&list);
}

TEST(StringListTests, AddIncreasesSize) {
    char** list;

    string_list_init(&list);

    string_list_add(&list, "hello");
    string_list_add(&list, "world");

    EXPECT_EQ(string_list_size(list), 2);

    string_list_destroy(&list);
}

TEST(StringListTests, IndexOfWorksCorrectly) {
    char** list;

    string_list_init(&list);

    string_list_add(&list, "hello");
    string_list_add(&list, "world");

    EXPECT_EQ(string_list_index_of(list, "hello"), 0);
    EXPECT_EQ(string_list_index_of(list, "world"), 1);
    EXPECT_EQ(string_list_index_of(list, "cat"), -1);

    string_list_destroy(&list);
}

TEST(StringListTests, RemoveDeletesElement) {
    char** list = nullptr;
    string_list_init(&list);

    string_list_add(&list, (char*)"one");
    string_list_add(&list, (char*)"two");
    string_list_add(&list, (char*)"three");

    string_list_remove(&list, (char*)"two");

    EXPECT_EQ(string_list_size(list), 2);
    EXPECT_EQ(string_list_index_of(list, (char*)"one"), 0);
    EXPECT_EQ(string_list_index_of(list, (char*)"two"), -1);
    EXPECT_EQ(string_list_index_of(list, (char*)"three"), 1);

    string_list_destroy(&list);
}

TEST(StringListTests, RemoveNonExistingElement) {
    char** list = nullptr;
    string_list_init(&list);

    string_list_add(&list, "one");
    string_list_add(&list, "two");

    string_list_remove(&list, "three");

    EXPECT_EQ(string_list_size(list), 2);

    string_list_destroy(&list);
}

TEST(StringListTests, DestroySetsListToNull) {
    char** list = nullptr;
    string_list_init(&list);

    string_list_add(&list, (char*)"test");
    string_list_destroy(&list);

    EXPECT_EQ(list, nullptr);
}

TEST(StringListTests, RemovesDuplicates) {
    char** list = nullptr;
    string_list_init(&list);

    string_list_add(&list, "a");
    string_list_add(&list, "b");
    string_list_add(&list, "a");
    string_list_add(&list, "c");
    string_list_add(&list, "d");
    string_list_add(&list, "a");

    string_list_remove_duplicates(&list);

    EXPECT_EQ(string_list_size(list), 4);
    EXPECT_EQ(string_list_index_of(list, "a"), 0);
    EXPECT_EQ(string_list_index_of(list, "b"), 1);
    EXPECT_EQ(string_list_index_of(list, "c"), 2);
    EXPECT_EQ(string_list_index_of(list, "d"), 3);

    string_list_destroy(&list);
}

TEST(StringListTests, ReplaceInStrings) {
    char** list = nullptr;
    string_list_init(&list);

    string_list_add(&list, "a");
    string_list_add(&list, "b");
    string_list_add(&list, "a");
    string_list_add(&list, "c");
    string_list_add(&list, "d");
    string_list_add(&list, "a");

    string_list_replace_in_strings(list, "a", "1");

    EXPECT_EQ(string_list_size(list), 6);
    EXPECT_STREQ(list[0], "1");
    EXPECT_STREQ(list[1], "b");
    EXPECT_STREQ(list[2], "1");
    EXPECT_STREQ(list[3], "c");
    EXPECT_STREQ(list[4], "d");
    EXPECT_STREQ(list[5], "1");

    string_list_destroy(&list);
}

TEST(StringListTests, SortsLexicographically) {
    char** list = nullptr;
    string_list_init(&list);

    string_list_add(&list, (char*)"banana");
    string_list_add(&list, (char*)"apple");
    string_list_add(&list, (char*)"cherry");

    string_list_sort(list);

    EXPECT_EQ(string_list_index_of(list, (char*)"apple"), 0);
    EXPECT_EQ(string_list_index_of(list, (char*)"banana"), 1);
    EXPECT_EQ(string_list_index_of(list, (char*)"cherry"), 2);

    string_list_destroy(&list);
}