#include "gtest/gtest.h" 
#include <algorithm>

namespace {
    bool IsSame(int a, int b, int c) {
        if (a != b) return false;
        if (b != c) return false;

        return true;
    }
}

TEST(TestModern, Functor) {
#if 201703L <= __cplusplus // C++17~    
    // C++17 Searcher
    {
        std::string data{"Hello world."};
        std::string sub{"wo"};

        auto itr = std::search(
            data.begin(),
            data.end(),
            std::default_searcher(
                sub.begin(),
                sub.end()
            )
        );

        // data에서 sub를 찾았다면 오프셋을 구해봅니다.
        if (itr != data.end()) {
            EXPECT_TRUE(std::distance(data.begin(), itr) == 6);
        }
    }
#endif

#if 201703L <= __cplusplus // C++17~
    // C++17 not_fn
    {
        // IsSame을 부정하는 함수자를 만듭니다.
        auto IsDifferent{std::not_fn(IsSame)};
        EXPECT_TRUE(IsDifferent(1, 1, 1) == false); // 인자가 여러개여도 됩니다. 
    }
#endif    
}