#include <gtest/gtest.h>
#include "StringList.h"


TEST(StringListBasic, InitializesMetaData) {
     char** list = nullptr;
     StringListInit(&list);

     ASSERT_NE(list, nullptr);
     EXPECT_EQ(StringListSize(list), 0);

     StringListDestroy(&list);
}

TEST(StringListBasic, AddsElements) {
     char** list = nullptr;
     StringListInit(&list);

     StringListAdd(&list, (char*)"Hello");
     StringListAdd(&list, (char*)"World");

     EXPECT_EQ(StringListSize(list), 2);
     EXPECT_STREQ(list[0], "Hello");
     EXPECT_STREQ(list[1], "World");

     StringListDestroy(&list);
}

TEST(StringListBasic, FindsIndexByString) {
     char** list = nullptr;
     StringListInit(&list);

     StringListAdd(&list, (char*)"Apple");
     StringListAdd(&list, (char*)"Banana");

     EXPECT_EQ(StringListIndexOf(list, (char*)"Banana"), 1);
     EXPECT_EQ(StringListIndexOf(list, (char*)"Cherry"), -1);

     StringListDestroy(&list);
}

TEST(StringListModification, RemovesDuplicates) {
     char** list = nullptr;
     StringListInit(&list);

     StringListAdd(&list, (char*)"A");
     StringListAdd(&list, (char*)"B");
     StringListAdd(&list, (char*)"A");
     StringListAdd(&list, (char*)"C");
     StringListAdd(&list, (char*)"A");

     StringListRemoveDuplicates(list);

     EXPECT_EQ(StringListSize(list), 3);
     EXPECT_STREQ(list[0], "A");
     EXPECT_STREQ(list[1], "B");
     EXPECT_STREQ(list[2], "C");

     StringListDestroy(&list);
}

TEST(StringListModification, RemovesString) {
     char** list = nullptr;
     StringListInit(&list);

     StringListAdd(&list, (char*)"Keep");
     StringListAdd(&list, (char*)"Delete");
     StringListAdd(&list, (char*)"Keep");

     StringListRemove(list, (char*)"Delete");

     EXPECT_EQ(StringListSize(list), 2);
     EXPECT_STREQ(list[1], "Keep");

     StringListDestroy(&list);
}

TEST(StringListModification, ReplacesSubstringInStrings) {
     char** list = nullptr;
     StringListInit(&list);

     StringListAdd(&list, (char*)"cat-bat-cat");
     StringListReplaceInStrings(list, (char*)"cat", (char*)"elephant");

     EXPECT_STREQ(list[0], "elephant-bat-elephant");

     StringListDestroy(&list);
}

TEST(StringListSorting, SortsAlphabetically) {
     char** list = nullptr;
     StringListInit(&list);

     StringListAdd(&list, (char*)"Zebra");
     StringListAdd(&list, (char*)"Apple");
     StringListAdd(&list, (char*)"Mango");

     StringListSort(list);

     EXPECT_STREQ(list[0], "Apple");
     EXPECT_STREQ(list[1], "Mango");
     EXPECT_STREQ(list[2], "Zebra");

     StringListDestroy(&list);
}

int main(int argc, char** argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}