#include "gtest/gtest.h" 

namespace MetaProgramming_1 {

    // Factorial n -1 을 재귀 호출합니다.
    template<unsigned int n>
    struct Factorial { 
        enum {
            val = n * Factorial<n-1>::val
        }; 
    };
    // 0일때 특수화 버전. 더이상 재귀 호출을 안합니다.
    template<>
    struct Factorial<0> { 
        enum {
            val = 1
        }; 
    };
}

TEST(TestClassicCpp, MetaProgramming) {
    {
        using namespace MetaProgramming_1;

        // 컴파일타임에 계산된 120이 val에 대입됩니다.
        unsigned int val = Factorial<5>::val; 
        EXPECT_TRUE(val == 1 * 2 * 3 * 4 * 5);
    }
}
