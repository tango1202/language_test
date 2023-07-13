#include "gtest/gtest.h" 

TEST(TestClassicCpp, MemberVariable) {
    // 메모리 접근 편의를 위해 4byte단위로 멤버 변수를 할당. 4byte 단위로 읽을 수 있도록 빈공간 할당(패딩)
    {   
        class T {
            char m_Char1; // 1byte, 메모리 접근 편의를 위해 32bit(4byte) 단위로 할당(패딩). 3byte 빈공간이 생김 
            int m_Int1; // 4byte
            char m_Char2; // 1byte, 메모리 접근 편의를 위해 32bit(4byte) 단위로 할당(패딩). 3byte 빈공간이 생김 
            int m_Int2; // 4byte
        };
        EXPECT_TRUE(sizeof(T) == 16);
    }
    {
         class T {
            char m_Char1; // 1byte
            char m_Char2; // 1byte, 패딩을 위해 2byte 빈공간이 생김
            int m_Int1; // 4byte
            int m_Int2; // 4byte
        };
        EXPECT_TRUE(sizeof(T) == 12);       
    }
}
