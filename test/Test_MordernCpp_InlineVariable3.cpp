#include "gtest/gtest.h" 
#include "Test_MordernCpp_InlineVariable.h"

namespace InlineVariable {
    extern void IncreaseVal1_17(); // 다른 파일에 있는 함수 선언
    extern void IncreaseVal2_17();
}

namespace {
    class T {
    public:
        static int s_m_Val; // 정적 멤버 변수. 별도 정의 필요
    };
    int T::s_m_Val{10}; // 별도 정의 필요

    // C++17 인라인 변수를 이용한 클래스 정적 멤버 변수 정의
    class T_17 {
    public:
        static inline int s_m_Val_17{10}; // 정적 멤버 변수. 인라인 변수여서 클래스 선언부에서 정의 및 초기화가 가능합니다. 
    };
}

TEST(TestMordern, InlineVariable) {
    // C++17 인라인 변수
    {
        using namespace InlineVariable;
        IncreaseVal1_17();
        IncreaseVal2_17();

        EXPECT_TRUE(g_Val_17 == 12);   
    }

    {
        EXPECT_TRUE(T::s_m_Val == 10);
    }
    // C++17 인라인 변수를 이용한 클래스 정적 멤버 변수 정의
    {
        EXPECT_TRUE(T_17::s_m_Val_17 == 10);
    }
}
