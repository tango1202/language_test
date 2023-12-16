#include "gtest/gtest.h" 

namespace {

    // ----
    // Zero 초기화
    // ----   
    int g_Val; // 전역 변수. 명시적 초기화 가능
    static int s_Val; // 정적 전역 변수. 명시적 초기화 가능

    class T {
    public:
        static int s_m_Val; // 정적 멤버 변수. 명시적 초기화 불가능. 선언과 정의 분리. 단 const 형은 선언에서 초기화 가능
        static const int s_c_m_Val; // 정적 상수 멤버 변수. 명시적 초기화 가능
        int m_Val; // 멤버 변수. 명시적 초기화 불가능

        int f1() const {
            static int s_l_Val; // 정적 지역 변수. 명시적 초기화 가능
            return s_l_Val;
        }

        int f2() const {
            int l_Val; // 지역 변수. 명시적 초기화 가능
            return l_Val;
        }
    };
    int T::s_m_Val; // 선언과 정의 분리
    const int T::s_c_m_Val = 0; // 선언과 정의 분리 
}

TEST(TestLegacyCpp, ZeroInitialization) {
 
    // ----
    // Zero 초기화
    // ----
    {
        T t;
        T* ptr1 = new T; // 괄호 없이 생성합니다. 자동 초기화가 안됩니다.
        T* ptr2 = new T(); // 괄호로 생성합니다. 자동 초기화가 됩니다.

        EXPECT_TRUE(g_Val == 0); // 전역 변수는 0으로 자동 초기화
        EXPECT_TRUE(s_Val == 0); // 정적 전역 변수는 0으로 자동 초기화
        EXPECT_TRUE(T::s_m_Val == 0); // 정적 멤버 변수는 0으로 자동 초기화
        EXPECT_TRUE(T::s_c_m_Val == 0); // 정적 상수 멤버 변수는 명시적 초기화
        EXPECT_TRUE(ptr1->m_Val == 0 || ptr1->m_Val != 0); // new T; 로 생성하면 자동 초기화 되지 않습니다.
        EXPECT_TRUE(ptr2->m_Val == 0); // new T(); 로 생성하면 자동 초기화됩니다.
        EXPECT_TRUE(t.f1() == 0); // 정적 지역 변수는 0으로 자동 초기화
        // EXPECT_TRUE(t.f2() != 0); // 지역 변수는 쓰레기값이 될 수도 있음
       
        int arr[3] = {1, }; 
        EXPECT_TRUE(arr[0] == 1 && arr[1] == 0 && arr[2] == 0); // 배열 갯수 보다 초기화 갯수가 적을때 나머지 요소는 0으로 자동 초기화
    
        delete ptr1;
        delete ptr2;
    }

}
