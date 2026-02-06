#include <gtest/gtest.h>

#include "any_type.h"

TEST(AnyTypeTest, DefaultConstructedHasNoValue) {
    AnyType a;
    EXPECT_FALSE(a.hasValue());
}

TEST(AnyTypeTest, ConstructFromInt) {
    AnyType a(42);
    EXPECT_TRUE(a.hasValue());
    EXPECT_EQ(a.get<int>(), 42);
}

TEST(AnyTypeTest, ConstructFromDouble) {
    AnyType a(3.14);
    EXPECT_EQ(a.get<double>(), 3.14);
}

TEST(AnyTypeTest, ConstructFromBool) {
    AnyType a(true);
    EXPECT_TRUE(a.get<bool>());
}

TEST(AnyTypeTest, BadCastThrows) {
    AnyType a(42);
    EXPECT_THROW(a.get<double>(), BadAnyCast);
}

TEST(AnyTypeTest, GetFromEmptyThrows) {
    AnyType a;
    EXPECT_THROW(a.get<int>(), BadAnyCast);
}

TEST(AnyTypeTest, CopyConstruct) {
    AnyType a(42);
    AnyType b(a);
    EXPECT_EQ(b.get<int>(), 42);

    b = 99;
    EXPECT_EQ(a.get<int>(), 42);
}

TEST(AnyTypeTest, MoveConstruct) {
    AnyType a(42);
    AnyType b(std::move(a));
    EXPECT_EQ(b.get<int>(), 42);
    EXPECT_FALSE(a.hasValue());
}

TEST(AnyTypeTest, CopyAssign) {
    AnyType a(42);
    AnyType b;
    b = a;
    EXPECT_EQ(b.get<int>(), 42);
}

TEST(AnyTypeTest, MoveAssign) {
    AnyType a(42);
    AnyType b;
    b = std::move(a);
    EXPECT_EQ(b.get<int>(), 42);
    EXPECT_FALSE(a.hasValue());
}

TEST(AnyTypeTest, AssignDifferentType) {
    AnyType a(42);
    a = 3.14;
    EXPECT_EQ(a.get<double>(), 3.14);
}

TEST(AnyTypeTest, Reset) {
    AnyType a(42);
    a.reset();
    EXPECT_FALSE(a.hasValue());
}

TEST(AnyTypeTest, Swap) {
    AnyType a(42);
    AnyType b(3.14);
    a.swap(b);
    EXPECT_EQ(a.get<double>(), 3.14);
    EXPECT_EQ(b.get<int>(), 42);
}

TEST(AnyTypeTest, SelfAssign) {
    AnyType a(42);
    a = a;
    EXPECT_EQ(a.get<int>(), 42);
}

TEST(AnyTypeTest, ChainedReassignment) {
    AnyType a = 1;
    a = true;
    a = 1.7;
    EXPECT_EQ(a.get<double>(), 1.7);
}
