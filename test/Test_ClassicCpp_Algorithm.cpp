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

TEST(TestClassicCpp, Find) {
    
    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);

    std::vector<int>::iterator result = std::find(v.begin(), v.end(), 1); // 값이 1인 위치를 리턴합니다.
    EXPECT_TRUE(*result == 1 && result == (v.begin() + 1));
}
TEST(TestClassicCpp, Search) {
    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::vector<int> sub; // 검색하려는 부분 시퀀스
    sub.push_back(1);
    sub.push_back(2);

    std::vector<int>::iterator result = std::search(v.begin(), v.end(), sub.begin(), sub.end()); // 부분 시퀀스가 1, 2인 위치를 찾습니다.
    EXPECT_TRUE(*result == 1 && result == (v.begin() + 2));
}
TEST(TestClassicCpp, Count) {
    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(1); 
    v.push_back(2);
    v.push_back(3);

    size_t result = std::count(v.begin(), v.end(), 1); // 1인 요소의 갯수를 구합니다.
    EXPECT_TRUE(result == 2); // 1은 2개 입니다.
}



