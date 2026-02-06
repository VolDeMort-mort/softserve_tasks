#include "AnyType.hpp"
#include <gtest/gtest.h>

TEST(AnyTypeTest, DefaultConstruction) {
    AnyType val;
    ASSERT_EQ(val.getType(), AnyType::Type::None);
}

TEST(AnyTypeTest, TemplatedConstruction) {
    AnyType intVal(10);
    ASSERT_EQ(intVal.getType(), AnyType::Type::Int);

    AnyType doubleVal(3.14);
    ASSERT_EQ(doubleVal.getType(), AnyType::Type::Double);

    AnyType boolVal(true);
    ASSERT_EQ(boolVal.getType(), AnyType::Type::Bool);
    
    AnyType charVal('a');
    ASSERT_EQ(charVal.getType(), AnyType::Type::Char);
}

TEST(AnyTypeTest, GetValueHappyPath) {
    AnyType intVal(42);
    EXPECT_EQ(intVal.getValue<int>(), 42);

    AnyType doubleVal(2.718);
    EXPECT_DOUBLE_EQ(doubleVal.getValue<double>(), 2.718);

    AnyType boolVal(false);
    EXPECT_EQ(boolVal.getValue<bool>(), false);
}

TEST(AnyTypeTest, GetValueBadCast) {
    AnyType val(10);
    EXPECT_THROW(val.getValue<double>(), BadCastException);
    EXPECT_THROW(val.getValue<bool>(), BadCastException);
}

TEST(AnyTypeTest, TemplatedAssignment) {
    AnyType val;
    val = 5;
    ASSERT_EQ(val.getType(), AnyType::Type::Int);
    EXPECT_EQ(val.getValue<int>(), 5);

    val = 1.23;
    ASSERT_EQ(val.getType(), AnyType::Type::Double);
    EXPECT_DOUBLE_EQ(val.getValue<double>(), 1.23);
    
    val = 'z';
    ASSERT_EQ(val.getType(), AnyType::Type::Char);
    EXPECT_EQ(val.getValue<char>(), 'z');
}

TEST(AnyTypeTest, CopyConstruction) {
    AnyType original(100);
    AnyType copy(original);

    ASSERT_EQ(original.getType(), copy.getType());
    EXPECT_EQ(copy.getValue<int>(), 100);

    original = 200;
    EXPECT_EQ(copy.getValue<int>(), 100);
}

TEST(AnyTypeTest, CopyAssignment) {
    AnyType original(55.5);
    AnyType copy;
    copy = original;

    ASSERT_EQ(original.getType(), copy.getType());
    EXPECT_DOUBLE_EQ(copy.getValue<double>(), 55.5);

    copy = copy;
    ASSERT_EQ(copy.getType(), AnyType::Type::Double);
    EXPECT_DOUBLE_EQ(copy.getValue<double>(), 55.5);
}

TEST(AnyTypeTest, MoveConstruction) {
    AnyType original(true);
    AnyType moved(std::move(original));

    ASSERT_EQ(moved.getType(), AnyType::Type::Bool);
    EXPECT_EQ(moved.getValue<bool>(), true);
    ASSERT_EQ(original.getType(), AnyType::Type::None);
}

TEST(AnyTypeTest, MoveAssignment) {
    AnyType original('x');
    AnyType moved;
    moved = std::move(original);

    ASSERT_EQ(moved.getType(), AnyType::Type::Char);
    EXPECT_EQ(moved.getValue<char>(), 'x');
    ASSERT_EQ(original.getType(), AnyType::Type::None);

    moved = std::move(moved);
    ASSERT_EQ(moved.getType(), AnyType::Type::Char);
    EXPECT_EQ(moved.getValue<char>(), 'x');
}

TEST(AnyTypeTest, Reset) {
    AnyType val(123);
    ASSERT_EQ(val.getType(), AnyType::Type::Int);
    val.reset();
    ASSERT_EQ(val.getType(), AnyType::Type::None);
    EXPECT_THROW(val.getValue<int>(), BadCastException);
}

TEST(AnyTypeTest, Swap) {
    AnyType valA(99);
    AnyType valB(false);

    valA.swap(valB);

    ASSERT_EQ(valA.getType(), AnyType::Type::Bool);
    EXPECT_EQ(valA.getValue<bool>(), false);

    ASSERT_EQ(valB.getType(), AnyType::Type::Int);
    EXPECT_EQ(valB.getValue<int>(), 99);
}
