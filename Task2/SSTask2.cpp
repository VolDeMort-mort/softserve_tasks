#include <gtest/gtest.h>
#include "AnyType.h"

TEST(AnyTypeTest, BasicConstructorAndType) {
    AnyType intAny(42);
    EXPECT_EQ(intAny.GetType(), AnyType::Type::Int);
    EXPECT_EQ(intAny.ToInt(), 42);

    AnyType doubleAny(3.14);
    EXPECT_EQ(doubleAny.GetType(), AnyType::Type::Double);
    EXPECT_DOUBLE_EQ(doubleAny.ToDouble(), 3.14);
}

TEST(AnyTypeTest, TypeSafeAccessThrowsException) {
    AnyType anyType = 42;

    EXPECT_EQ(anyType.ToInt(), 42);

    EXPECT_THROW(anyType.ToDouble(), BadAnyTypeCast);
    EXPECT_THROW(anyType.ToBool(), BadAnyTypeCast);
}

TEST(AnyTypeTest, AssignmentAndReassignment) {
    AnyType anyType = 1;
    EXPECT_EQ(anyType.GetType(), AnyType::Type::Int);

    anyType = true;
    EXPECT_EQ(anyType.GetType(), AnyType::Type::Bool);
    EXPECT_TRUE(anyType.ToBool());

    anyType = 1.7;
    EXPECT_EQ(anyType.GetType(), AnyType::Type::Double);
    EXPECT_DOUBLE_EQ(anyType.ToDouble(), 1.7);
}

TEST(AnyTypeTest, TaskUsageExample) {
    AnyType anyType = 1;
    anyType = true;
    anyType = 1.7;

    try {
        int v = anyType.ToInt();
        FAIL() << "Expected BadAnyTypeCast exception";
    }
    catch (const BadAnyTypeCast&) {
    }

    double storedValue = anyType.ToDouble();
    EXPECT_DOUBLE_EQ(storedValue, 1.7);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}