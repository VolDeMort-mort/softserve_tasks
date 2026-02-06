#include <gtest/gtest.h>
#include "AnyType.h"

TEST(AnyTypeTest, DefaultConstructor) {
     AnyType val;
     EXPECT_EQ(val.GetType(), TypeTag::Empty);
}

TEST(AnyTypeTest, StoreAndRetrieveFundamentalTypes) {
     AnyType valInt = 42;
     EXPECT_EQ(valInt.GetType(), TypeTag::Int);
     EXPECT_EQ(valInt.ToInt(), 42);

     AnyType valDouble = 3.14;
     EXPECT_EQ(valDouble.GetType(), TypeTag::Double);
     EXPECT_DOUBLE_EQ(valDouble.ToDouble(), 3.14);

     AnyType valBool = true;
     EXPECT_TRUE(valBool.ToBool());
}

TEST(AnyTypeTest, ThrowsOnInvalidCast) {
     AnyType val = 100; 

     EXPECT_THROW({
         val.ToDouble();
          }, std::runtime_error);

     EXPECT_THROW({
         val.ToBool();
          }, std::runtime_error);
}

TEST(AnyTypeTest, ClearMethod) {
     AnyType val = 5.5;
     EXPECT_EQ(val.GetType(), TypeTag::Double);

     val.Clear();
     EXPECT_EQ(val.GetType(), TypeTag::Empty);

     EXPECT_THROW(val.ToDouble(), std::runtime_error);
}

TEST(AnyTypeTest, SwapObjects) {
     AnyType a = 10;
     AnyType b = 20.5;

     a.Swap(b);

     EXPECT_EQ(a.GetType(), TypeTag::Double);
     EXPECT_EQ(a.ToDouble(), 20.5);
     EXPECT_EQ(b.GetType(), TypeTag::Int);
     EXPECT_EQ(b.ToInt(), 10);
}

TEST(AnyTypeTest, CopySemantics) {
     AnyType original = 123;
     AnyType copy = original; 

     EXPECT_EQ(copy.ToInt(), 123);

     AnyType assigned;
     assigned = copy; 
     EXPECT_EQ(assigned.ToInt(), 123);
}

TEST(AnyTypeTest, MoveSemantics) {
     AnyType source = 77;
     AnyType movedTo(std::move(source));

     EXPECT_EQ(movedTo.ToInt(), 77);

     EXPECT_EQ(source.GetType(), TypeTag::Empty);
}

TEST(AnyTypeTest, DynamicTypeSwitching) {
     AnyType val = 10;
     EXPECT_EQ(val.GetType(), TypeTag::Int);

     val = true;
     EXPECT_EQ(val.GetType(), TypeTag::Bool);
     EXPECT_TRUE(val.ToBool());

     val = 'Z';
     EXPECT_EQ(val.GetType(), TypeTag::Char);
     EXPECT_EQ(val.ToChar(), 'Z');
}

int main(int argc, char** argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}