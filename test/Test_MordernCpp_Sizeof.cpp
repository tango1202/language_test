#include "gtest/gtest.h" 

TEST(TestMordern, Sizeof) {

    class T {
    public:
        int m_X;
    };

    // C++03에서는 컴파일 오류
    // C++11부터 허용
    EXPECT_TRUE(sizeof(T::m_X) == sizeof(int));
}

