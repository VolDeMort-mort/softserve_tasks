#include <gtest/gtest.h>
#include "CStringList.h"

static char** CreateList()
{
    char** list = nullptr;
    StringListInit(&list);
    return list;
}

static void DestroyList(char**& list)
{
    StringListDestroy(&list);
}

TEST(StringList, Init)
{
    char** list = nullptr;
    StringListInit(&list);

    ASSERT_NE(list, nullptr);
    EXPECT_EQ(StringListSize(list), 0);

    DestroyList(list);
}

TEST(StringList, AddAndSize)
{
    char** list = CreateList();

    StringListAdd(&list, (char*)"one");
    StringListAdd(&list, (char*)"two");
    StringListAdd(&list, (char*)"three");

    EXPECT_EQ(StringListSize(list), 3);

    DestroyList(list);
}

TEST(StringList, IndexOf)
{
    char** list = CreateList();

    StringListAdd(&list, (char*)"apple");
    StringListAdd(&list, (char*)"banana");
    StringListAdd(&list, (char*)"cherry");

    EXPECT_EQ(StringListIndexOf(list, (char*)"banana"), 1);
    EXPECT_EQ(StringListIndexOf(list, (char*)"missing"), -1);

    DestroyList(list);
}

TEST(StringList, Remove)
{
    char** list = CreateList();

    StringListAdd(&list, (char*)"dup");
    StringListAdd(&list, (char*)"keep");
    StringListAdd(&list, (char*)"dup");
    StringListAdd(&list, (char*)"dup");

    StringListRemove(list, (char*)"dup");

    EXPECT_EQ(StringListSize(list), 1);
    EXPECT_STREQ(list[0], "keep");

    DestroyList(list);
}

TEST(StringList, RemoveDuplicates)
{
    char** list = CreateList();

    StringListAdd(&list, (char*)"a");
    StringListAdd(&list, (char*)"b");
    StringListAdd(&list, (char*)"a");
    StringListAdd(&list, (char*)"c");
    StringListAdd(&list, (char*)"b");

    StringListRemoveDuplicates(list);

    EXPECT_EQ(StringListSize(list), 3);
    EXPECT_NE(StringListIndexOf(list, (char*)"a"), -1);
    EXPECT_NE(StringListIndexOf(list, (char*)"b"), -1);
    EXPECT_NE(StringListIndexOf(list, (char*)"c"), -1);

    DestroyList(list);
}

TEST(StringList, ReplaceInStrings)
{
    char** list = CreateList();

    StringListAdd(&list, (char*)"hello world");
    StringListAdd(&list, (char*)"world world");

    StringListReplaceInStrings(list, (char*)"world", (char*)"there");

    EXPECT_STREQ(list[0], "hello there");
    EXPECT_STREQ(list[1], "there there");

    DestroyList(list);
}

TEST(StringList, Sort)
{
    char** list = CreateList();

    StringListAdd(&list, (char*)"delta");
    StringListAdd(&list, (char*)"alpha");
    StringListAdd(&list, (char*)"charlie");
    StringListAdd(&list, (char*)"bravo");

    StringListSort(list);

    EXPECT_STREQ(list[0], "alpha");
    EXPECT_STREQ(list[1], "bravo");
    EXPECT_STREQ(list[2], "charlie");
    EXPECT_STREQ(list[3], "delta");

    DestroyList(list);
}

TEST(StringList, EmptyListSafety)
{
    char** list = CreateList();

    EXPECT_EQ(StringListSize(list), 0);
    EXPECT_EQ(StringListIndexOf(list, (char*)"x"), -1);

    StringListRemove(list, (char*)"x");
    StringListRemoveDuplicates(list);
    StringListSort(list);

    EXPECT_EQ(StringListSize(list), 0);

    DestroyList(list);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
