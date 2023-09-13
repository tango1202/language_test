#include "gtest/gtest.h" 

    template <typename T>
class A {};
TEST(TestMordern, Sizeof) {

    class T {
    public:
        int m_X;
    };

    // C++03에서는 컴파일 오류
    // C++11부터 허용
    EXPECT_TRUE(sizeof(T::m_X) == sizeof(int));



std::vector<A<int> > a; // C++03. 공백을 추가해야 했습니다.
std::vector<A<int>> b; // C++11
}

