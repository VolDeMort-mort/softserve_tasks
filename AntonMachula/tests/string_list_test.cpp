#include <cstring>
#include <gtest/gtest.h>

#include "string_list.h"

class StringListTest : public ::testing::Test {
protected:
  char **list = nullptr;

  void SetUp() override { StringListInit(&list); }

  void TearDown() override { StringListDestroy(&list); }
};

TEST_F(StringListTest, InitCreatesEmptyList) {
  EXPECT_EQ(StringListSize(list), 0);
}

TEST_F(StringListTest, AddIncreasesSize) {
  StringListAdd(&list, "hello");
  EXPECT_EQ(StringListSize(list), 1);
}

TEST_F(StringListTest, AddMultipleStrings) {
  StringListAdd(&list, "one");
  StringListAdd(&list, "two");
  StringListAdd(&list, "three");
  EXPECT_EQ(StringListSize(list), 3);
}

TEST_F(StringListTest, IndexOfFindsString) {
  StringListAdd(&list, "apple");
  StringListAdd(&list, "banana");
  StringListAdd(&list, "cherry");
  EXPECT_EQ(StringListIndexOf(list, "banana"), 1);
}

TEST_F(StringListTest, IndexOfReturnsMinusOneForMissing) {
  StringListAdd(&list, "apple");
  EXPECT_EQ(StringListIndexOf(list, "orange"), -1);
}

TEST_F(StringListTest, RemoveDeletesAllOccurrences) {
  StringListAdd(&list, "apple");
  StringListAdd(&list, "banana");
  StringListAdd(&list, "apple");
  StringListRemove(&list, "apple");
  EXPECT_EQ(StringListSize(list), 1);
  EXPECT_EQ(StringListIndexOf(list, "apple"), -1);
}

TEST_F(StringListTest, RemoveDuplicatesKeepsUnique) {
  StringListAdd(&list, "apple");
  StringListAdd(&list, "banana");
  StringListAdd(&list, "apple");
  StringListAdd(&list, "banana");
  StringListRemoveDuplicates(&list);
  EXPECT_EQ(StringListSize(list), 2);
}

TEST_F(StringListTest, ReplaceInStrings) {
  StringListAdd(&list, "hello world");
  StringListAdd(&list, "world peace");
  StringListReplaceInStrings(list, "world", "planet");
  EXPECT_EQ(StringListIndexOf(list, "hello planet"), 0);
  EXPECT_EQ(StringListIndexOf(list, "planet peace"), 1);
}

TEST_F(StringListTest, SortOrdersAlphabetically) {
  StringListAdd(&list, "cherry");
  StringListAdd(&list, "apple");
  StringListAdd(&list, "banana");
  StringListSort(list);
  EXPECT_EQ(StringListIndexOf(list, "apple"), 0);
  EXPECT_EQ(StringListIndexOf(list, "banana"), 1);
  EXPECT_EQ(StringListIndexOf(list, "cherry"), 2);
}

TEST_F(StringListTest, DestroySetsPointerToNull) {
  char **local_list = nullptr;
  StringListInit(&local_list);
  StringListAdd(&local_list, "test");
  StringListDestroy(&local_list);
  EXPECT_EQ(local_list, nullptr);
}

TEST_F(StringListTest, AddTriggersReallocation) {
  for (int i = 0; i < 10; i++) {
    StringListAdd(&list, "item");
  }
  EXPECT_EQ(StringListSize(list), 10);
}

TEST_F(StringListTest, AddCopiesStringContent) {
  char buffer[32] = "original";
  StringListAdd(&list, buffer);

  strcpy(buffer, "modified");

  EXPECT_STREQ(list[0], "original");
}

TEST_F(StringListTest, AddPreservesExistingStringsAfterRealloc) {
  StringListAdd(&list, "first");
  StringListAdd(&list, "second");
  StringListAdd(&list, "third");
  StringListAdd(&list, "fourth");
  StringListAdd(&list, "fifth");

  EXPECT_STREQ(list[0], "first");
  EXPECT_STREQ(list[1], "second");
  EXPECT_STREQ(list[2], "third");
  EXPECT_STREQ(list[3], "fourth");
  EXPECT_STREQ(list[4], "fifth");
}


TEST_F(StringListTest, RemoveNonExistentStringDoesNothing) {
  StringListAdd(&list, "apple");
  StringListAdd(&list, "banana");
  StringListRemove(&list, "cherry");
  EXPECT_EQ(StringListSize(list), 2);
  EXPECT_STREQ(list[0], "apple");
  EXPECT_STREQ(list[1], "banana");
}

TEST_F(StringListTest, RemoveFromEmptyListDoesNothing) {
  StringListRemove(&list, "anything");
  EXPECT_EQ(StringListSize(list), 0);
}

TEST_F(StringListTest, RemoveAllElementsLeavesEmptyList) {
  StringListAdd(&list, "same");
  StringListAdd(&list, "same");
  StringListAdd(&list, "same");
  StringListRemove(&list, "same");
  EXPECT_EQ(StringListSize(list), 0);
}

TEST_F(StringListTest, RemovePreservesOrderOfRemainingElements) {
  StringListAdd(&list, "a");
  StringListAdd(&list, "remove_me");
  StringListAdd(&list, "b");
  StringListAdd(&list, "remove_me");
  StringListAdd(&list, "c");
  StringListRemove(&list, "remove_me");
  EXPECT_EQ(StringListSize(list), 3);
  EXPECT_STREQ(list[0], "a");
  EXPECT_STREQ(list[1], "b");
  EXPECT_STREQ(list[2], "c");
}


TEST_F(StringListTest, RemoveDuplicatesKeepsFirstOccurrence) {
  StringListAdd(&list, "first");
  StringListAdd(&list, "second");
  StringListAdd(&list, "first");
  StringListRemoveDuplicates(&list);
  EXPECT_EQ(StringListSize(list), 2);
  EXPECT_STREQ(list[0], "first");
  EXPECT_STREQ(list[1], "second");
}

TEST_F(StringListTest, RemoveDuplicatesOnUniqueListDoesNothing) {
  StringListAdd(&list, "a");
  StringListAdd(&list, "b");
  StringListAdd(&list, "c");
  StringListRemoveDuplicates(&list);
  EXPECT_EQ(StringListSize(list), 3);
}


TEST_F(StringListTest, ReplaceWithLongerString) {
  StringListAdd(&list, "hi there");
  StringListReplaceInStrings(list, "hi", "hello");
  EXPECT_STREQ(list[0], "hello there");
}

TEST_F(StringListTest, ReplaceWithShorterString) {
  StringListAdd(&list, "hello there");
  StringListReplaceInStrings(list, "hello", "hi");
  EXPECT_STREQ(list[0], "hi there");
}

TEST_F(StringListTest, ReplaceMultipleOccurrences) {
  StringListAdd(&list, "ab ab ab");
  StringListReplaceInStrings(list, "ab", "X");
  EXPECT_STREQ(list[0], "X X X");
}

TEST_F(StringListTest, ReplaceNoMatchDoesNothing) {
  StringListAdd(&list, "hello");
  StringListReplaceInStrings(list, "xyz", "abc");
  EXPECT_STREQ(list[0], "hello");
}


TEST_F(StringListTest, SortEmptyList) {
  StringListSort(list);
  EXPECT_EQ(StringListSize(list), 0);
}

TEST_F(StringListTest, SortSingleElement) {
  StringListAdd(&list, "only");
  StringListSort(list);
  EXPECT_STREQ(list[0], "only");
}

TEST_F(StringListTest, SortAlreadySorted) {
  StringListAdd(&list, "a");
  StringListAdd(&list, "b");
  StringListAdd(&list, "c");
  StringListSort(list);
  EXPECT_STREQ(list[0], "a");
  EXPECT_STREQ(list[1], "b");
  EXPECT_STREQ(list[2], "c");
}

TEST_F(StringListTest, SortReversed) {
  StringListAdd(&list, "c");
  StringListAdd(&list, "b");
  StringListAdd(&list, "a");
  StringListSort(list);
  EXPECT_STREQ(list[0], "a");
  EXPECT_STREQ(list[1], "b");
  EXPECT_STREQ(list[2], "c");
}
