#include "pch.h"
#include "../AnyTypeTask/AnyType.h"\

TEST(AnyTypeTest, GetInt)
{
    AnyType a = 42;
    EXPECT_EQ(a.toInt(), 42);
}

TEST(AnyTypeTest, GetDouble)
{
    AnyType a = 3.14;
    EXPECT_DOUBLE_EQ(a.toDouble(), 3.14);
}

TEST(AnyTypeTest, GetBool)
{
    AnyType a = true;
    EXPECT_EQ(a.toBool(), true);
}

TEST(AnyTypeTest, GetChar)
{
    AnyType a = 'x';
    EXPECT_EQ(a.toChar(), 'x');
}

TEST(AnyTypeTest, BadCast)
{
    AnyType a = 10.5;
    EXPECT_THROW(a.toInt(), std::bad_cast);
}

TEST(AnyTypeTest, BadCastAfterReassign)
{
    AnyType a = 5;
    a = true;
    EXPECT_THROW(a.toInt(), std::bad_cast);
}

TEST(AnyTypeTest, CopyConstructor)
{
    AnyType a = 7;
    AnyType b = a;

    EXPECT_EQ(b.toInt(), 7);
}

TEST(AnyTypeTest, CopyAssignment)
{
    AnyType a = 9;
    AnyType b;

    b = a;
    EXPECT_EQ(b.toInt(), 9);
}

TEST(AnyTypeTest, MoveConstructor)
{
    AnyType a = 11;
    AnyType b = std::move(a);

    EXPECT_EQ(b.toInt(), 11);
    EXPECT_THROW(a.toInt(), std::bad_cast);
}

TEST(AnyTypeTest, MoveAssignment)
{
    AnyType a = 13;
    AnyType b;

    b = std::move(a);
    EXPECT_EQ(b.toInt(), 13);
    EXPECT_THROW(a.toInt(), std::bad_cast);
}

TEST(AnyTypeTest, AssignDifferentTypes)
{
    AnyType a = 1;

    a = true;
    EXPECT_EQ(a.toBool(), true);

    a = 2.5;
    EXPECT_DOUBLE_EQ(a.toDouble(), 2.5);

    a = 'c';
    EXPECT_EQ(a.toChar(), 'c');
}

TEST(AnyTypeTest, Destroy)
{
    AnyType a = 3;
    a.destroy();

    EXPECT_THROW(a.toInt(), std::bad_cast);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
