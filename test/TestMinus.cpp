#include "gtest/gtest.h"
#include "../cpp/src/Minus.h"

TEST(TestMinus, Test1) { // 테스트
    EXPECT_TRUE(Minus::Run(1, 2) == -1);    
}

TEST(TestMinus, Test2) {
    EXPECT_TRUE(Minus::Run(1, -1) == 2);    
}   
