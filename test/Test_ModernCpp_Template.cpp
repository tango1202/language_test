#include "gtest/gtest.h" 
#include <cstdarg>

namespace {
#if 201402L <= __cplusplus // C++14~        
    // C++14 변수 템플릿
    template<class T>
    // constexpr T pi_14{3.1415926535897932385L}; // (X) 컴파일 오류. 중괄호 초기화는 암시적 형변환이 안되서 = 로 초기화 합니다.
    constexpr T pi_14 = 3.1415926535897932385L; 
#endif

#if 201402L <= __cplusplus // C++14~    
    // C++14 변수 템플릿. 팩토리얼 
    
    // 1을 뺀 값을 재귀 호출합니다.
    template<int val> 
    constexpr int factorial_14 = val * factorial_14<val - 1>; 

    // 1일때 특수화 버전. 더이상 재귀 호출을 안합니다.
    template<>
    constexpr int factorial_14<1> = 1;
#endif
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
// 클래스 템플릿 인수 추론 사용자 정의 가이드
namespace Template_1_1 {
    template<typename T>
    class A {
    public:
        A(T x, T y) {}
    };    
}
// 클래스 템플릿 인수 추론 사용자 정의 가이드
namespace Template_1_2 {
    template<typename T>
    class A {
    public:
        A(T x, T y) {}
    };  
#if 201703L <= __cplusplus // C++17~  
    // 클래스 템플릿 인수 추론 사용자 정의 가이드
    template<typename T, typename U>
    A(T x, U y) -> A<U>; // T와 U가 타입이 다르다면 U 타입으로 추론하세요. 
#endif    
}
// auto 템플릿 인자
namespace Template_2 { 
    template<typename T, T val> // 타입과 개체를 전달받습니다.
    class A {
    public:
        T GetVal() {return val;} 
    };

#if 201703L <= __cplusplus // C++17~
    template<auto val> // 비타입 템플릿 인자인 경우 auto를 사용할 수 있습니다.
    class A_17 {
    public:
        auto GetVal() const {return val;}
    };   
#endif    
}
namespace Template_3 {
    template<int val>
    class A {};

    enum MyEnum{Val};
    template<enum MyEnum myEnum>
    class B {};

    class MyClass {};
    template<MyClass* ptr>
    class C {};

    template<MyClass& ref>
    class D {};  

    MyClass g_MyClass;
    
}
namespace Template_4 {
#if 202002L <= __cplusplus // C++20~ 
    template<double d> 
    class A_20 {};
    
    class MyClass_11 {
    public: 
        int m_Val; // 멤버 변수는 public이고, mutable이 아니어야 합니다.
    public: 
        constexpr explicit MyClass_11(int val) : m_Val(val) {} // constexpr 생성자여야 합니다.
    };
    template<MyClass_11 myClass> 
    class B_20 {};
#endif
}

namespace Template_5 {
#if 202002L <= __cplusplus // C++20~ 
    // 축약된 함수 템플릿

    // template<typename T> 
    // void f(T param);
    void f_20(auto param) {} 

    // template<typename T, typename U>
    // void g(T param1, U param2);
    void g_20(auto param1, auto param2) {} // param1과 param2는 동일한 타입이라는 보장이 없습니다.

    // template<typename... Params>
    // void h_11(Params... params);
    void h_20(auto... params) {} // 가변 템플릿도 지원합니다.

    // template<typename T, typename U>
    // void i(T param1, U param2) {}
    template<typename T>
    void i_20(T param1, auto param2) {} // 템플릿 내에서 혼합해서 사용할 수 있습니다.
#endif
}

namespace Template_5_1 {
#if 202002L <= __cplusplus // C++20~     
    int Sum_20(auto count, ...) { // 가변 인자를 사용하는 함수입니다.

        int result = 0;
        std::va_list paramList; // 가변 인자
        va_start(paramList, count); // 가변 인자 처리 시작
        for (int i = 0; i < count; ++i) {
            result += va_arg(paramList, int); // 가변 인자 추출
        }
        va_end(paramList); // 가변 인자 처리 끝
        return result;       
    }
#endif
}

namespace Template_6 {
    template<typename T>
    class A {
    public:
        explicit A(T val) {}
    };
}

TEST(TestModern, Template) {
    // 축약된 함수 템플릿
    {
        using namespace Template_5_1;
#if 202002L <= __cplusplus // C++20~           
        EXPECT_TRUE(Sum_20(3, 1, 2, 3) == 1 + 2 + 3);
#endif        
    }
#if 201402L <= __cplusplus // C++14~        
    // C++14 변수 템플릿
    {
        // pi_14를 타입에 따라 서로 다른 정밀도로 사용할 수 있습니다.
        std::cout << "pi_14<double> : " << std::setprecision(10) << pi_14<double> << std::endl;
        std::cout << "pi_14<float> : " << std::setprecision(10) << pi_14<float> << std::endl;
        std::cout << "pi_14<int> : " << std::setprecision(10) << pi_14<int> << std::endl;
    } 
#endif 

#if 201402L <= __cplusplus // C++14~    
    // C++14 변수 템플릿. 팩토리얼 
    {
        EXPECT_TRUE(factorial_14<5> == 5 * 4 * 3 * 2 * 1);
    }
#endif
    
    // (C++17~) 클래스 템플릿 인자 추론
    {
        using namespace Template_1;
        EXPECT_TRUE(Func(1, 2) == 3); // 인수 1, 2 로부터 추론해서 사용합니다.   
       
        A<int> a{10}; // ~C++17 이전에는 타입을 명시해야 합니다.
        EXPECT_TRUE(a.Func(1, 2) == 3);
#if 201703L <= __cplusplus // C++17~       
        A a_17{10}; // C++17 이후부터는 인수로부터 추론합니다.
        EXPECT_TRUE(a_17.Func(1, 2) == 3);    
#endif         
    }
    {
        using namespace Template_1_1;
#if 201703L <= __cplusplus // C++17~            
        A a_17{1, 2}; // A<int>. 인수로부터 인자를 추론합니다.
        // A b_17{1, 1.5}; // (X) 컴파일 오류. A<int, double>은 안됩니다.
#endif          
        A<double> c{1, 1.5}; // 명시적으로 A<double>을 사용합니다.
    } 
    {
        using namespace Template_1_2;

#if 201703L <= __cplusplus // C++17~     
        A a_17{1, 2}; 
        A b_17{1, 1.5}; // (O) 클래스 템플릿 인수 추론 사용자 정의 가이드에 의해 A<double>로 추론합니다.
#endif        
        A<double> c{1, 1.5};
    } 
       
    {
        using namespace Template_2;

        A<int, 10> a{};
        EXPECT_TRUE(a.GetVal() == 10);
        
#if 201703L <= __cplusplus // C++17~
        A_17<10> a_17{};
        EXPECT_TRUE(a_17.GetVal() == 10);
#endif
    }
    // 축약된 함수 템플릿
    {
#if 202002L <= __cplusplus // C++20~ 
        using namespace Template_5;

        f_20(10);
        g_20(10, 3.14);
        h_20(10, 3.14, "Hello");
        i_20(10, 3.14);
#endif
    }     
    // 템플릿 인자에 타입이 아닌 개체 지원 확장
    {
        using namespace Template_3;

        A<10> a; // 정수 타입
        B<Val> b; // 열거형의 열거자
        C<&g_MyClass> c; // 전역, 정적 개체의 포인터
        D<g_MyClass> d; // 전역, 정적 개체의 참조자
    }
    {
#if 202002L <= __cplusplus // C++20~ 
        using namespace Template_4;

        A_20<3.14> a; // 실수 타입
        B_20<MyClass_11{1}> b; // 멤버 변수는 public이고, mutable이 없으며, constexpr 생성자가 있는 리터럴 타입
#endif
    }
    // initializer_list 사용시 클래스 템플릿 인수 추론
    {
        using namespace Template_6;

        A<int> a{10}; // ~C++17 이전에는 타입을 명시해야 합니다.
#if 201703L <= __cplusplus // C++17~       
        A a_17{10}; // C++17 부터는 인수 타입으로부터 추론합니다.
#endif

        std::vector<int> v_11{1, 2, 3}; // ~C++20 이전에는 initializer_list로 초기화시에 타입을 명시해야 합니다.
#if 202002L <= __cplusplus // C++20~         
        std::vector v_20{1, 2, 3}; // C++20 부터는 initializer_list로 초기화시에 타입을 명시하지 않아도 됩니다.
#endif
    }
}
