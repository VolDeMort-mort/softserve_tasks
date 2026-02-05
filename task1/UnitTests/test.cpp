#include "pch.h"
#include "../task1_lib/dynamicStringList.h"

struct TestDynList : public::testing::Test {   
    char** list;

    void SetUp() override {
        list = NULL;
        StringListInit(&list);
    }

    void TearDown() override {
        StringListDestroy(&list);
    }
};

TEST_F(TestDynList, TestInit) {
    ASSERT_NE(list, nullptr);
    EXPECT_EQ(StringListSize(list), 0);
    EXPECT_GE(StringListCapacity(list), 10);
}

TEST_F(TestDynList, TestAdd) {
    StringListAdd(&list, "hello");
    EXPECT_EQ(StringListSize(list), 1);
    EXPECT_STREQ(list[0], "hello");

    StringListAdd(&list, "world");
    EXPECT_EQ(StringListSize(list), 2);
    EXPECT_STREQ(list[1], "world");
}

TEST_F(TestDynList, TestIndexOf) {
    StringListAdd(&list, "a");
    StringListAdd(&list, "b");
    StringListAdd(&list, "a");
    EXPECT_EQ(StringListIndexOf(list, "a"), 0);
    EXPECT_EQ(StringListIndexOf(list, "b"), 1);
    EXPECT_EQ(StringListIndexOf(list, "c"), -1);
}

TEST_F(TestDynList, TestRemoveAll) {
    StringListAdd(&list, "a");
    StringListAdd(&list, "b");
    StringListAdd(&list, "a");
    EXPECT_EQ(StringListSize(list), 3);

    StringListRemove(&list, "a");
    EXPECT_EQ(StringListSize(list), 1);
    EXPECT_STREQ(list[0], "b");
}

TEST_F(TestDynList, TestRemoveDup) {
    StringListAdd(&list, "a");
    StringListAdd(&list, "b");
    StringListAdd(&list, "a");
    StringListAdd(&list, "c");
    StringListAdd(&list, "b");

    EXPECT_EQ(StringListSize(list), 5);
    StringListRemoveDuplicates(&list);
    EXPECT_EQ(StringListSize(list), 3);

    EXPECT_STREQ(list[0], "a");
    EXPECT_STREQ(list[1], "b");
    EXPECT_STREQ(list[2], "c");
}

TEST_F(TestDynList, TestReplace) {
    StringListAdd(&list, "a");
    StringListAdd(&list, "b");
    StringListAdd(&list, "a");

    StringListReplaceInStrings(list, "a", "d");
    EXPECT_EQ(StringListSize(list), 3);
    EXPECT_STREQ(list[0], "d");
    EXPECT_STREQ(list[1], "b");
    EXPECT_STREQ(list[2], "d");
}

TEST_F(TestDynList, TestSort) {
    StringListAdd(&list, "b");
    StringListAdd(&list, "a");
    StringListAdd(&list, "d");
    StringListAdd(&list, "e");
    StringListAdd(&list, "c");

    StringListSort(list);

    EXPECT_STREQ(list[0], "a");
    EXPECT_STREQ(list[1], "b");
    EXPECT_STREQ(list[2], "c");
    EXPECT_STREQ(list[3], "d");
    EXPECT_STREQ(list[4], "e");
}

TEST_F(TestDynList, TestNull) {
    // Calling with null pointers should not crash
    StringListAdd(NULL, "x");
    StringListRemove(NULL, "x");
    StringListRemoveDuplicates(NULL);

    char** nullList = NULL;
    StringListDestroy(&nullList);
    EXPECT_EQ(nullList, nullptr);
}

TEST_F(TestDynList, TestAutoChangeCapacity) {
    size_t capacity = StringListCapacity(list);
    for (int i = 0; i < capacity + 1; ++i) {
        StringListAdd(&list, "a");
    }
    EXPECT_EQ(StringListCapacity(list), capacity * 2);

    //---------------------------------------------
    StringListRemove(&list, "a");

    EXPECT_EQ(StringListCapacity(list), MIN_CAP);

    //---------------------------------------------
    int n = 128;
    for (int i = 0; i < n; ++i) {
        StringListAdd(&list, "a");
    }

    StringListRemove(&list, "a");

    EXPECT_EQ(StringListCapacity(list), n / 2);
}
