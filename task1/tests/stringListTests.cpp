#include <gtest/gtest.h>
extern "C" {
#include "StringList.h"
}

class StringListTest : public ::testing::Test {
protected:
    char** list;

    void SetUp() override {
        stringListInit(&list);
    }

    void TearDown() override {
        stringListDestroy(&list);
    }
};

TEST_F(StringListTest, GivenEmptyList_WhenInit_ThenSizeIsZero) {
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(stringListSize(list), 0);
}

TEST_F(StringListTest, GivenList_WhenDestroyCalledTwice_ThenNoCrash) {
    stringListDestroy(&list);
    EXPECT_EQ(list, nullptr);
    stringListDestroy(&list);
    EXPECT_EQ(list, nullptr);
}

TEST_F(StringListTest, GivenEmptyList_WhenAddString_ThenSizeAndContentCorrect) {
    stringListAdd(&list, "red");
    EXPECT_EQ(stringListSize(list), 1);
    EXPECT_STREQ(list[0], "red");
}

TEST_F(StringListTest, GivenList_WhenAddMultipleStrings_ThenOrderPreserved) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "green");
    stringListAdd(&list, "blue");
    EXPECT_EQ(stringListSize(list), 3);
    EXPECT_STREQ(list[0], "red");
    EXPECT_STREQ(list[1], "green");
    EXPECT_STREQ(list[2], "blue");
}

TEST_F(StringListTest, GivenListWithStrings_WhenRemoveMiddle_ThenRemainingShiftDown) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "green");
    stringListAdd(&list, "blue");

    stringListRemove(&list, "green");
    EXPECT_EQ(stringListSize(list), 2);
    EXPECT_STREQ(list[0], "red");
    EXPECT_STREQ(list[1], "blue");
}

TEST_F(StringListTest, GivenEmptyList_WhenRemove_ThenNoCrash) {
    stringListRemove(&list, "red");
    EXPECT_EQ(stringListSize(list), 0);
}

TEST_F(StringListTest, GivenList_WhenRemoveNonexistent_ThenListUnchanged) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "green");
    stringListRemove(&list, "yellow");
    EXPECT_EQ(stringListSize(list), 2);
    EXPECT_STREQ(list[0], "red");
    EXPECT_STREQ(list[1], "green");
}

TEST_F(StringListTest, GivenListWithDuplicates_WhenRemove_ThenAllOccurrencesRemoved) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "green");
    stringListAdd(&list, "red");
    stringListAdd(&list, "blue");
    stringListAdd(&list, "red");

    stringListRemove(&list, "red");
    EXPECT_EQ(stringListSize(list), 2);
    EXPECT_STREQ(list[0], "green");
    EXPECT_STREQ(list[1], "blue");
}

TEST_F(StringListTest, GivenListWithStrings_WhenIndexOf_ThenReturnsCorrectIndex) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "green");
    stringListAdd(&list, "blue");

    EXPECT_EQ(stringListIndexOf(list, "red"), 0);
    EXPECT_EQ(stringListIndexOf(list, "green"), 1);
    EXPECT_EQ(stringListIndexOf(list, "blue"), 2);
    EXPECT_EQ(stringListIndexOf(list, "yellow"), -1);
}

TEST_F(StringListTest, GivenEmptyList_WhenIndexOf_ThenReturnsMinusOne) {
    EXPECT_EQ(stringListIndexOf(list, "red"), -1);
}

TEST_F(StringListTest, GivenListWithDuplicates_WhenRemoveDuplicates_ThenOnlyFirstOccurrenceKept) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "green");
    stringListAdd(&list, "red");
    stringListAdd(&list, "blue");
    stringListAdd(&list, "green");

    stringListRemoveDuplicates(&list);
    EXPECT_EQ(stringListSize(list), 3);
    EXPECT_STREQ(list[0], "red");
    EXPECT_STREQ(list[1], "green");
    EXPECT_STREQ(list[2], "blue");
}

TEST_F(StringListTest, GivenListWithAllIdentical_WhenRemoveDuplicates_ThenOneRemains) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "red");
    stringListAdd(&list, "red");

    stringListRemoveDuplicates(&list);
    EXPECT_EQ(stringListSize(list), 1);
    EXPECT_STREQ(list[0], "red");
}

TEST_F(StringListTest, GivenListWithStrings_WhenReplace_ThenAllStringsUpdated) {
    stringListAdd(&list, "dark red");
    stringListAdd(&list, "dark blue");
    stringListAdd(&list, "light green");

    stringListReplaceInStrings(list, "dark", "light");
    EXPECT_STREQ(list[0], "light red");
    EXPECT_STREQ(list[1], "light blue");
    EXPECT_STREQ(list[2], "light green");
}

TEST_F(StringListTest, GivenList_WhenReplaceNonexistentPattern_ThenUnchanged) {
    stringListAdd(&list, "red green");
    stringListReplaceInStrings(list, "yellow", "purple");
    EXPECT_STREQ(list[0], "red green");
}

TEST_F(StringListTest, GivenUnsortedList_WhenSort_ThenAlphabeticalOrder) {
    stringListAdd(&list, "red");
    stringListAdd(&list, "blue");
    stringListAdd(&list, "green");
    stringListAdd(&list, "emerald");

    stringListSort(list);
    EXPECT_STREQ(list[0], "blue");
    EXPECT_STREQ(list[1], "emerald");
    EXPECT_STREQ(list[2], "green");
    EXPECT_STREQ(list[3], "red");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
