#include "gtest/gtest.h" 

namespace {
    static int s_Val = 10; // 현재 파일에서 사용 가능  

    // 선언에서
    class T {
    public:
        static int m_Val1; //  선언 외부에서 초기값이 세팅되어야 함
        // static int m_Val1 = 10; // (X) 컴파일 오류. 
        static const int m_Val2 = 20; // const 인 경우 선언에서 초기화 지원
    };

    // 선언 외부에서
    int T::m_Val1 = 10; // 초기값 세팅
}
TEST(TestClassicCpp, Static) {
    {
        EXPECT_TRUE(s_Val == 10);   
    }
    {
        EXPECT_TRUE(T::m_Val1 == 10);
        EXPECT_TRUE(T::m_Val2 == 20);
    }
    {
        class T {
        public:
            static int f() { return 10; } // 정적 함수

            static int GetVal() {
                static int val = 30; // 최초 1회 초기화됩니다.
                ++val; // 호출시마다 증가합니다.
                return val;
            }
        };

        EXPECT_TRUE(T::f() == 10); // (O) T의 정적 함수 호출
        T a;
        EXPECT_TRUE(a.f() == 10); // (X) T의 정적 함수 호출. 되기는 합니다만 일반 멤버 함수 호출과 구분이 안되어 가독성이 떨어집니다.

        EXPECT_TRUE(T::GetVal() == 31); // 1회 호출
        EXPECT_TRUE(T::GetVal() == 32); // 2회 호출
        EXPECT_TRUE(T::GetVal() == 33); // 3회 호출
    }

}