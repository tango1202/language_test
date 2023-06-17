#include "gtest/gtest.h" 

TEST(TestClassicCpp, PointerReference) {
    {
        int x = 20;
        int* p = &x; // p는 x의 주소를 가집니다.
        *p = 30; // p가 가리키는 곳이 30이 됩니다.
        EXPECT_TRUE(*p == 30 && x == 30); 
    }
    {
        int x = 20;
        int& r = x; // r은 x의 별칭입니다.
        r = 30; // x는 30이 됩니다.
        EXPECT_TRUE(r == 30 && x == 30);
    }
}