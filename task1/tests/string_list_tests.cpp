#include <gtest/gtest.h>
#include "string_list.h"

// init test
TEST(StringListTest, InitializesEmpty) {
    char** list = nullptr;
    StringListInit(&list);

    ASSERT_NE(list, nullptr);
    EXPECT_EQ(StringListSize(&list), 0);

    StringListDestroy(&list);
    EXPECT_EQ(list, nullptr);
}

// add str test
TEST(StringListTest, AddsElementsCorrectly) {
    char** list = nullptr;
    StringListInit(&list);

    StringListAdd(&list, "Hello");
    StringListAdd(&list, "World");

    EXPECT_EQ(StringListSize(&list), 2);
    EXPECT_STREQ(list[0], "Hello");
    EXPECT_STREQ(list[1], "World");

    StringListDestroy(&list);
}

// duplicating test
TEST(StringListTest, RemovesDuplicates) {
    char** list = nullptr;
    StringListInit(&list);

    StringListAdd(&list, "A");
    StringListAdd(&list, "B");
    StringListAdd(&list, "A");

    StringListRemoveDuplicates(&list);

    EXPECT_EQ(StringListSize(&list), 2);
    EXPECT_STREQ(list[0], "A");
    EXPECT_STREQ(list[1], "B");

    StringListDestroy(&list);
}

// sort test
TEST(StringListTest, SortsElements) {
    char** list = nullptr;
    StringListInit(&list);

    StringListAdd(&list, "C");
    StringListAdd(&list, "A");
    StringListAdd(&list, "B");

    StringListSort(&list);

    EXPECT_STREQ(list[0], "A");
    EXPECT_STREQ(list[1], "B");
    EXPECT_STREQ(list[2], "C");

    StringListDestroy(&list);
}

// add more than initial capacity
TEST(StringListTest, ExpandsCapacity) {
    char** list = nullptr;
    StringListInit(&list);

    for (int i = 0; i < 15; i++) {
        StringListAdd(&list, "test_item");
    }

    EXPECT_EQ(StringListSize(&list), 15);
    EXPECT_STREQ(list[14], "test_item");

    StringListDestroy(&list);
}
// trying delete smth that doesnt exist
TEST(StringListTest, RemoveNonExistent) {
    char** list = nullptr;
    StringListInit(&list);
    StringListAdd(&list, "Exist");

    StringListRemove(&list, "NonExist");

    EXPECT_EQ(StringListSize(&list), 1);
    StringListDestroy(&list);
}

TEST(StringListTest, ReplacesStringsCorrectly) {
    char** list = nullptr;
    StringListInit(&list);

    // replace with longer word
    StringListAdd(&list, "I like apple and another apple");
    StringListReplaceInStrings(&list, "apple", "pineapple");

    EXPECT_STREQ(list[0], "I like pineapple and another pineapple");

    // replace with shorter word
    StringListReplaceInStrings(&list, "pineapple", "pin");
    EXPECT_STREQ(list[0], "I like pin and another pin");

    // replace non-existent word
    StringListReplaceInStrings(&list, "ghost", "void");
    EXPECT_STREQ(list[0], "I like pin and another pin");

    StringListDestroy(&list);
}