#include "gtest/gtest.h" 

namespace IfForInit_1 {
    int* Func() {return nullptr;}
}
namespace IfForInit_2 {
    std::shared_ptr<int> Func() {return std::shared_ptr<int>{};}
}
TEST(TestMordern, RangeFor) {
    {
        std::vector<int> v{1, 2, 3};

        int sum{0};

        // 컨테이너 요소를 탐색 합니다.
        auto itr{v.begin()};
        auto endItr{v.end()};
        for (;itr != endItr; ++itr) {
            sum += *itr;
        }
        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v{1, 2, 3};

        int sum{0};

        // for (auto val : v) { // val == int
        // for (const int& val : v) {  // val == const int&
        for (int val : v) { // 혹은 for (auto val : v)
            sum += val;
        }

        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v{1, 2, 3};

        for (int& val : v) { // 혹은 for (auto& val : v)
            ++val; // 벡터의 요소가 수정됩니다.
        }

        EXPECT_TRUE(v[0] == 2 && v[1] == 3 && v[2] == 4);
    }
}

TEST(TestMordern, IfForInit) {
    // (C++17~) 초기식을 포함하는 if(), switch()
    {
        using namespace IfForInit_1;
        int* result{Func()}; // 결과값을 구한 후
        if (result != nullptr) { // 조건 평가
            // Todo
        }       
    }
    {
        using namespace IfForInit_2;
        std::shared_ptr<int> result{Func()};
        if (result != nullptr) {
            // Todo
        }    
    }
    {
        using namespace IfForInit_2;
        // result는 if 문 이 끝나면 소멸됩니다.
        if (std::shared_ptr<int> result{Func()}; result != nullptr) {
            // Todo
        } 
    }    
}




