#include "gtest/gtest.h" 

TEST(TestMordern, Utility) {
    // 정수 비교
    {
        int x{-1}; // 0xFFFF FFFF(4294967295) 
        unsigned int y{1};
        // EXPECT_TRUE(x < y); // (X) 런타임 오류. -1은 unsigned int로 형변환되어 4294967295입니다.
    }
#if 202002L <= __cplusplus // C++20~
    {
        int x{-1}; // 0xFFFF FFFF(4294967295) 
        unsigned int y{1};
        EXPECT_TRUE(std::cmp_less(x, y)); // (O) 음수와 양수를 정상적으로 비교합니다.       
    }
 
#endif    
}