#include <gtest/gtest.h>
#include "structures.h" 


TEST(AnyTypeBasic, EmptyDefaultConstructed)
{
	AnyType a;
	EXPECT_FALSE(a.has_value());
}


TEST(AnyTypeBasic, EmplaceInt)
{
	AnyType a;
	a.emplace<int>(5);
	EXPECT_TRUE(a.has_value());
	EXPECT_EQ(a.cast<int>(), 5);
}


TEST(AnyTypeBasic, TypeInfo)
{
	AnyType a(10);
	EXPECT_EQ(a.type(), typeid(int));
}


TEST(AnyTypeBasic, StoreDouble)
{
	AnyType a(3.14);
	EXPECT_DOUBLE_EQ(a.cast<double>(), 3.14);
}

TEST(AnyTypeBasic, Copy)
{
	AnyType a(123);
	AnyType b = a;
	EXPECT_EQ(b.cast<int>(), 123);
}


TEST(AnyTypeBasic, Move)
{
	AnyType a(123);
	AnyType b = std::move(a);
	EXPECT_EQ(b.cast<int>(), 123);
	EXPECT_FALSE(a.has_value());
}



TEST(AnyTypeAdvanced, Reset)
{
	AnyType a(42);
	EXPECT_TRUE(a.has_value());
	a.reset();
	EXPECT_FALSE(a.has_value());
}

TEST(AnyTypeAdvanced, EmplaceOverwrite)
{
	AnyType a(56);
	a.emplace<double>(3.14);
	EXPECT_TRUE(a.has_value());
	EXPECT_DOUBLE_EQ(a.cast<double>(), 3.14);
}


TEST(AnyTypeAdvanced, CastWrongTypeThrows)
{
	AnyType a(42);
	EXPECT_THROW(a.cast<double>(), BadAnyCast);
}


TEST(AnyTypeAdvanced, Swap)
{
	AnyType a(10);
	AnyType b(3.14);
	a.swap(b);
	EXPECT_DOUBLE_EQ(a.cast<double>(), 3.14);
	EXPECT_EQ(b.cast<int>(), 10);
}


TEST(AnyTypeAdvanced, SelfSwap)
{
	AnyType a(99);
	a.swap(a); 
	EXPECT_EQ(a.cast<int>(), 99);
}


TEST(AnyTypeAdvanced, MoveAssignment)
{
	AnyType a(50);
	AnyType b;
	b = std::move(a);
	EXPECT_EQ(b.cast<int>(), 50);
	EXPECT_FALSE(a.has_value());
}


TEST(AnyTypeAdvanced, CopyAssignment)
{
	AnyType a(100);
	AnyType b;
	b = a;
	EXPECT_EQ(b.cast<int>(), 100);
}