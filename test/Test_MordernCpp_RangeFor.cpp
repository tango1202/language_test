#include "gtest/gtest.h" 

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
        for (int val : v) {
            sum += val;
        }

        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    {
        std::vector<int> v{1, 2, 3};

        for (int& val : v) {
            ++val; // 벡터의 요소가 수정됩니다.
        }

        EXPECT_TRUE(v[0] == 2 && v[1] == 3 && v[2] == 4);
    }
}





