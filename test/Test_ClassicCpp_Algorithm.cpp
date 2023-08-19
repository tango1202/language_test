#include "gtest/gtest.h" 

#include <algorithm>

namespace {
    template<typename InputIterator, typename Function>
    Function my_for_each(InputIterator first, InputIterator last, Function f) {
        // first~last 직전까지 요소에 대해 f() 호출
        for (; first != last; ++first) { 
            f(*first);
        }
        return f; 
    }
    // 10으로 세팅하는 함수
    void Setter(int& val) {
        val = 10;
    }
}

TEST(TestClassicCpp, Algorithm) {
    {
        // 0으로 초기화 되어 생성
        std::vector<int> v(3);

        EXPECT_TRUE(v[0] == 0 && v[1] == 0 && v[2] == 0);

        // 모든 요소에 대해 Setter 적용
        std::for_each(v.begin(), v.end(), Setter);
        EXPECT_TRUE(v[0] == 10 && v[1] == 10 && v[2] == 10); 
    }
}
