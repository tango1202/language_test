#include "gtest/gtest.h" 

namespace {
    template<typename Type>
    Type Sum(Type param) {
        return param; // 재귀 호출하다가 마지막에 도달하면 호출됩니다.
    }
    template<typename Type, typename... Types>
    Type Sum(Type param, Types... params) {
        return param + Sum(params...); // 재귀 호출시 params의 첫번째 인자는 param으로 전달되고, 나머지는 params에 전달됩니다.
    }    

    template<typename... Types>
    int Func(Types... params) {
        return sizeof...(params);
    }
}

TEST(TestCppPattern, VaradicTemplate) {
    {
        int val = Sum(1, 3, 5);
        EXPECT_TRUE(val == 1 + 3 + 5);  
    }
    {
        EXPECT_TRUE(Func(1, 2, 3) == 3);
    }
}