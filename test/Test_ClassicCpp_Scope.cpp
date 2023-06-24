#include "gtest/gtest.h" 

namespace {
    int x = 1; // 전역 변수. namespace 내에 있어 현 파일에서만 접근 가능
}

TEST(TestClassicCpp, Scope) {
    EXPECT_TRUE(x == 1); // 전역 변수
    int x = 2; 
    EXPECT_TRUE(x == 2); // 지역변수가 전역 x를 가림
    {
        int x = 3;
        EXPECT_TRUE(x == 3); // 블록이 지역 x를 가림. 지역 x에 접근할 방법이 없어요.
        ::x = 10; // :: 붙여 강제로 전역 접근
    } 
    EXPECT_TRUE(x == 2); // 지역 변수
    EXPECT_TRUE(::x == 10); // 전역 변수
}
