#include "gtest/gtest.h" 

namespace Parameter_1 {
    // T, U : 인자(Parameter) 집합
    template<typename T, typename U> 
    class A {};
}
namespace Parameter_2 {
    template<typename T, typename U>
    class A {
    public:
        T f(U param) {return param;}
    };    
}
namespace Parameter_3 {
    template<typename T, T val>
    class A {
    public:
        T f() {return val;} // val을 사용할 수 있습니다.
    };
}
namespace Parameter_4 {
    template<typename T, int val>
    class A {
    public:
        T f() {return val;}
    };
}
namespace Parameter_5 {
    template<typename T> 
    class A {
    public:
        int m_X;
    };
 
    template<typename T> 
    class A<T*> { // A의 템플릿 특수화 버전
    public: 
        long m_Y;
    };
 
    // U : 템플릿 템플릿 인자. 인스턴스화된 타입이 아닌 템플릿을 전달함
    template<template<typename> typename U>
    class B {
    public:
        U<int> m_U1; // 템플릿인 U로 부터 개체 생성
        U<int*> m_U2;
    };
}
namespace Parameter_6 {
    class A; // 전방 선언
    typedef class {} B; // 이름없이 클래스를 typedef

    template<typename T>
    class C {}; 
}
namespace Parameter_7 {
    // template<bool b = 3 > 4> // (X) 컴파일 오류. 
    // class A {};

    template<bool b = (3 > 4)> // (O)
    class B {};   
}
namespace Parameter_8 {

    template<typename T = char, typename U = int>
    class A {};
}
namespace Parameter_9 {
    template<typename T>
    class A {};

    template<typename T>
    class B {
    public:
        typedef T Type; // 종속 타입
        static int m_Static; // 정적 멤버 변수
    };

    // 템플릿 클래스
    template<typename T>
    class C : B<T> {
        // B<T>::Type m_Val; // (X) 컴파일 오류. B<T>::Type이 static 변수인지, B<T>에 종속된 타입인지 모릅니다.
        typename B<T>::Type m_Val; // (O) 
        
        void f() {
            B<T>::m_Static; // 정적 멤버 변수 접근
        } 
    };

    // 템플릿 템플릿 인자
    template<template<typename> typename T> 
    class D {
        // T<int>::Type m_Val; // (X) 컴파일 오류. B<T>::Type이 static 변수인지, B<T>에 종속된 타입인지 모릅니다. 
        typename T<int>::Type m_Val; // (O) 
    };

    // 템플릿 함수
    template<typename T>
    // void f(B<T>::Type val) {} // (X) 컴파일 오류. B<T>::Type이 static 변수인지, B<T>에 종속된 타입인지 모릅니다. 
    void f(typename B<T>::Type val) {} // (O)
}

namespace Parameter_10 {
    template<typename T>
    class A {
    public:
        template<typename U>
        void f() {}
    };
    
    template<typename T>
    void g() {
        A<T> a; // 아직 구체 타입으로 인스턴스화 안됐습니다.
        // a.f<T>(); // (X) 아직 인스턴스화 되지 않아 함수 이름에 접근할 수 없습니다.
        a.template f<T>(); // (O) 템플릿임을 명시합니다.
    }
}

namespace Paramenter_11 {
    template<typename T> // #1
    class A {};

    // template<typename U> // (X) 컴파일 오류. #1과 동등
    // class A {};
}


TEST(TestClassicCpp, TemplateParameterArgument) {

    {
        using namespace Parameter_1;

        // int, char : 인자에 대응되는 인수(Argument) 집합
        A<int, char> a;     
    }
    // 타입
    {
        using namespace Parameter_2;

        A<int, char> a;
        EXPECT_TRUE(a.f('a') == static_cast<int>('a'));        
    }
    // 템플릿 개체
    {
        using namespace Parameter_3;

        A<int, 10> a;
        EXPECT_TRUE(a.f() == 10);
    }
    // 비 템플릿 개체
    {
        using namespace Parameter_4;

        A<int, 10> a;
        EXPECT_TRUE(a.f() == 10);

        A<int, 11> b;
        // a = b; // (X) 컴파일 오류. A<int, 10> 은 A<int, 11> 과 타입이 다릅니다.
    }
    // 템플릿 템플릿 인자
    {
        using namespace Parameter_5;
        B<A> b; 
        b.m_U1.m_X = 10;
        b.m_U2.m_Y = 20;

        EXPECT_TRUE(b.m_U1.m_X == 10);
        EXPECT_TRUE(b.m_U2.m_Y == 20);
    }
    // 불완전한 형식의 인스턴스화 
    {
        using namespace Parameter_6;
        C<A> x1;  // (O) A는 전방선언만 한 클래스 개체
        C<A*> x2; // (O)
        C<B> x3;  // (O) B는 이름 없는 클래스의 typedef  
    }
    // 기본값 인자
    {
        using namespace Parameter_8;
        A<> a; // T == char, U == int
        A<char> b; // U == int
        A<char, char> c;
    }

}
