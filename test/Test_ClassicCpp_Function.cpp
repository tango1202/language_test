#include "gtest/gtest.h" 
#include "Test_Inline.h"
#include <cstdarg>

namespace {
    // (X) 컴파일 오류. 배열을 리턴값으로 사용할 수 없습니다.
    // int [] f() {
    //     int a[3] = {0,};
    //     return a;
    // }

    // ----
    // 함수 포인터
    // ----
    void f(int) {} // f 함수 정의

    int plus(int a, int b) {
        return a + b;
    }
    int minus(int a, int b) {
        return a - b;
    }
    // ----
    // 기본값 인자
    // ----  
    int f1(int a = 10) { // 인수를 전달하지 않으면 기본값 10
        return a;
    } 
    // 선언부
    int f2(int a, int b = 20, int c = 30); // (O)
    // int f2(int a, int b = 20, int c); // (X) 컴파일 오류. c에도 기본값을 줘야 합니다.

    // 정의부
    int f2(int a, int b /*= 20*/, int c /*= 30*/) { // (O) 선언과 정의 분리시, 정의부는 기본값 작성 안함.
        return 0;
    }

    class T {
    //    void f3(T* p = this) {} // (X) 컴파일 오류. this는 기본값을 사용할 수 없음
    };

    // 전역 변수 기본값
    int g_Val = 10;
    int f4(int val = g_Val) { // g_Val의 런타임 호출 시점의 값이 사용됩니다.
        return val;
    }

    // 함수 기본값
    int g() {return g_Val;}
    int f5(int val = g()) { // g()의  런타임 호출 시점의 값이 사용됩니다.
        return val;
    }
}
// ----
// 이름 탐색 규칙 - ADL(Argument-dependent lookup), Keonig 
// ----
namespace A {
    class Date {};
    int MyFunc(int) {return 1;}
} 
namespace B { 
    int MyFunc(double) {return 2;}
    int g() {
        A::Date d;
        // 현 유효 범위에서 MyFunc(double)을 찾음. 1을 double로 암시적으로 변환함
        return MyFunc(1); 
    }
}
namespace C {
    class Date {};
    int MyFunc(const Date&, int) {return 1;}
} 
namespace D { 
    int MyFunc(const C::Date&, double) {return 2;}
    int g() {
        C::Date d;
        // Koenig 검색이 활용됨. 
        // 인자로 전달한 d 가 네임스페이스 C에 있기 때문에, C의 함수들중 MyFunc(const Date&, int)를 찾아냄
        // MyFunc(const Date&, int) 과 MyFunc(const C::Date&, double) 중 타입이 일치하는 MyFunc(const Date&, int)이 채택됨
        return MyFunc(d, 1); 
    }
}

TEST(TestClassicCpp, Function) {
    // ----
    // 함수 포인터
    // ----
    {
        void (*p)(int); // void 를 리턴하고 int형을 인수로 전달받는 함수의 함수 포인터 p 정의

        p = f; // 함수 포인터에 f 함수 대입. p = &f; 와 동일
        p(10); // f 함수 실행. (*p)(10); 과 동일
    }
    {
        typedef void (*Func)(int); // void 를 리턴하고 int형을 인수로 전달받는 함수의 함수 포인터 타입 정의

        Func p; // 함수 포인터 p 정의
        p = f; // 함수 포인터에 f 함수 대입. p = &f; 와 동일
        p(10); // f 함수 실행. (*p)(10); 과 동일        
    }
    {
        typedef int (*Func)(int, int); // 함수 포인터 typedef
        class Button {
        private: 
            Func m_Func; // 버튼이 클릭될때 실행될 함수
        public:
            explicit Button(Func func) : // 생성시 실행할 함수 포인터 전달
                m_Func(func) {}
            int Click(int a, int b) { 
                return m_Func(a, b); // 함수 포인터 실행
            }
        };

        // 버튼 생성시 어떤 연산을 수행할지 함수를 전달해 둡니다.
        Button plusButton(plus); // 클릭시 더하기
        Button minusButton(minus); // 클릭시 빼기

        EXPECT_TRUE(plusButton.Click(10, 20) == 30); 
        EXPECT_TRUE(minusButton.Click(10, 20) == -10); 
    }
    // 멤버 함수 포인터
    {
        class Data { 
        public: 
            int Print() const {return 1;} 
        };

        typedef int (Data::*Func)() const; // Data 클래스 멤버 함수 typedef
        Func func = &Data::Print;  // 멤버 함수 포인터 전달

        Data data;
        EXPECT_TRUE((data.*func)() == 1); // data 개체로 부터 멤버 함수 포인터 실행
    }
    {
        class Data { 
        public: 
            int Print() const {return 1;}
            int Preview() const {return 2;}
        };
        typedef int (Data::*Func)() const; // Data 클래스 멤버 함수 typedef
        
        class Button {
        private: 
            const Data& m_Data; // 버튼이 관리하는 Data
        public:
            explicit Button(const Data& data) :
                m_Data(data) {}
            int Click(Func func) { 
                return (m_Data.*func)(); // 전달된 멤버 함수 포인터 실행
            }
        };

        Data data;
        Button button(data);

        EXPECT_TRUE(button.Click(&Data::Print) == 1); // data 개체로 부터 Print 함수 실행
        EXPECT_TRUE(button.Click(&Data::Preview) == 2); // data 개체로 부터 Preview 함수 실행
    } 
    // ----
    // 리턴
    // ----  
    {
        class T {
            int m_Val;

            void f() {} // 아무것도 리턴 안함
            int g() {return 0;} // 정수값을 리턴함
            const int& h() const {return m_Val;} // 멤버 변수의 참조자를 리턴함
        };        
    }
    // ----
    // RVO
    // ----
    {
        class T {
            int m_X;
            int m_Y;
        public:
            // 값 생성자
            T(int x, int y) :
                m_X(x),
                m_Y(y) {
                std::cout << "RVO -> T::T()" << std::endl;
            }

            // 복사 생성자
            T(const T& other) {
                std::cout << "RVO -> T(const T& other)" << std::endl;    
            }
            
            T f() {
                T result(0, 0);
                return result;
            }
        };

        T t1(0, 0);
        T t2(t1.f()); // T t2 = t1.f(); 와 동일
    } 
    // ----
    // 가변 인자
    // ----
    {
        class T {
        public:
            static int Sum(int count, ...) {
                int result = 0;
                std::va_list paramList; // 가변 인자
                va_start(paramList, count); // 가변 인자 처리 시작
                for (int i = 0; i < count; ++i) {
                    result += va_arg(paramList, int); // 가변 인자 추출
                }
                va_end(paramList); // 가변 인자 처리 끝
                return result;       
            }
        };
        EXPECT_TRUE(T::Sum(3, 1, 2, 3) == 1 + 2 + 3);
    }
    {
        EXPECT_TRUE(f1() == 10); // 아무값도 주지 않으면 a 는 10
        EXPECT_TRUE(f1(20) == 20);
    }
    // 전역 변수 기본값
    {
        EXPECT_TRUE(g_Val == 10);
        EXPECT_TRUE(f4() == 10);
        g_Val = 20;
        EXPECT_TRUE(f4() == 20);
    }
    // 함수 기본값
    {
        g_Val = 10;
        EXPECT_TRUE(f5() == 10);
        g_Val = 20;
        EXPECT_TRUE(f5() == 20);        
    }
    // 상속관계에서 기본값 제정의
    {
        class Base {
        public:
            virtual int f6(int val = 10) const {
                return val;
            }
        };
        class Derived : public Base {
        public:
            virtual int f6(int val = 20) const { // (△) 비권장. Base의 디폴트 값과 다르게 했습니다. 혼란스러워 질 수 있습니다.
                return val;
            }
        };
        {
            Base b;
            Derived d;
            EXPECT_TRUE(b.f6() == 10);   
            EXPECT_TRUE(d.f6() == 20);    
        }
        {
            Derived d;
            Base* p = &d;
            EXPECT_TRUE(p->f6() == 10); // vtable을 참조하여 Base 의 기본값인 10을 사용합니다.   
        }
        // 오버로딩된 함수 호출 규칙
        {
            class T {
            public:
                int f(int) {return 1;}
                int f(double) {return 2;}
            };

            T t;
            EXPECT_TRUE(t.f(1) == 1); // (O) 타입 일치. int 버전 호출
            EXPECT_TRUE(t.f(1.) == 2); // (O) 타입 일치. double 버전 호출
            // EXPECT_TRUE(t.f(1L) == 1); // (X) 컴파일 오류. long 버전이 없음
            EXPECT_TRUE(t.f(1.F) == 2); // (△) 비권장. float 버전이 없지만, double로 암시적 형변환 되므로 double 버전 호출
        }
        // 오버로딩된 함수 호출 규칙 - 배열, 최상위 const
        {
            class T {
            public:
                
                int f(int) {return 1;}
                int f(int* a) {return 2;} 

                // (X) 컴파일 오류. 배열은 f(int* a)와 동일해서 오버로딩 안됨.
                // int f(int a[3]) {return 3;}
                // int f(int a[]) {return 4;}

                // (X) 컴파일 오류. int f(int) 와 int f(const int) 는 동일해서 오버로딩 안됨.
                // int f(const int) {return 5;}

                // (X) 컴파일 오류. f(int* a)와 f(int* const a)는 동일해서 오버로딩 안됨.
                // int f(int* const a) {return 6;}

                // (O) f(int)와 f(const int) 는 동일해서 오버로딩 안되지만, 
                // f(int* a)와 f(const int* a)는 다르므로(포인터 자체가 const가 아니라 가리키는 곳이 const임) 오버로딩 됨
                int f(const int* a) {return 7;}

                // (O) 함수 상수성에 따라 선택됨
                int f(int) const {return 8;} 

                // (X) 컴파일 오류. 리턴 타입은 오버로딩 함수를 취급하는데 사용하지 않습니다.
                // double f(int) {return 9.};
            };

            T t;
            EXPECT_TRUE(t.f(1) == 1); // (O) 타입 일치. int 버전 호출
            int a = 10;
            EXPECT_TRUE(t.f(&a) == 2); // (O) 타입 일치. int* 버전 호출
            int arr[3] = {1, 2, 3};
            EXPECT_TRUE(t.f(arr) == 2); // (O) 배열은 int* 버전 호출
            int* const p = &a;
            EXPECT_TRUE(t.f(p) == 2); // (O) int* const는 int* 버전 호출
            EXPECT_TRUE(t.f(const_cast<const int*>(&a)) == 7); // (O) const int* 는 const int* 버전 호출
            EXPECT_TRUE(const_cast<const T&>(t).f(1) == 8); // (O) 개체 상수성에 따라 상수 함수 선택됨
        }
        // 이름 탐색 규칙 - ADL(Argument-dependent lookup), Keonig 
        {
            EXPECT_TRUE(B::g() == 2); // B::MyFunc 이 채택됨
            EXPECT_TRUE(D::g() == 1); // C::MyFunc 이 채택됨
        }
    }
}

