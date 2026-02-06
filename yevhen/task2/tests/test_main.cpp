#include <gtest/gtest.h>
#include "../src/AnyType.h"
#include "../src/BadAnyCast.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}