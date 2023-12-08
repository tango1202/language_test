#include "gtest/gtest.h" 

namespace Parameter_1 {
    // T, U : 인자(Parameter) 집합
    template<typename T, typename U> 
    class A {};
}
namespace Parameter_1_1 {
    // 인자를 사용하지 않는다면, 인자명을 생략할 수 있습니다.
    template<typename, typename> 
    class A {};
}
namespace Parameter_2 {
    template<typename T, typename U>
    class A {
    public:
        T f(U param) {return param;} // U를 T로 암시적으로 형변환 합니다.
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
    class A { // #1
    public:
        int m_X;
    };
 
    template<typename T> 
    class A<T*> { // #2. A의 템플릿 특수화 버전
    public: 
        long m_Y;
    };
 
    // U : 템플릿 템플릿 인자. 인스턴스화 하지 않은 템플릿을 전달함
    template<template<typename> typename U>
    class B {
    public:
        U<int> m_U1; // #1로 개체 정의
        U<int*> m_U2; // #2로 개체 정의
    };
}
namespace Parameter_6 {
    class A; // 전방 선언
    typedef class {} B; // 이름없이 클래스를 typedef

    template<typename T>
    class C {}; 
}
namespace Parameter_7 {

    template<typename T = char, typename U = int>
    class A {};

    // template<typename T = char, typename U> // (X) 컴파일 오류. T가 기본값을 사용했기 때문에 U도 기본값을 사용해야 합니다.
    // class B {};    
}
namespace Parameter_8 {
    template<typename T>
    class A {};

    template<typename T>
    class B {
    public:
        typedef T Type; // 종속 타입
        static int m_Static; // 정적 멤버 변수
    };

    // 클래스 템플릿
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

    // 함수 템플릿
    template<typename T>
    // void f(B<T>::Type val) {} // (X) 컴파일 오류. B<T>::Type이 static 변수인지, B<T>에 종속된 타입인지 모릅니다. 
    void f(typename B<T>::Type val) {} // (O)
}
namespace Parameter_9 {
    // template<bool b = 3 > 4> // (X) 컴파일 오류. 
    // class A {};

    template<bool b = (3 > 4)> // (O)
    class B {};   
}
namespace Parameter_10 {
    template<typename T> 
    class Runner {
        T m_Obj;
    public:
        int Run() {
            return m_Obj.f(10); // 템플릿 정의에서는 함수 정의가 없어도 컴파일 됩니다.  
        }
    };
     
    class A {
    public:
        template<typename U>
        int f(U val) {return 1;} // 멤버 함수 템플릿입니다.
    };
    class B {
    public:
        template<typename U>
        int f(U val) {return 2;} // 멤버 함수 템플릿입니다.
    };
}

namespace Parameter_11 {
    template<typename T> 
    class Runner {
        T m_Obj;
    public:
        int Run() {
            // (X) 컴파일 오류. 멤버 함수 템플릿을 명시적으로 호출하기 위해 <>을 사용하면, 
            // < 을 비교 연산으로 파싱해서 컴파일 오류가 납니다.
            // return m_Obj.f<int>(10);   

            // (O) < 가 템플릿으로 파싱되도록 template 키워드를 작성했습니다.
            return m_Obj.template f<int>(10);
        }
    };
     
    class A {
    public:
        template<typename U>
        int f(U val) {return 1;} // 멤버 함수 정의 입니다.
    };
    class B {
    public:
        template<typename U>
        int f(U val) {return 2;} // 멤버 함수 정의 입니다.
    };
}
namespace Paramenter_12 {
    template<typename T> // #1.
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
    {
        using namespace Parameter_1_1;

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
        b.m_U2.m_Y = 20; // A<T*> 버전으로 생성됩니다.

        EXPECT_TRUE(b.m_U1.m_X == 10);
        EXPECT_TRUE(b.m_U2.m_Y == 20);
    }
    // 불완전한 형식의 인스턴스화 
    {
        using namespace Parameter_6;
        C<A> x1;  // (O) A는 전방 선언만 한 클래스 개체
        C<A*> x2; // (O)
        C<B> x3;  // (O) B는 이름 없는 클래스의 typedef  
    }
    // 기본값 인자
    {
        using namespace Parameter_7;
        A<> a; // T == char, U == int
        A<char> b; // U == int
        A<char, char> c;
    }
    {
        using namespace Parameter_10;

        Runner<A> a;
        Runner<B> b;

        EXPECT_TRUE(a.Run() == 1);
        EXPECT_TRUE(b.Run() == 2); 
    }    
}
