#include "gtest/gtest.h" 

TEST(TestMordern, Align) {
    // 4byte 단위로 정렬합니다.
    class alignas(alignof(int)) A {
        char m_A;
        int m_B;
    };

    // 2, 4, 8, 16... 단위로 정렬하며, 내부 멤버중 제일 큰값보다 커야 합니다.
    class alignas(8) B {
        char m_A[13];
        int m_B; 
    };

    // int 보다 적은값이므로 무시되고 alignof(int) 크기로 정렬됩니다.
    class alignas(2) C {
        char m_A[13];
        int m_B; 
    };

    // 4byte 단위로 멤버 변수가 할당 되므로 char(1) + int(4) = 5 이므로 4 * 2 개 영역에 할당됨
    EXPECT_TRUE(alignof(A) == 4 && sizeof(A) == 4 * 2);

    // 8byte 단위로 멤버 변수가 할당 되므로 13 + int(4) = 17 이므로 8 * 3 개 영역에 할당됨
    EXPECT_TRUE(alignof(B) == 8 && sizeof(B) == 8 * 3); 

    // 4byte 단위로 멤버 변수가 할당 되므로 13 + int(4) = 17 이므로 4 * 5 개 영역에 할당됨
    EXPECT_TRUE(alignof(C) == 4 && sizeof(C) == 4 * 5); 
}