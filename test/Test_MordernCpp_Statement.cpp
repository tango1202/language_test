#include "gtest/gtest.h" 

namespace IfForInit_1 {
    int* Func() {return nullptr;}
}
namespace IfForInit_2 {
    std::shared_ptr<int> Func() {return std::shared_ptr<int>{};}
}
TEST(TestMordern, RangeFor) {
    {
        std::vector<int> v_11{1, 2, 3};

        int sum{0};

        // 컨테이너 요소를 탐색 합니다.
        auto itr_11{v_11.begin()};
        auto endItr_11{v_11.end()};
        for (;itr_11 != endItr_11; ++itr_11) {
            sum += *itr_11;
        }
        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v_11{1, 2, 3};

        int sum{0};

        for (int val_11 : v_11) { // 혹은 for (auto val_11 : v_11)
            sum += val_11;
        }

        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v_11{1, 2, 3};

        for (int& val_11 : v_11) { // 혹은 for (auto& val : v_11)
            ++val_11; // 벡터의 요소가 수정됩니다.
        }

        EXPECT_TRUE(v_11[0] == 2 && v_11[1] == 3 && v_11[2] == 4);
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
#if 201703L <= __cplusplus // C++17~    
    {
        using namespace IfForInit_2;
        // result는 if 문 이 끝나면 소멸됩니다.
        if (std::shared_ptr<int> result_17{Func()}; result_17 != nullptr) {
            // Todo
        }
    } 
#endif       
}




