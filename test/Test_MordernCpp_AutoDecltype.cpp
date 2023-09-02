#include "gtest/gtest.h" 

namespace Auto {
    // (X) 컴파일 오류
    // double Func(int a, auto b) {
    //     return a + b;   
    // }
}
namespace Decltype_1 {
    int Func(int a, int b) {
        return a + b;
    } 
}
namespace Decltype_2 {
    template<typename T, typename U>
    void Func(T a, U b, decltype(a + b)* result) { // decltype(a + b) 은 int
        *result = a + b;
    }  
}
namespace Decltype_3 {
    // C++ 11
    template<typename T, typename U>
    auto Add(T a, U b) -> decltype(a + b) { 
        return a + b;
    } 
    // C++ 14
    // template<typename T, typename U>
    // decltype(auto) Add(T a, U b) { 
    //     return a + b;
    // }
    // C++ 14
    // template<typename T, typename U>
    // auto Add(T a, U b) { 
    //     return a + b;
    // } 
}
TEST(TestCppPattern, Auto) {
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
TEST(TestCppPattern, Decltype) {
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
        decltype((t->m_Val)) b = 10; // 괄호를 추가하면 표현식으로 평가됩니다. t가 const 이므로 const double
    }
    // decltype(auto)
    {
        using namespace Decltype_1;

        // Func(10, 20) 함수 결과 타입
        decltype(Func(10, 20)) c = Func(10, 20); // C++ 11
        // decltype(auto) d = Func(10, 20); // C++ 14    
    }
    // 함수 인자
    {
        using namespace Decltype_2;
        int result = 0;
        Func(10, 20, &result);
        EXPECT_TRUE(result == 30);        
    }
    // 리턴값 추론
    {
        using namespace Decltype_3;
        auto result = Add(10, 20);
        EXPECT_TRUE(result == 30);  
    }

}
