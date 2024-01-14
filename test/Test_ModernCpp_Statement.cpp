#include "gtest/gtest.h" 

namespace IfForInit_1 {
    int* Func() {return nullptr;}
}
namespace IfForInit_2 {
    std::shared_ptr<int> Func() {return std::shared_ptr<int>{};}
}
TEST(TestModern, RangeFor) {
    {
        std::vector<int> v_11{1, 2, 3};

        int sum{0};

        // 컨테이너 요소를 탐색 합니다.
        std::vector<int>::iterator itr{v_11.begin()};
        std::vector<int>::iterator endItr{v_11.end()};
        for (;itr != endItr; ++itr) {
            sum += *itr;
        }
        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v_11{1, 2, 3};

        int sum{0};

        for (int val : v_11) { // 혹은 for (auto val : v_11)
            sum += val;
        }

        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v_11{1, 2, 3};

        for (int& val : v_11) { // 혹은 for (auto& val : v_11)
            ++val; // 벡터의 요소가 수정됩니다.
        }

        EXPECT_TRUE(v_11[0] == 2 && v_11[1] == 3 && v_11[2] == 4);
    }
#if 202002L <= __cplusplus // C++20~
    {
        int sum{0};

        // 초기식을 이용하여 v_20 값을 초기화 합니다.
        for (std::vector<int> v_20{1, 2, 3}; int val : v_20) { 
            sum += val;
        }

        EXPECT_TRUE(sum == 1 + 2 + 3);        
    }
#endif    
}

TEST(TestModern, IfForInit) {
    // (C++17~) 초기식을 포함하는 if(), switch()
    {
        using namespace IfForInit_1;
        int* result{Func()}; // 결과값을 구한 후
        if (result != nullptr) { // 조건 평가
            // Todo
        }       
    }
    { // 유효 범위를 짧게 하기 위해 중괄호를 사용했습니다. 
        using namespace IfForInit_2;
        // result는 중괄호가 끝나면 소멸됩니다.
        std::shared_ptr<int> result{Func()};
        if (result != nullptr) {
            // Todo
        }    
    } // 여기서 result가 소멸됩니다.
#if 201703L <= __cplusplus // C++17~    
    {
        using namespace IfForInit_2;
        // result_17은 if 문이 끝나면 소멸됩니다.
        if (std::shared_ptr<int> result_17{Func()}; result_17 != nullptr) {
            // Todo
        }
    } 
#endif  
}




