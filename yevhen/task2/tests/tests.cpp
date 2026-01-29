#include <gtest/gtest.h>
#include "../src/AnyType.h"
#include "../src/BadAnyCast.h"
#include <thread>

TEST(AnyTypeTest, AssignmentValid) {
    AnyType a;
    a = 42;
    EXPECT_EQ(Type::i, a.getType());

    a = true;
    EXPECT_EQ(Type::b, a.getType());
}

TEST(AnyTypeTest, AssignmentChangesType) {
    AnyType a;
    a = 3.14;
    a = 2.28345321f;
    EXPECT_FLOAT_EQ(2.28345321f, a.ToFloat());
}

TEST(AnyTypeTest, AssignmentInvalidCast) {
    AnyType a(321);
    a = false;
    EXPECT_THROW(a.ToInt(), BadAnyCast);
}

TEST(AnyTypeTest, CopyOperator) {
    AnyType a(3.14159);
    AnyType b(1);
    b = a;
    EXPECT_DOUBLE_EQ(3.14159, b.ToDouble());
    b = 'a';
    EXPECT_EQ('a', b.ToChar());
    EXPECT_DOUBLE_EQ(3.14159, a.ToDouble());
}

TEST(AnyTypeTest, CopyConstructor) {
    AnyType a(2.71828);
    AnyType b(a);
    a = 3;
    EXPECT_DOUBLE_EQ(2.71828, b.ToDouble());
    EXPECT_EQ(3, a.ToInt());
}

TEST(AnyTypeTest, SelfAssignment) {
    AnyType a(12345u);
    a = a;
    EXPECT_EQ(12345u, a.ToUInt());
}

TEST(AnyTypeTest, ResetFunctionality) {
    AnyType a(100u);
    EXPECT_EQ(Type::u_i, a.getType());
    a.reset();
    EXPECT_EQ(Type::invalid, a.getType());
    EXPECT_THROW(a.ToUInt(), BadAnyCast);
}

TEST(AnyTypeTest, SwapFunctionality) {
    AnyType a(10);
    AnyType b(20u);
    swap(a, b);
    EXPECT_EQ(20u, a.ToUInt());
    EXPECT_EQ(10, b.ToInt());
}
