#include "pch.h"
#include <type_traits>
#include "../Task2/AnyType.h"

TEST(AnyTypeTest, InitTest) {
	AnyType any(2);
	EXPECT_EQ(any.get<int>(), 2);
	EXPECT_EQ(any.typeof(), AnyType::Type::Int);
}

TEST(AnyTypeTest, CopyConstructorTest) {
	AnyType any1(3.14);
	AnyType any2(any1);
	EXPECT_EQ(any2.get<double>(), 3.14);
	EXPECT_EQ(any2.typeof(), AnyType::Type::Double);
}

TEST(AnyTypeTest, MoveConstructorTest) {
	AnyType any1(42);
	AnyType any2(std::move(any1));
	EXPECT_EQ(any2.get<int>(), 42);
	EXPECT_EQ(any2.typeof(), AnyType::Type::Int);
	EXPECT_EQ(any1.typeof(), AnyType::Type::None);
}

TEST(AnyTypeTest, AssignmentOperatorTest) {
	AnyType any1(100);
	AnyType any2;
	any2 = any1;
	EXPECT_EQ(any2.get<int>(), 100);
	EXPECT_EQ(any2.typeof(), AnyType::Type::Int);
}

TEST(AnyTypeTest, MoveAssignmentOperatorTest) {
	AnyType any1(200);
	AnyType any2;
	any2 = std::move(any1);
	EXPECT_EQ(any2.get<int>(), 200);
	EXPECT_EQ(any2.typeof(), AnyType::Type::Int);
	EXPECT_EQ(any1.typeof(), AnyType::Type::None);
}

TEST(AnyTypeTest, DestroyTest) {
	AnyType any(123);
	any.destroy();
	EXPECT_EQ(any.typeof(), AnyType::Type::None);
}

TEST(AnyTypeTest, SwapTest) {
	AnyType any1(10);
	AnyType any2(20.5);
	any1.swapWith(any2);
	EXPECT_EQ(any1.get<double>(), 20.5);
	EXPECT_EQ(any1.typeof(), AnyType::Type::Double);
	EXPECT_EQ(any2.get<int>(), 10);
	EXPECT_EQ(any2.typeof(), AnyType::Type::Int);
}

TEST(AnyTypeTest, TypeMismatchTest) {
	AnyType any(5ULL);
	EXPECT_ANY_THROW(any.get<int>());
	EXPECT_ANY_THROW(any.get<double>());
	EXPECT_ANY_THROW(any.get<float>());
	EXPECT_ANY_THROW(any.get<short>());
	EXPECT_ANY_THROW(any.get<long long>());

	any = (signed char)-5;
	EXPECT_ANY_THROW(any.get<char>());
	EXPECT_ANY_THROW(any.get<int>());
}

TEST(AnyTypeTest, GetFromEmptyTest) {
	AnyType any;
	EXPECT_ANY_THROW(any.get<int>());
	EXPECT_ANY_THROW(any.get<double>());
}

TEST(AnyTypeTest, MultipleTypesTest) {
	AnyType any;
	any = true;
	EXPECT_EQ(any.get<bool>(), true);
	EXPECT_EQ(any.typeof(), AnyType::Type::Bool);
	any = 'A';
	EXPECT_EQ(any.get<char>(), 'A');
	EXPECT_EQ(any.typeof(), AnyType::Type::Char);
	any = 255U;
	EXPECT_EQ(any.get<unsigned int>(), 255U);
	EXPECT_EQ(any.typeof(), AnyType::Type::UInt);
	any = 3.14f;
	EXPECT_EQ(any.get<float>(), 3.14f);
	EXPECT_EQ(any.typeof(), AnyType::Type::Float);
	any = 2.71828;
	EXPECT_EQ(any.get<double>(), 2.71828);
	EXPECT_EQ(any.typeof(), AnyType::Type::Double);
}


TEST(AnyTypeTest, SetValTest) {
	AnyType any(5);
	any.set(10.5);
	EXPECT_EQ(any.typeof(), AnyType::Type::Double);
	EXPECT_EQ(any.get<double>(), 10.5);

	any.set(true);
	EXPECT_EQ(any.typeof(), AnyType::Type::Bool);
	EXPECT_EQ(any.get<bool>(), true);

}