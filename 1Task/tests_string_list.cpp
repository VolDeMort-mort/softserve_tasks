#include "string_list.h"
#include <gtest/gtest.h>
#include <cstring>

class StringListTest : public ::testing::Test {
protected:
    char** list = nullptr;

    void SetUp() override { ListCreate(list); }
    void TearDown() override { ListDestroy(list); }
};


TEST_F(StringListTest, InitiallyEmpty) {
    ASSERT_NE(list, nullptr);
    EXPECT_EQ(ListSize(list), 0);
}


TEST_F(StringListTest, AddAndSize) {
    ListAdd(list, "one");
    ListAdd(list, "two");
    ListAdd(list, "three");

    EXPECT_EQ(ListSize(list), 3);
    EXPECT_STREQ(list[1], "one");
    EXPECT_STREQ(list[2], "two");
    EXPECT_STREQ(list[3], "three");
}


TEST_F(StringListTest, FindIndex) {
    ListAdd(list, "apple");
    ListAdd(list, "banana");
    ListAdd(list, "orange");

    EXPECT_EQ(ListFindIndex(list, "apple"), 1);
    EXPECT_EQ(ListFindIndex(list, "banana"), 2);
    EXPECT_EQ(ListFindIndex(list, "orange"), 3);
    EXPECT_EQ(ListFindIndex(list, "kiwi"), -1);
}


TEST_F(StringListTest, EraseValue) {
    ListAdd(list, "apple");
    ListAdd(list, "banana");
    ListAdd(list, "apple");
    ListAdd(list, "orange");

    ListEraseValue(list, "apple");

    EXPECT_EQ(ListSize(list), 2);
    EXPECT_STREQ(list[1], "banana");
    EXPECT_STREQ(list[2], "orange");
}


TEST_F(StringListTest, RemoveDups) {
    ListAdd(list, "a");
    ListAdd(list, "b");
    ListAdd(list, "a");
    ListAdd(list, "c");
    ListAdd(list, "b");

    ListRemoveDups(list);

    EXPECT_EQ(ListSize(list), 3);
    EXPECT_STREQ(list[1], "a");
    EXPECT_STREQ(list[2], "b");
    EXPECT_STREQ(list[3], "c");
}


TEST_F(StringListTest, Sort) {
    ListAdd(list, "banana");
    ListAdd(list, "apple");
    ListAdd(list, "orange");

    ListSort(list);

    EXPECT_STREQ(list[1], "apple");
    EXPECT_STREQ(list[2], "banana");
    EXPECT_STREQ(list[3], "orange");
}


TEST_F(StringListTest, ReplaceInStrings) {
    ListAdd(list, "one two one");
    ListAdd(list, "one");
    ListAdd(list, "three");

    StringListReplaceInStrings(list, "one", "seven");

    EXPECT_STREQ(list[1], "seven two seven");
    EXPECT_STREQ(list[2], "seven");
    EXPECT_STREQ(list[3], "three");
}


TEST_F(StringListTest, EdgeCasesOnEmptyList) {
    EXPECT_EQ(ListFindIndex(list, "x"), -1);

    ListEraseValue(list, "nothing");
    EXPECT_EQ(ListSize(list), 0);

    ListRemoveDups(list);
    EXPECT_EQ(ListSize(list), 0);

    ListSort(list);
    EXPECT_EQ(ListSize(list), 0);

    StringListReplaceInStrings(list, "a", "b");
    EXPECT_EQ(ListSize(list), 0);
}


TEST_F(StringListTest, ManyAddsReallocCheck) {
    for (int i = 0; i < 1000; ++i)
        ListAdd(list, "x");

    EXPECT_EQ(ListSize(list), 1000);
}

