#include "gtest/gtest.h" 

TEST(TestMordern, DigitSeperator) {
    // C++14
    int val1 = 1000000;
    int val2 = 1'000'000;

    EXPECT_TRUE(val1 == val2);
}