#include "gtest/gtest.h" 

namespace {
    // ----
    // 지역 변수
    // ----
    void f() {
        int a; // 함수 블록을 벗어나면 자동으로 소멸됩니다.
    }
   
    // ----
    // 정적 전역 변수
    // ----
    static int s_Val = 10; // 현재 파일에서 사용 가능  

    // ----
    // 정적 멤버 변수
    // ----
    // 선언에서
    class T {
    public:
        static int s_Val1; // 선언과 정의 분리 필요. 외부에서 초기값이 세팅되어야 함
        // static int s_Val1 = 10; // (X) 컴파일 오류. const만 클래스 선언에서 초기화 지원
        static const int s_Val2 = 20; // const 인 경우 클래스 선언에서 초기화 지원
    };
    // 선언 외부에서
    int T::s_Val1 = 10; // 초기값 세팅
}
TEST(TestClassicCpp, Static) {
    // ----
    // 정적 전역 변수
    // ----
    {
        EXPECT_TRUE(s_Val == 10);   
    }
    // ----
    // 멤버 변수
    // ----
    {
        class Date {
            int m_Year;
            int m_Month;
            int m_Day;
        public:
            Date(int year, int month, int day) :
                m_Year(year), // 생성자 본문 실행전 초기화 리스트에서 생성됩니다.
                m_Month(month), 
                m_Day(day) {}
            ~Date() {} // 소멸자 본문 실행 후 소멸됩니다.
        };       
    }
    // ----
    // 정적 멤버 변수
    // ----
    {
        EXPECT_TRUE(T::s_Val1 == 10);
        EXPECT_TRUE(T::s_Val2 == 20);
    }
    // ----
    // 함수내 정적 지역 변수
    // ----
    {
        class T {
        public:
            static int GetVal() {
                static int s_Val = 30; // 최초 1회 초기화됩니다.
                ++s_Val; // 호출시마다 증가합니다.
                return s_Val;
            }
        };

        EXPECT_TRUE(T::GetVal() == 31); // 1회 호출
        EXPECT_TRUE(T::GetVal() == 32); // 2회 호출
        EXPECT_TRUE(T::GetVal() == 33); // 3회 호출
    }
    {
        class A {
        private:
            A() {} // private여서 외부에서 생성할 수 없습니다.
        public:
            static A& GetInstance() {
                static A s_A;
                return s_A;
            }    
        };
        class B {
        private:
            explicit B(A) {} // private여서 외부에서 생성할 수 없습니다. A로부터 생성됩니다.
        public:
            static B& GetInstance() {
                static B s_B(A::GetInstance()); // A::GetInstance()로 A개체는 반드시 생성되고 초기화 됨을 보장합니다.
                return s_B;
            } 
        };
        class C {
        private:
            explicit C(B) {} // private여서 외부에서 생성할 수 없습니다. B로부터 생성됩니다.
        public:
            static C& GetInstance() {
                static C s_C(B::GetInstance()); // B::GetInstance()로 B개체는 반드시 생성되고 초기화 됨을 보장합니다.
                return s_C;
            } 

        };
        A& g_A = A::GetInstance();
        B& g_B = B::GetInstance();
        C& g_C = C::GetInstance();         
    }
    // ----
    // 임시 개체
    // ----
    {
        class T {};

        T(); // 임시 개체 T()가 생성되고 아무곳에서도 사용하지 않았으므로 소멸됩니다.
        T a = T(); // 임시 개체 T()가 생성되고 a에 복사 생성합니다. 다만 컴파일러 최적화에 의해 생성자를 1회 호출하기는 합니다.
        // T& b = T(); // (X) 컴파일 오류. 임시 개체는 소멸되었으므로 참조할 수 없습니다.
        const T& c = T(); // const로 사용하면 수명이 연장됩니다.
    }   
    {
        class T {
        public:
            static int f(int x) {
                return x * x; // 리턴시 임시 개체 생성
            }
        };

        int result = T::f(1 + 1); // 인수를 인자에 전달시 임시 개체 생성
    }
    {
        class T {
        public:
            static int f(int x) {
                int temp = x * x; // 임시 개체 생성하여 리턴합니다.
                return temp; 
            }
        };

        int temp = 1 + 1; // 임시 개체를 생성하여 함수에 전달합니다.
        int result = T::f(temp);         
    }
}
