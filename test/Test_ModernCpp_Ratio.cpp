#include "gtest/gtest.h" 

TEST(TestModern, Ratio) {
    {
        using Result = std::ratio_add< // 1/ 100 + 99 / 100 을 오차없이 계산 합니다.
            std::ratio<1, 100>, 
            std::ratio<99, 100>
        >;
        enum class MyEnum {Val1 = Result::num, Val2 = Result::den};
   
        // 분자와 분모가 모두 1입니다.
        EXPECT_TRUE(static_cast<int>(MyEnum::Val1) == 1 && static_cast<int>(MyEnum::Val2) == 1);
    }
    {
        using Result = std::ratio_equal<
            std::ratio<1, 3>, 
            std::ratio<2, 6>
        >;
        EXPECT_TRUE(Result::value == true);
    }
}