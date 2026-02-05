#include "pch.h"

#include "../task2_lib/AnyType.h"

// Construcrors tests
TEST(AnyTypeTest, TestDefaultConstor) {
    AnyType a;
    EXPECT_EQ(a.getType(), AnyType::Type::Empty);
}

TEST(AnyTypeTest, TestConstructorsAndGetters) {
    // Char
    AnyType a_char('x');
    EXPECT_EQ(a_char.getType(), AnyType::Type::Char);
    EXPECT_EQ(a_char.getChar(), 'x');

    // UnsignedChar
    AnyType a_uchar((unsigned char)123);
    EXPECT_EQ(a_uchar.getType(), AnyType::Type::UnsignedChar);
    EXPECT_EQ(a_uchar.getUnsignedChar(), (unsigned char)123);

    // SignedChar
    AnyType a_schar((signed char)-12);
    EXPECT_EQ(a_schar.getType(), AnyType::Type::SignedChar);
    EXPECT_EQ(a_schar.getSignedChar(), (signed char)-12);

    // Int
    AnyType a_int(123);
    EXPECT_EQ(a_int.getType(), AnyType::Type::Int);
    EXPECT_EQ(a_int.getInt(), 123);

    // ShortInt
    AnyType a_short((short int)123);
    EXPECT_EQ(a_short.getType(), AnyType::Type::ShortInt);
    EXPECT_EQ(a_short.getShortInt(), (short int)123);

    // LongInt
    AnyType a_long((long int)123);
    EXPECT_EQ(a_long.getType(), AnyType::Type::LongInt);
    EXPECT_EQ(a_long.getLongInt(), (long int)123);

    // LongLongInt
    AnyType a_ll((long long int)123);
    EXPECT_EQ(a_ll.getType(), AnyType::Type::LongLongInt);
    EXPECT_EQ(a_ll.getLongLongInt(), (long long int)123);

    // UnsignedInt
    AnyType a_uint((unsigned int)123u);
    EXPECT_EQ(a_uint.getType(), AnyType::Type::UnsignedInt);
    EXPECT_EQ(a_uint.getUnsignedInt(), (unsigned int)123u);

    // UnsignedShortInt
    AnyType a_ushort((unsigned short int)123u);
    EXPECT_EQ(a_ushort.getType(), AnyType::Type::UnsignedShortInt);
    EXPECT_EQ(a_ushort.getUnsignedShortInt(), (unsigned short int)123u);

    // UnsignedLongInt
    AnyType a_ulong((unsigned long int)123u);
    EXPECT_EQ(a_ulong.getType(), AnyType::Type::UnsignedLongInt);
    EXPECT_EQ(a_ulong.getUnsignedLongInt(), (unsigned long int)123u);

    // UnsignedLongLongInt
    AnyType a_ull((unsigned long long int)123ull);
    EXPECT_EQ(a_ull.getType(), AnyType::Type::UnsignedLongLongInt);
    EXPECT_EQ(a_ull.getUnsignedLongLongInt(), (unsigned long long int)123ull);

    // Float
    AnyType a_float(123.0f);
    EXPECT_EQ(a_float.getType(), AnyType::Type::Float);
    EXPECT_FLOAT_EQ(a_float.getFloat(), 123.0f);

    // Double
    AnyType a_double(123.0);
    EXPECT_EQ(a_double.getType(), AnyType::Type::Double);
    EXPECT_DOUBLE_EQ(a_double.getDouble(), 123.0);

    // LongDouble
    AnyType a_longdouble(123.0L);
    EXPECT_EQ(a_longdouble.getType(), AnyType::Type::LongDouble);
    EXPECT_DOUBLE_EQ(a_longdouble.getLongDouble(), 123.0L);

}


TEST(AnyTypeTest, TestCopyConstructorAndAssignment) {
    AnyType a((int)7);
    AnyType b(a);
    EXPECT_EQ(b.getType(), AnyType::Type::Int);
    EXPECT_EQ(b.getInt(), 7);

    AnyType c;
    c = a;
    EXPECT_EQ(c.getType(), AnyType::Type::Int);
    EXPECT_EQ(c.getInt(), 7);
}

TEST(AnyTypeTest, TestMoveConstructorAndAssignment) {
    AnyType a((int) 7);
    AnyType b(std::move(a));
    EXPECT_EQ(b.getType(), AnyType::Type::Int);
    EXPECT_EQ(b.getInt(), 7);
    EXPECT_EQ(a.getType(), AnyType::Type::Empty);

    AnyType c;
    c = std::move(b);
    EXPECT_EQ(c.getType(), AnyType::Type::Int);
    EXPECT_EQ(c.getInt(), 7);
    EXPECT_EQ(b.getType(), AnyType::Type::Empty);
}
//------------------------------------------------

TEST(AnyTypeTest, TestSwap) {
    AnyType a((int)1);
    AnyType b((double)2.5);

    a.swap(b);

    EXPECT_EQ(a.getType(), AnyType::Type::Double);
    EXPECT_DOUBLE_EQ(a.getDouble(), 2.5);

    EXPECT_EQ(b.getType(), AnyType::Type::Int);
    EXPECT_EQ(b.getInt(), 1);
}

TEST(AnyTypeTest, TestDestroy) {
    AnyType a(5);
    EXPECT_EQ(a.getType(), AnyType::Type::Int);
    a.destroy();
    EXPECT_EQ(a.getType(), AnyType::Type::Empty);
}

TEST(AnyTypeTest, TestWrongGetterThrows) {
    AnyType a(5);
    EXPECT_EQ(a.getType(), AnyType::Type::Int);

    EXPECT_THROW(a.getDouble(), BadAnyTypeCase);
}
