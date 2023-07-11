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
    // ----
    // 초기화 순서
    // ----
    {
        class T {
            int m_A;
            int m_B;
            int m_C;
        public:
            T(int a, int b, int c) :
                m_C(c + m_B), // (△) 비권장. 3
                m_B(b + m_A), // (△) 비권장. 2
                m_A(a) {} // (△) 비권장. 1
            int GetA() const {return m_A;}
            int GetB() const {return m_B;}
            int GetC() const {return m_C;}
        };
        T t(10, 20, 30);
        EXPECT_TRUE(t.GetA() == 10 && t.GetB() == 30 && t.GetC() == 60);        
    }
    {
        class T {
        public:
            int a;
            int b;
            int c;
            T(int a, int b, int c) : // 멤버 변수명과 인자명이 같더라도 초기화 리스트에서 사용 가능합니다.
                a(a), 
                b(b),
                c(c) {
                // 함수 본문에서 멤버 변수명과 인자명이 같으면, 멤버 변수는 this를 써서 접근합니다.
                this->a += 1; // 멤버 변수 a를 수정함
                a += 2; // 인자 a를 수정함       
            }
        };
        T t(10, 20, 30); 
        EXPECT_TRUE(t.a == 11 && t.b == 20 && t.c == 30);
    }

}
