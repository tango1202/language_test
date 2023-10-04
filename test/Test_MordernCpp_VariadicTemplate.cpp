#include "gtest/gtest.h" 

namespace VariadicTemplate_1 {
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

    // func(params...) 를 호출합니다.
    template<typename Func, typename... Params>
    int Forwarding(Func func, Params... params) {
        return func(params...);
    }

    int MySum(int a, int b, int c) {
        return a + b + c;
    }
}

namespace VariadicTemplate_2 {
    int Sum(int a, int b, int c) {
        return a + b + c;
    }
    template<typename... Params>
    int Func(Params... params) {
        return Sum((params + 1)...); // 파라메터 팩의 각 요소에 1을 더해 배포합니다.
    }        
}

TEST(TestMordern, VariadicTemplate) {
    {
        using namespace VariadicTemplate_1;
        int val{Sum(1, 3, 5)};
        EXPECT_TRUE(val == 1 + 3 + 5);  
    }
    {
        using namespace VariadicTemplate_1;
        EXPECT_TRUE(Func(1, 2, 3) == 3);
    }
    {
        using namespace VariadicTemplate_1;
        EXPECT_TRUE(Forwarding(MySum, 1, 2, 3) == 1 + 2 + 3);       
    }
    // 파라메터 팩 확장
    {
        using namespace VariadicTemplate_2;
        EXPECT_TRUE(Func(1, 2, 3) == 2 + 3 + 4);
    }
}