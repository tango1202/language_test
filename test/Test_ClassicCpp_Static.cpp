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
    // 정적 멤버 함수
    // ----
    {
        class T {
        public:
            static int f() {return 10;} // 정적 함수
        };

        EXPECT_TRUE(T::f() == 10); // (O) T의 정적 함수 호출
        T obj;
        EXPECT_TRUE(obj.f() == 10); // (△) 비권장. T의 정적 함수 호출. 되기는 합니다만 일반 멤버 함수 호출과 구분이 안되어 가독성이 떨어집니다.
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
    // ----
    // 임시 개체
    // ----
    {
        class Object {
        public:
            Object() {
                int& count = GetCountRef();
                ++count; // 생성할때마다 값을 증가시킵니다.
            };
            // 정적 변수의 참조자를 리턴합니다.
            static int& GetCountRef() {
                static int s_Count = 0;
                return s_Count;
            }  
            // 정적 변수의 값을 리턴합니다.
            static int GetCount() {
                return GetCountRef();
            }

            // 개체 형식(값 타입)을 리턴합니다. 연산자 오버로딩 참고.
            Object operator +(const Object& other) const {
                return Object();
            }
        };

        {
            class T {
            public:
                static void f(const Object& obj) {
                    obj + obj; // 리턴시 임시 개체 생성
                }
            };
            Object arg;
            int& count = Object::GetCountRef();
            count = 0;

            T::f(arg + arg); // 인수를 인자에 전달시 임시 개체 생성

            EXPECT_TRUE(Object::GetCount() == 2);            
        }
        {
            class T {
            public:
                static Object f(const Object& obj) {
                    obj + obj; // 리턴시 임시 개체 생성

                    return Object(); // 개체를 생성하여 리턴
                }
            };
            Object arg;
            int& count = Object::GetCountRef();
            count = 0;

            T::f(arg + arg); // 인수를 인자에 전달시 임시 개체 생성

            EXPECT_TRUE(Object::GetCount() == 3);               
        }
        {
            class T {
            public:
                static Object f(const Object& obj) {
                    Object result(obj + obj); // 연산시 임시 개체 생성

                    return result; // 복사 생성자로 생성한 것을 리턴
                }
            };
            Object arg;
            int& count = Object::GetCountRef();
            count = 0;

            T::f(arg + arg); // 인수를 인자에 전달시 임시 개체 생성

            EXPECT_TRUE(Object::GetCount() == 2);               
        }
         {
            class T {
            public:
                static Object f(const Object& obj) {
                    Object result(obj + obj); // 연산시 임시 개체 생성

                    return result; // 복사 생성자로 생성한 것을 리턴
                }
            };
            Object arg;
            int& count = Object::GetCountRef();
            count = 0;

            Object result = T::f(arg + arg); // 인수를 인자에 전달시 임시 개체 생성, 리턴받은 개체를 생성(?)

            EXPECT_TRUE(Object::GetCount() == 2);               
        }    
    }

}