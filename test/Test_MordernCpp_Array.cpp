#include "gtest/gtest.h" 

TEST(TestMordern, Array) {
    {
        std::array<int, 3> a{1, 2, 3};
        std::array<int, 3> b{4, 5}; 
        std::array<int, 2> c{4, 5};   
        
        // int* ptr = a; // (X) 컴파일 오류. array는 포인터로 암시적 변환이 되지 않습니다.
        int* ptr = a.data(); // data를 이용하면 포인터로 변환됩니다.  
        
        EXPECT_TRUE(b[0] == 4 && b[1] == 5 && b[2] == 0); // 요소의 갯수보다 적게 초기화하면 나머지 요소는 0으로 초기화 됩니다.

        // c = a; // (X) 컴파일 오류. 요소의 갯수가 다르면 컴파일 오류가 발생합니다.
        b = a; // 요소의 갯수가 같으면 대입이 가능합니다.
        EXPECT_TRUE(b[0] == 1 && b[1] == 2 && b[2] == 3);
    }

}