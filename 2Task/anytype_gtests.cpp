#include "AnyType.h"
#include <gtest/gtest.h>


TEST(AnyTypeTest, DefaultConstructedIsEmpty) {
    AnyType a;
    EXPECT_FALSE(a.has_value());
    EXPECT_THROW(a.type(), std::logic_error);
}


TEST(AnyTypeTest, StoreAndGetInt) {
    AnyType a = 42;
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a.get<int>(), 42);
}


TEST(AnyTypeTest, OverwriteWithDifferentType) {
    AnyType a = 42;
    a = 3.14;

    EXPECT_EQ(a.get<double>(), 3.14);
    EXPECT_THROW(a.get<int>(), BadAnyCast);
}


TEST(AnyTypeTest, BoolStorage) {
    AnyType a = true;
    EXPECT_TRUE(a.get<bool>());
}


TEST(AnyTypeTest, CopyConstructor) {
    AnyType a = 100L;
    AnyType b = a;

    EXPECT_EQ(b.get<long>(), 100L);
    EXPECT_EQ(a.get<long>(), 100L);
}


TEST(AnyTypeTest, MoveConstructor) {
    AnyType a = 55;
    AnyType b = std::move(a);

    EXPECT_EQ(b.get<int>(), 55);
    EXPECT_FALSE(a.has_value());
}


TEST(AnyTypeTest, CopyAssignment) {
    AnyType a = 10;
    AnyType b;

    b = a;

    EXPECT_EQ(b.get<int>(), 10);
}


TEST(AnyTypeTest, MoveAssignment) {
    AnyType a = 77;
    AnyType b;

    b = std::move(a);

    EXPECT_EQ(b.get<int>(), 77);
    EXPECT_FALSE(a.has_value());
}


TEST(AnyTypeTest, SwapValues) {
    AnyType a = 10L;
    AnyType b = 2.5f;

    a.swap(b);

    EXPECT_EQ(a.get<float>(), 2.5f);
    EXPECT_EQ(b.get<long>(), 10L);
}


TEST(AnyTypeTest, DestroyResetsState) {
    AnyType a = 123;
    a.destroy();

    EXPECT_FALSE(a.has_value());
    EXPECT_THROW(a.get<int>(), BadAnyCast);
}


TEST(AnyTypeTest, TypeCheck) {
    AnyType a = 42u;
    EXPECT_EQ(a.type(), typeid(unsigned int));
}


TEST(AnyTypeTest, WrongCastThrows) {
    AnyType a = 3.14;
    EXPECT_THROW(a.get<int>(), BadAnyCast);
}


TEST(AnyTypeTest, SupportsVariousFundamentalTypes) {
    AnyType a = L'A';
    EXPECT_EQ(a.get<wchar_t>(), L'A');

    a = static_cast<unsigned long long>(9999999);
    EXPECT_EQ(a.get<unsigned long long>(), 9999999ULL);

    a = static_cast<char16_t>(u'Z');
    EXPECT_EQ(a.get<char16_t>(), u'Z');
}


TEST(AnyTypeTest, MultipleReassignments) {
    AnyType a = 1;
    a = 2.0;
    a = false;
    a = 'x';

    EXPECT_EQ(a.get<char>(), 'x');
}

