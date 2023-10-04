#include "gtest/gtest.h" 

namespace {
    // C++14 변수 템플릿
    template<class T>
    // constexpr T pi{3.1415926535897932385L}; // 중괄호 초기화는 암시적 형변환이 안되서 = 로 초기화 합니다.
    constexpr T pi = 3.1415926535897932385L; 

    // C++14 변수 템플릿. 팩토리얼 
    
    // 1을 뺀 값을 재귀 호출합니다.
    template<int val> 
    constexpr int factorial = val * factorial<val - 1>; 

    // 1일때 특수화 버전. 더이상 재귀 호출을 안합니다.
    template<>
    constexpr int factorial<1> = 1;
}

TEST(TestMordern, VariableTemplate) {
    // C++14 변수 템플릿
    {
        // 동일한 값을 타입에 따라 서로 다른 정밀도로 사용할 수 있습니다.
        std::cout << "pi<double> : " << std::setprecision(10) << pi<double> << std::endl;
        std::cout << "pi<float> : " << std::setprecision(10) <<pi<float> << std::endl;
        std::cout << "pi<int> : " << std::setprecision(10) <<pi<int> << std::endl;
    } 
    // C++14 변수 템플릿. 팩토리얼 
    {
        EXPECT_TRUE(factorial<5> == 5 * 4 * 3 * 2 * 1);
    }
}