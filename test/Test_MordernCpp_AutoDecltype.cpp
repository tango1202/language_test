#include "gtest/gtest.h" 

namespace Auto {
    // (X) 컴파일 오류
    // double Func(int a, auto b) {
    //     return a + b;   
    // }
}

namespace Decltype_1 {
    template<typename T, typename U>
    void Func(T a, U b, decltype(a + b)* result) { // decltype(a + b) 은 int
        *result = a + b;
    }  
}
namespace Decltype_2 {
    // C++11
    template<typename T, typename U>
    auto Add(T a, U b) -> decltype(a + b) { 
        return a + b;
    } 
    // C++14
    // template<typename T, typename U>
    // decltype(auto) Add(T a, U b) { 
    //     return a + b;
    // }
    // C++14
    // template<typename T, typename U>
    // auto Add(T a, U b) { 
    //     return a + b;
    // } 
}
namespace Decltype_3 {
    int Func(int a, int b) {
        return a + b;
    } 
}
namespace Decltype_4 {
    // C++14
    // auto Add1(int a, int b) {
    //     return a + b;
    // }
    // auto Add2(int a, int b) {

    //     const int result = a + b;
        
    //     // 템플릿 함수 인수 추론에 의해 const int 대신 int가 사용됩니다.
    //     return result;
    // }

    // decltype(auto) Add3(int a, int b) {
    //     const int result = a + b;

    //     // 개체 엑세스로 평가. result 타입 그대로 평가
    //     return result; 
    // }
    // decltype(auto) Add4(int a, int b) {
    //     const int result = a + b; // (X) 예외 발생. Func4의 지역 변수 참조를 전달하기 때문

    //     // 왼값 표현식의 결과로 평가. T&형태로 평가
    //     return (result); 
    // }
    // (X) 컴파일 오류. 리턴 타입은 동일해야 합니다.
    // auto Add5(int a, int b) {
    //     if (a < 10) {
    //         return 10; // int
    //     }
    //     else {
    //         return 10.0F; // float
    //     }
    // }
    // class T {
    // public:
    //     // (X) 컴파일 오류. 가상 함수는 리턴 타입 추론을 할 수 없습니다.
    //     virtual auto Add(int a) {return a;}
    // };
}
TEST(TestMordern, Auto) {
    {
        int a = 0;
        const int b = 0;

        auto c = a; // int로 추론됨
        auto d = b; // 최상위 const는 무시되므로 const int 가 아닌 int로 추론됨
        d = 10;   

        std::vector<int> v;

        // std::vector<int>::iterator itr = v.begin(); 
        // std::vector<int>::iterator itrEnd = v.end();

        auto itr = v.begin(); // 템플릿 사용에 따른 긴 타입명 간소화
        auto itrEnd = v.end();
    }
}
TEST(TestMordern, Decltype) {
    {
        int a = 0;
        const int b = 0;

        decltype(a) c = a; // a와 동일한 int로 추론됨
        decltype(b) d = a; // auto와는 다르게 b와 동일한 const int로 추론됨
        // d = 10; // (X) 컴파일 오류. const int이므로 값대입 안됨
    }
    {
        class T {
        public:
            double m_Val;
        };
        const T* t;

        decltype(t->m_Val) a = 10; // 멤버 엑세스로 평가됩니다. double
        decltype((t->m_Val)) b = 10; // 괄호를 추가하면 왼값 표현식으로 처리합니다. t가 const 이므로 const double&
    }
    // 함수 인자
    {
        using namespace Decltype_1;
        int result = 0;
        Func(10, 20, &result);
        EXPECT_TRUE(result == 30);        
    }

    // 후행 리턴 타입
    {
        using namespace Decltype_2;
        auto result = Add(10, 20);
        EXPECT_TRUE(result == 30);  
    }
    // decltype(auto)
    {
        using namespace Decltype_3;

        // Func(10, 20) 함수 결과 타입
        decltype(Func(10, 20)) c = Func(10, 20); // C++11
        // decltype(auto) d = Func(10, 20); // C++14    
    }
    // 리턴타입 추론
    {
        using namespace Decltype_4;

        // C++14
        // auto result1 = Add1(10, 20); // int를 리턴
        // auto result2 = Add2(10, 20); // const int를 리턴했지만 템플릿 함수 인수 추론 규칙에 따라 int를 리턴
        // auto result3 = Add3(10, 20); // const int 리턴. 리턴하는 result 타입과 동일
        // auto result4 = Add4(10, 20); // const int& 리턴. 리턴하는 (result) 표현식과 동일. 
    }

}
