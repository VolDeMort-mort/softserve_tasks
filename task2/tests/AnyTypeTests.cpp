#include <gtest/gtest.h>
#include "AnyType.h"

TEST(AnyTypeTest, StoresIntAndConverts) {
    AnyType a(42);
    EXPECT_EQ(a.ToInt(), 42);
    EXPECT_STREQ(a.get_type(), "INT");
}

TEST(AnyTypeTest, ThrowsOnBadCast) {
    AnyType a(3.14);
    EXPECT_THROW(a.ToInt(), BadAnyCast);
}

TEST(AnyTypeTest, SwapWorks) {
    AnyType a(10);
    AnyType b('A');

    a.Swap(b);

    EXPECT_EQ(b.ToInt(), 10);
    EXPECT_EQ(a.ToChar(), 'A');
}

TEST(AnyTypeTest, CopyConstructorKeepsValue) {
    AnyType a(true);
    AnyType b(a);

    EXPECT_TRUE(b.ToBool());
    EXPECT_STREQ(b.get_type(), "BOOL");
}

TEST(AnyTypeTest, AssignmentFromPrimitive) {
    AnyType a;
    a = 5;
    EXPECT_EQ(a.ToInt(), 5);

    a = false;
    EXPECT_FALSE(a.ToBool());
}
