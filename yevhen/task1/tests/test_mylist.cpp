#include <gtest/gtest.h>
#include "../src/mylist.h"
#include <string.h>

class StringListTest : public testing::Test {
protected:
    char **list;

    void SetUp() override {
        StringListInit(&list);
    }

    void TearDown() override {
        StringListDestroy(&list);
    }
};

TEST_F(StringListTest, AddString) {
    EXPECT_EQ(0, StringListSize(&list));

    StringListAdd(&list, "one");
    EXPECT_EQ(1, StringListSize(&list));

    StringListAdd(&list, "two");
    StringListAdd(&list, "three");
    EXPECT_EQ(3, StringListSize(&list));
}

TEST_F(StringListTest, IndexOfString) {
    StringListAdd(&list, "apple");
    StringListAdd(&list, "banana");
    StringListAdd(&list, "cherry");

    EXPECT_EQ(0, StringListIndexOf(&list, "apple"));
    EXPECT_EQ(1, StringListIndexOf(&list, "banana"));
    EXPECT_EQ(2, StringListIndexOf(&list, "cherry"));
    EXPECT_EQ(-1, StringListIndexOf(&list, "notexist"));
}

TEST_F(StringListTest, RemoveString) {
    StringListAdd(&list, "x");
    StringListAdd(&list, "y");
    StringListAdd(&list, "x");

    StringListRemove(&list, "x");
    EXPECT_EQ(1, StringListSize(&list));
    EXPECT_EQ(0, StringListIndexOf(&list, "y"));
    EXPECT_EQ(-1, StringListIndexOf(&list, "x"));
}

TEST_F(StringListTest, RemoveDuplicatesStrings) {
    StringListAdd(&list, "a");
    StringListAdd(&list, "b");
    StringListAdd(&list, "a");
    StringListAdd(&list, "c");

    StringListRemoveDuplicates(&list);
    EXPECT_EQ(2, StringListSize(&list));
    EXPECT_EQ(0, StringListIndexOf(&list, "a"));
    EXPECT_EQ(1, StringListIndexOf(&list, "b"));
    EXPECT_EQ(2, StringListIndexOf(&list, "c"));
}

TEST_F(StringListTest, ReplaceSubstring) {
    StringListAdd(&list, "hello world");
    StringListAdd(&list, "world hello");

    StringListReplaceInStrings(&list, "world", "test");

    EXPECT_STREQ("hello test", list[0]);
    EXPECT_STREQ("test hello", list[1]);
}

TEST_F(StringListTest, Sort) {
    StringListAdd(&list, "banana");
    StringListAdd(&list, "apple");
    StringListAdd(&list, "cherry");

    StringListSort(&list);

    EXPECT_STREQ("apple", list[0]);
    EXPECT_STREQ("banana", list[1]);
    EXPECT_STREQ("cherry", list[2]);
}