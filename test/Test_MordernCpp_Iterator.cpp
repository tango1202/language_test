#include "gtest/gtest.h" 

TEST(TestMordern, Iterator) {
    // move_iterator
    {
        std::vector<std::string> v{"a", "b", "c"};

        std::vector<std::string> v1{v.begin(), v.end()};
        EXPECT_TRUE(v[0] == "a" && v[1] == "b" && v[2] == "c"); // 요소가 복사됩니다.

        std::vector<std::string> v2{std::make_move_iterator(v.begin()), std::make_move_iterator(v.end())};
        EXPECT_TRUE(v[0].empty() && v[1].empty() && v[2].empty()); // 요소가 이동되어 비었습니다.
    }
    // next, prev
    {
        std::vector<int> v{1, 2, 3, 4};
        auto itr{v.begin()};
        std::advance(itr, 2); // 인자로 전달한 이터레이터를 뒤로 2번 이동시킴
        EXPECT_TRUE(*itr == 3);

        itr = v.begin();
        EXPECT_TRUE(*std::next(itr, 2) == 3 && *itr == 1); // 뒤로 2번 이동. itr은 수정되지 않음
        EXPECT_TRUE(*std::prev(itr, -2) == 3 && *itr == 1); // 앞으로 -2번 이동. itr은 수정되지 않음
    }
}