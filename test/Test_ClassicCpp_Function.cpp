#include "gtest/gtest.h" 
#include "Test_Inline.h"
#include <cstdarg>

namespace Function_0 {
    int f(int param); // 함수 선언 
                      // 인자명을 꼭 동일하게 맞출 필요는 없습니다. 
                      // 관례적으로 맞출 뿐입니다. 
                      // 즉, int f(int); 와 같이 하셔도 됩니다.
                      
    int g(int param) {return f(param);} // (O) 선언만 된 함수 사용
    int f(int param) {return param * 2;} // 함수 정의    
}
namespace Function_1 {
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

namespace Function_2 {
    int f(int) {return 1;} // #1
    int f(char) {return 2;} // #2
    int f(long) {return 3;} // #3
}
namespace Function_3 {
    int f(int) {return 1;}
    int f(double) {return 2;}
}
namespace Function_4 {
    int f(int* a) {return 1;} 
    // int f(int a[3]) {return 2;} // (X) 컴파일 오류. 배열은 f(int* a)와 동일해서 오버로딩 안됨.
    // int f(int a[]) {return 3;} // (X) 컴파일 오류. 배열은 f(int* a)와 동일해서 오버로딩 안됨.
}
namespace Function_5 {
    int f(int a) {return 1;} 
    // int f(const int a) {return 2;} // (X) 컴파일 오류. int f(int) 와 동일해서 오버로딩 안됨.
}
namespace Function_6 {
    int f(int) {return 1;}
    // double f(int) {return 2;} // (X) 컴파일 오류. 리턴 타입은 오버로딩 함수 결정에 사용하지 않음
}
namespace Function_7 {
    int f(const int& a) {return 1;} //int 타입, int& 타입, const int& 타입을 모두 받을 수 있습니다.
}
namespace Function_8 {
    int f(int& a) {return 1;} //int 타입, int& 타입을 받을 수 있습니다.
}
namespace Function_9 {
    int f(int& a) {return 1;} // int 타입, int& 타입을 받을 수 있습니다.
    int f(const int& a) {return 2;} // const int 타입, const int& 타입을 받을 수 있습니다.  
}
namespace Function_10 {
    int f(int a) {return 1;} // int 타입, const int 타입, int& 타입, const int& 타입을 받을 수 있습니다.
    int f(int& a) {return 2;} // int 타입, int& 타입을 받을 수 있습니다.
    int f(const int& a) {return 3;} // const int 타입, const int& 타입을 받을 수 있습니다.  
}
// 오버로딩 함수 탐색 규칙
namespace Function_10_1 {
    int f(int a) {return 1;}
    int g(int a) {return 2;}
    int h(double a) {return 3;}
}

namespace Function_11 {
    // int f(void, int); // (X) 컴파일 오류
    // int f(const void); // (X) 컴파일 오류  
    // int f(void*); // (O)
    // int f(const void*); // (O)
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
    // 함수 선언
    {
        using namespace Function_0;
        EXPECT_TRUE(g(10) == 20);
    }
    // ----
    // 함수 포인터
    // ----
    {
        using namespace Function_1;

        void (*p)(int); // void 를 리턴하고 int형을 인수로 전달받는 함수의 함수 포인터 p 선언

        p = f; // 함수 포인터에 f 함수 대입. p = &f; 와 동일
        p(10); // f 함수 실행. (*p)(10); 과 동일
    }
    {
        using namespace Function_1;

        typedef void (*Func)(int); // void 를 리턴하고 int형을 인수로 전달받는 함수의 함수 포인터 타입 정의

        Func p; // 함수 포인터 p 정의
        p = f; // 함수 포인터에 f 함수 대입. p = &f; 와 동일
        p(10); // f 함수 실행. (*p)(10); 과 동일        
    }
    {
        using namespace Function_1;

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
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T() {
                std::cout << "T : Default Constructor" << std::endl;
            }
            // 값 생성자
            T(int x, int y) :
                m_X(x),
                m_Y(y) {
                std::cout << "T : Value Constructor" << std::endl;
            }

            // 복사 생성자
            T(const T& other) {
                std::cout << "T : Copy Constructor" << std::endl;    
            }
            // 복사 대입 연산자
            T& operator =(const T& other) {
                std::cout << "T : Assign" << std::endl;    
                return *this;
            }    
            
            // 값 타입으로 생성합니다. RVO 가 적용됩니다.
            static T Create() { 
                T result(0, 0);
                return result;
            }

            // 포인터를 생성해서 리턴합니다. 호출한 곳에서 delete 해줘야 합니다.
            static T* CreatePtr() {
                return new T(0, 0);
            }

            // 생성된 개체에 복사 대입 합니다. 포인터라서 널검사가 필요합니다. NULL 이면 예외를 방출합니다.
            static void Create(T* ptr) {
                if (ptr == NULL) {
                    throw std::invalid_argument("NULL"); 
                }

                *ptr = T(0, 0);
            }

            // 생성된 개체에 복사 대입 합니다.
            static void Create(T& ref) {
                ref = T(0, 0);
            }
        };

        T a = T::Create(); // 리턴값 최적화로 복사하지 않습니다.

        T* b = T::CreatePtr(); // (△) 비권장. 생성한 포인터만 복사합니다. 포인터이기 때문에 나중에 delete 해야 합니다.
        delete b;

        T c; 
        T::Create(&c); // (△) 비권장. 생성 후 대입 받으면, 기본 생성자, 값 생성자, 복사 대입 연산자가 호출됩니다.

        T d;
        T::Create(d); // (△) 비권장. 생성 후 대입 받으면, 기본 생성자, 값 생성자, 복사 대입 연산자가 호출됩니다.
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
        using namespace Function_1;

        EXPECT_TRUE(f1() == 10); // 아무값도 주지 않으면 a 는 10
        EXPECT_TRUE(f1(20) == 20);
    }
    // 전역 변수 기본값
    {
        using namespace Function_1;

        EXPECT_TRUE(g_Val == 10);
        EXPECT_TRUE(f4() == 10);
        g_Val = 20;
        EXPECT_TRUE(f4() == 20);
    }
    // 함수 기본값
    {
        using namespace Function_1;
        g_Val = 10;
        EXPECT_TRUE(f5() == 10);
        g_Val = 20;
        EXPECT_TRUE(f5() == 20);        
    }
    // 상속관계에서 기본값 재정의
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
            EXPECT_TRUE(p->f6() == 10); // 가상 함수 테이블을 참조하여 Base 의 기본값인 10을 사용합니다.   
        }
    }
    // 함수 오버로딩
    {
        using namespace Function_2;
        EXPECT_TRUE(f(10) == 1); // int f(int) 호출
        EXPECT_TRUE(f('a') == 2); // int f(char) 호출
        EXPECT_TRUE(f(10L) == 3); // int f(long) 호출
    }
    // 오버로딩된 함수 호출 규칙
    {
        using namespace Function_3;   

        EXPECT_TRUE(f(1) == 1); // (O) 타입 일치. int 버전 호출
        EXPECT_TRUE(f(1.) == 2); // (O) 타입 일치. double 버전 호출
        // EXPECT_TRUE(t.f(1L) == 1); // (X) 컴파일 오류. long 버전이 없음
        EXPECT_TRUE(f(1.F) == 2); // (△) 비권장. float 버전이 없지만, double로 암시적 형변환 되므로 double 버전 호출
    }
    {
        class T {
        public:
            int f() {return 1;}
            int f() const {return 2;} // (O) 함수 상수성에 따라 선택됨 
        };

        T val;
        const T constVal = val;

        EXPECT_TRUE(val.f() == 1);
        EXPECT_TRUE(constVal.f() == 2);
    }
    // 값 타입과 참조자간의 모호성

    {
        using namespace Function_7;

        int val = 10;
        int& ref = val;
        const int& constRef = val;

        EXPECT_TRUE(f(val) == 1); // int를 const int&에 대입하여 호출합니다.
        EXPECT_TRUE(f(ref) == 1); // int&를 const int&에 대입하여 호출합니다.
        EXPECT_TRUE(f(constRef) == 1);        
    }    
    {
        using namespace Function_8;

        int val = 10;
        int& ref = val;
        const int& constRef = val;

        EXPECT_TRUE(f(val) == 1); // int를 int&에 대입하여 호출합니다.
        EXPECT_TRUE(f(ref) == 1); 
        // EXPECT_TRUE(f(constRef) == 1); // (X) 컴파일 오류. const int&는 int&에 대입되지 않습니다.     
    }
    
    {
        using namespace Function_9;

        int val = 10;
        const int constVal = 10;

        int& ref = val;
        const int& constRef = val;

        EXPECT_TRUE(f(val) == 1); // int를 int&에 대입하여 호출합니다.
        EXPECT_TRUE(f(constVal) == 2); // const int를 const int&에 대입하여 호출합니다.

        EXPECT_TRUE(f(ref) == 1); 
        EXPECT_TRUE(f(constRef) == 2);        
    }
    {
        using namespace Function_10;

        int val = 10;
        const int constVal = 10;

        int& ref = val;
        const int& constRef = val;

        // EXPECT_TRUE(f(val) == 1); // (X) 컴파일 오류. f(int)와 f(int&)와 f(const int&)가 모호합니다.
        // EXPECT_TRUE(f(constVal) == 1); // (X) 컴파일 오류. f(int)와 f(const int&)가 모호합니다.

        // EXPECT_TRUE(f(ref) == 2); // (X) 컴파일 오류. f(int)와 f(int&)와 f(const int&)가 모호합니다.
        // EXPECT_TRUE(f(constRef) == 3); // (X) 컴파일 오류. f(int)와 f(const int&)가 모호합니다.
    }
    // 오버로딩 함수 탐색 규칙
    {
        using namespace Function_10_1;

        EXPECT_TRUE(f((bool)true) == 1); // bool은 int로 승격
        EXPECT_TRUE(g((char)'a') == 2); // char는 int로 승격
        EXPECT_TRUE(h((int)1) == 3); // int는 double로 승격
    }

    // 이름 탐색 규칙 - ADL(Argument-dependent lookup), Keonig 
    {
        EXPECT_TRUE(B::g() == 2); // B::MyFunc 이 채택됨
        EXPECT_TRUE(D::g() == 1); // C::MyFunc 이 채택됨
    }
}

