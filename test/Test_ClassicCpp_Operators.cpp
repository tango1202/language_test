#include "gtest/gtest.h" 

TEST(TestClassicCpp, Operators) {
    // 증감 연산자
    {
        int n = 10;
        int a = ++n;
        EXPECT_TRUE(a == 11); // 증가시킨 후 값
        EXPECT_TRUE(n == 11);

        int b = n++;
        EXPECT_TRUE(b == 11); // (△) 비권장. 의도한 것인지 조금 헷갈립니다. 증가시킨 전 값. 
        EXPECT_TRUE(n == 12);
    }
}