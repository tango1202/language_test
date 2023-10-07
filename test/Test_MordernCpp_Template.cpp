#include "gtest/gtest.h" 

namespace {
    // C++14 변수 템플릿
    template<class T>
    // constexpr T pi_14{3.1415926535897932385L}; // 중괄호 초기화는 암시적 형변환이 안되서 = 로 초기화 합니다.
    constexpr T pi_14 = 3.1415926535897932385L; 

    // C++14 변수 템플릿. 팩토리얼 
    
    // 1을 뺀 값을 재귀 호출합니다.
    template<int val> 
    constexpr int factorial_14 = val * factorial_14<val - 1>; 

    // 1일때 특수화 버전. 더이상 재귀 호출을 안합니다.
    template<>
    constexpr int factorial_14<1> = 1;

}
namespace Template_1 { 
    // (C++17~) 클래스 템플릿 인자 추론
    template<typename T>
    T Func(T a, T b) {return a + b;}

    template<typename T>
    class A {
    public:
        explicit A(T val) {}
        T Func(T a, T b) {return a + b;}
    };
}
// auto 템플릿 인자
namespace Template_2 { 
    template<int Num>
    class A {
    public:
        int GetNum() const {return Num;}
    };

    

    template<auto Num>
    class A_17 {
    public:
        auto GetNum() const {return Num;}
    };    
}

TEST(TestMordern, Template) {
    // C++14 변수 템플릿
    {
        // 동일한 값을 타입에 따라 서로 다른 정밀도로 사용할 수 있습니다.
        std::cout << "pi_14<double> : " << std::setprecision(10) << pi_14<double> << std::endl;
        std::cout << "pi_14<float> : " << std::setprecision(10) <<pi_14<float> << std::endl;
        std::cout << "pi_14<int> : " << std::setprecision(10) <<pi_14<int> << std::endl;
    } 
    // C++14 변수 템플릿. 팩토리얼 
    {
        EXPECT_TRUE(factorial_14<5> == 5 * 4 * 3 * 2 * 1);
    }
    // (C++17~) 클래스 템플릿 인자 추론
    {
        using namespace Template_1;
        EXPECT_TRUE(Func(1, 2) == 3); // 인수 1, 2 로부터 추론해서 사용합니다.   
       
        A<int> a{10}; // ~C++17 이전에는 타입을 명시해야 합니다.
        EXPECT_TRUE(a.Func(1, 2) == 3);
       
        A a_17{10}; // C++17 이후부터는 인수로부터 추론합니다.
        EXPECT_TRUE(a_17.Func(1, 2) == 3);     
    }
    {
        using namespace Template_2;
        EXPECT_TRUE(A<10>{}.GetNum() == 10);
        EXPECT_TRUE(A_17<10>{}.GetNum() == 10);
    }

}
