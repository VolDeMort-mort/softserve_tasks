#include "pch.h"
#include "../Task 2/AnyType.h"

TEST(AnyType, anyTypeTest)
{
	AnyType a = 5;

	EXPECT_EQ(a.toInt(), 5);
	EXPECT_THROW(a.toChar(), std::bad_cast);

	a = 2783965240U;
	EXPECT_EQ(a.toUInt(), 2783965240U);

	a = static_cast<short>(5);
	EXPECT_EQ(a.toShort(), 5);

	a = 5.5;
	EXPECT_EQ(a.toDouble(), 5.5);

	a = 3.3F;
	EXPECT_EQ(a.toFloat(), 3.3F);

	a = true;
	EXPECT_EQ(a.toBool(), true);

	a = 'a';
	EXPECT_EQ(a.toChar(), 'a');

	a = 931231233L;
	EXPECT_EQ(a.toLong(), 931231233L);
}
TEST(AnyType, constructorTest)
{
	AnyType a;
	EXPECT_FALSE(a.hasValue());
}
TEST(AnyType, templateConstrTest)
{
	AnyType a = 'a';
	EXPECT_TRUE(a.hasValue());
}
TEST(AnyType, copyConstrTest)
{
	AnyType a = 'a';
	AnyType b(a);
	EXPECT_TRUE(b.hasValue());

}
TEST(AnyType, moveConstrTest)
{
	AnyType a = 'a';
	AnyType b(std::move(a));
	EXPECT_TRUE(b.hasValue());
	EXPECT_FALSE(a.hasValue());
}
TEST(AnyType, AssignmentTest)
{
	AnyType a = 'a';
	a = 100;
}
TEST(AnyType, copyAssignmentTest)
{
	AnyType a = 'a';
	AnyType b = 42;
	a = b;
	EXPECT_EQ(a.toInt(), 42);
}
TEST(AnyType, moveTest)
{
	AnyType a = 'a';
	AnyType b = 100;
	a = std::move(b);

}
TEST(AnyType, swapTest)
{
	AnyType a = 5;
	AnyType b = true;

	a.swap(b);

	EXPECT_EQ(b.toInt(), 5);
	EXPECT_EQ(a.toBool(), true);
}

TEST(AnyType, changeTest)
{
	AnyType a = 5;

	a.changeType('a');
	EXPECT_EQ(a.toChar(), 'a');

	EXPECT_THROW(a.changeType("string"), std::invalid_argument);
}
