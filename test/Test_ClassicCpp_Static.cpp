#include "gtest/gtest.h" 

namespace {
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

}