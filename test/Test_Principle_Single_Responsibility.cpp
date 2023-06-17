#include "gtest/gtest.h" 

namespace {
    std::pair<int, int> CalcZoomDlgData(int percent) {
        int index = 5; // 기본적으로 사용자 정의 선택
        switch(percent) {
            case 50: index = 0; break;
            case 75: index = 1; break;
            case 100: index = 2; break;
            case 150: index = 3; break;
            case 200: index = 4; break;
        }
        return std::make_pair(index, percent);
    }    
}

TEST(TestPrinciple, SingleResponsibility) {
    {
        std::pair<int, int> result = CalcZoomDlgData(100);
        EXPECT_TRUE(result.first == 2);
    }
    {
        std::pair<int, int> result = CalcZoomDlgData(130);
        EXPECT_TRUE(result.first == 5 && result.second == 130);
    }
}