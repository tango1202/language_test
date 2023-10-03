#include "gtest/gtest.h" 

TEST(TestMordern, BineryLiteral) {
    // C++14 이진 리터럴
    {
        int val1{0b11};
        int val2{0b1111};
        int val3{0B11111111};

        EXPECT_TRUE(val1 == 0x03);
        EXPECT_TRUE(val2 == 0x0F);
        EXPECT_TRUE(val3 == 0xFF);
    }
}