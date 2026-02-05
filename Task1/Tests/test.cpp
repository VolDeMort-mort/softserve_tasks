#include "pch.h"
#include "../Task1/stringList.h"

#include <crtdbg.h> // memory leak check

class StringList : public ::testing::Test {
protected:
	stringList list = nullptr;
	StringList()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		StringListInit(&list);
	}
	~StringList()
	{
		StringListDestroy(&list);
	}
};

TEST_F(StringList, stringDestroy) {
	int i = 0;
	while (i < 10000)
	{
		StringListAdd(list, "Hello Hello Hello");
		++i;
	}
}
TEST_F(StringList, stringRemove) {

	ASSERT_NE(list, nullptr);
	StringListAdd(list, "Hello");
	StringListAdd(list, "Hello");

	StringListRemove(list, "Welcome");
	EXPECT_EQ(stringListGetSize(list), 2);

	StringListRemove(list, "Hello");
	EXPECT_EQ(stringListGetSize(list), 0);

	StringListAdd(list, "Welcome");
	StringListAdd(list, "Hello");
	StringListAdd(list, "Hi");
	
	stringListRemoveAt(list, 1);
	EXPECT_EQ(StringListIndexOf(list, "Hi"), 1);
	EXPECT_EQ(StringListIndexOf(list, "Hello"), -1);
}
TEST_F(StringList, stringReplace) {

	StringListAdd(list, "This is an instance of the string");

	StringListReplaceInStrings(list, "i", "i");
	EXPECT_STREQ(list[0], "This is an instance of the string");

	StringListReplaceInStrings(list, "i", "x");
	EXPECT_STREQ(list[0], "Thxs xs an xnstance of the strxng");
}
TEST_F(StringList, sortingList) 
{
	StringListAdd(list, "remove");
	StringListAdd(list, "Welcome");
	StringListAdd(list, "String");
	StringListAdd(list, "apple");
	StringListAdd(list, "bit");
	StringListAdd(list, "Hi");
	
	StringListSort(list);
	EXPECT_STREQ(list[0], "apple");
	EXPECT_STREQ(list[1], "bit");
	EXPECT_STREQ(list[2], "Hi");
	EXPECT_STREQ(list[3], "remove");
	EXPECT_STREQ(list[4], "String");
	EXPECT_STREQ(list[5], "Welcome");
}
TEST_F(StringList, stringGetIndexAndSize) 
{
	EXPECT_EQ(StringListIndexOf(list, "Hello"), -1);

	StringListAdd(list, "Hello");
	StringListAdd(list, "Welcome");
	StringListAdd(list, "Hi");

	EXPECT_EQ(StringListIndexOf(list, "Welcome"),1);

	EXPECT_EQ(stringListGetSize(list),3);

}