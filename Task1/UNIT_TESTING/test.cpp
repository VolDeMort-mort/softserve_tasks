#include "pch.h"
#include "../Task1/stringList.h"


TEST(StringListTest, InitAndDestroy) {
	char** list = nullptr;
	stringListInit(&list);
	EXPECT_NE(list, nullptr);
	EXPECT_NE(*list, nullptr);
	EXPECT_STREQ(*list, "5|0");
	stringListDestroy(&list);
	EXPECT_EQ(list, nullptr);
}

TEST(StringListTest, SizeAndLen) {

	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;

	stringListInit(&list);
	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(a, 5);
	EXPECT_EQ(b, 0);

	_encodeHeader(list, 22, 12);
	EXPECT_STREQ(list[0], "22|12");

	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(a, 22);
	EXPECT_EQ(b, 12);

	_encodeHeader(list, 5, 0);
	stringListDestroy(&list);
}

TEST(StringListTest, AddItemIncreaseCapTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;

	stringListInit(&list);
	for (int i = 1; i <= 16; i++) {
		stringListAdd(&list, (std::string("Hello") + std::to_string(i)).c_str());
	}

	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(a, 20);
	EXPECT_EQ(b, 16);

	for (int i = 1; i <= 16; i++) {
		EXPECT_STREQ(list[i], (std::string("Hello") + std::to_string(i)).c_str());
	}

	stringListDestroy(&list);
}

TEST(StringListTest, RemoveItemDecreaseCapTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;

	stringListInit(&list);

	for (int i = 0; i < 16; i++) {
		stringListAdd(&list, (std::string("Hello") + std::to_string(i)).c_str());
	}


	for (int i = 15; i >= 10; i--) {
		stringListRemoveAt(&list, i);
	}

	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(b, 10);

	for (int i = 0; i < 4; i++) {
		EXPECT_STREQ(list[i + 1], (std::string("Hello") + std::to_string(i)).c_str());
	}

	for (int i = 9; i >= 6; i--) {
		bool res = stringListRemoveAt(&list, i);
		EXPECT_EQ(res, true);
	}

	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(b, 6);

	for (int i = 0; i < 4; i++) {
		EXPECT_STREQ(list[i + 1], (std::string("Hello") + std::to_string(i)).c_str());
	}

	EXPECT_ANY_THROW(stringListRemoveAt(&list, 15));

	stringListDestroy(&list);
}

TEST(StringListTest, RemoveStringTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;

	stringListInit(&list);

	for (int i = 1; i <= 20; i++) {
		if ((i & 1) == 1) {
			stringListAdd(&list, (std::string("Hello") + std::to_string(i)).c_str());
		}
		else {
			stringListAdd(&list, (std::string("World")).c_str());
		}
	}

	bool res = stringListRemoveString(&list, "WorldNotExist");
	EXPECT_EQ(res, false);
	res = stringListRemoveString(&list, "World");
	EXPECT_EQ(res, true);
	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(a, 15);
	EXPECT_EQ(b, 10);
	for (int i = 1; i <= 10; i++) {
		EXPECT_STREQ(list[i], (std::string("Hello") + std::to_string(i * 2 - 1)).c_str());
	}

	stringListDestroy(&list);
}

TEST(StringListTest, getSizeTest) {
	char** list = nullptr;
	stringListInit(&list);
	EXPECT_EQ(stringListSize(list), 0);
	for (int i = 1; i <= 10; i++) {
		stringListAdd(&list, (std::string("Hello") + std::to_string(i)).c_str());
	}
	EXPECT_EQ(stringListSize(list), 10);
	stringListRemoveAt(&list, 5);
	EXPECT_EQ(stringListSize(list), 9);
	stringListRemoveString(&list, "Hello1");
	EXPECT_EQ(stringListSize(list), 8);
	stringListDestroy(&list);
}

TEST(StringListTest, FindTest) {
	char** list = nullptr;
	stringListInit(&list);
	for (int i = 1; i <= 10; i++) {
		stringListAdd(&list, (std::string("Hello") + std::to_string(i)).c_str());
	}
	int index = stringListFind(list, "Hello5");
	EXPECT_EQ(index, 4);
	index = stringListFind(list, "Hello11");
	EXPECT_EQ(index, -1);
	stringListDestroy(&list);
}

TEST(StringListTest, RemoveDuplicatesTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;
	stringListInit(&list);
	stringListAdd(&list, "Hello");
	stringListAdd(&list, "World");
	stringListAdd(&list, "Hello");
	stringListAdd(&list, "Test");
	stringListAdd(&list, "World");
	stringListAdd(&list, "Example");
	bool res = stringListRemoveDuplicates(&list);
	EXPECT_EQ(res, true);
	_decodeHeader(*list, &a, &b);
	EXPECT_EQ(b, 4);
	EXPECT_STREQ(list[1], "Hello");
	EXPECT_STREQ(list[2], "World");
	EXPECT_STREQ(list[3], "Test");
	EXPECT_STREQ(list[4], "Example");
	res = stringListRemoveDuplicates(&list);
	EXPECT_EQ(res, false);
	stringListDestroy(&list);
}

TEST(StringListTest, ReplaceAllTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;
	stringListInit(&list);
	stringListAdd(&list, "Hello");
	stringListAdd(&list, "World");
	stringListAdd(&list, "Test1");
	for(size_t i = 0; i < 5; i++) {
		stringListAdd(&list, "Hello");
	}

	bool res = stringListReplaceAll(&list, "Hello", "Hi!");
	EXPECT_EQ(res, true);
	_decodeHeader(*list, &a, &b);
	EXPECT_STREQ(list[1], "Hi!");
	EXPECT_STREQ(list[2], "World");
	EXPECT_STREQ(list[3], "Test1");
	for (int i = 4; i <= 8; i++) {
		EXPECT_STREQ(list[i], "Hi!");
	}

	res = stringListReplaceAll(&list, "sjofhdsjf", "11111");
	EXPECT_EQ(res, false);

	stringListDestroy(&list);
}

TEST(StringListTest, ReplacePartAllTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;

	stringListInit(&list);
	stringListAdd(&list, "TestSTRTOREPLACE#1");
	stringListAdd(&list, "STRTOREPLACETest#2");
	stringListAdd(&list, "TEST#3STRTOREPLACE");

	bool res = stringListReplacePartAll(&list, "STRTOREPLACE", "REPLACED");
	EXPECT_EQ(res, true);

	EXPECT_STREQ(list[1], "TestREPLACED#1");
	EXPECT_STREQ(list[2], "REPLACEDTest#2");
	EXPECT_STREQ(list[3], "TEST#3REPLACED");

	res = stringListReplacePartAll(&list, "NOTEXIST", "11111");
	EXPECT_EQ(res, false);

	stringListDestroy(&list);
}

TEST(StringListTest, SortingTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;
	stringListInit(&list);
	stringListAdd(&list, "dc");
	stringListAdd(&list, "ab");
	stringListAdd(&list, "aa");
	stringListAdd(&list, "ac");
	stringListAdd(&list, "a");
	bool res = stringListSort(&list, true);
	EXPECT_EQ(res, true);
	EXPECT_STREQ(list[1], "a");
	EXPECT_STREQ(list[2], "aa");
	EXPECT_STREQ(list[3], "ab");
	EXPECT_STREQ(list[4], "ac");
	EXPECT_STREQ(list[5], "dc");

	res = stringListSort(&list, false);
	EXPECT_EQ(res, true);
	EXPECT_STREQ(list[1], "dc");
	EXPECT_STREQ(list[2], "ac");
	EXPECT_STREQ(list[3], "ab");
	EXPECT_STREQ(list[4], "aa");
	EXPECT_STREQ(list[5], "a");

	res = stringListSort(&list, false);
	EXPECT_EQ(res, false);

	stringListDestroy(&list);
}

TEST(StirngListTest, GetAtTest) {
	char** list = nullptr;
	size_t a = 0;
	size_t b = 0;
	stringListInit(&list);
	for (int i = 1; i <= 5; i++) {
		stringListAdd(&list, (std::string("Hello") + std::to_string(i)).c_str());
	}

	for (int i = 0; i < 5; i++) {
		char* str = stringListGetAt(list, i);
		EXPECT_STREQ(str, (std::string("Hello") + std::to_string(i + 1)).c_str());
	}

	EXPECT_ANY_THROW(stringListGetAt(list, 5));
	EXPECT_ANY_THROW(stringListGetAt(list, -10));
	stringListDestroy(&list);
}