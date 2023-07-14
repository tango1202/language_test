#include "gtest/gtest.h" 

namespace {
    // ----
    // 개요
    // ----
    class T {
        int m_Val1; // 멤버 변수
        int& m_Val2; // 참조자 멤버 변수. 참조자는 초기화 되어야 함
        int* m_Val3; // 포인터 멤버 변수
        const int m_Val4; // 상수 멤버 변수. 초기값이 세팅되어야 함
        // const int m_Val4 = 0; // (O) 선언부 초기화 지원
    public:    
        static int s_m_Val5; // 정적 멤버 변수. 별도 정의 필요
        static const int s_c_m_Val6; // 정적 상수 멤버 변수
        // static const int s_c_m_Val6 = 0; // (O) 선언부 초기화 지원
    public: 
        T(int& val2, int val4) :
            m_Val2(val2), // 참조자는 반드시 초기화 리스트에서 세팅되어야 함
            m_Val4(val4) {} // 상수 멤버 변수는 초기값이 세팅되어야 함
    };

    int T::s_m_Val5; // 별도 정의 필요
    const int T::s_c_m_Val6 = 0; // 선언만 했다면 정의 필요. 선언부 초기화를 했다면 별도 정의 불필요
}

TEST(TestClassicCpp, MemberVariable) {
    // ----
    // 개요
    // ----
    {
        int val2;
        T t(val2, 10);

        T::s_m_Val5 = 10;
        EXPECT_TRUE(T::s_c_m_Val6 == 0);
    }
    // ----
    // 초기화 리스트
    // ----
    {
        class T {
        public:
            T() {} // 기본 생성자
            explicit T(int val) {} // 값 생성자
        };

        class U {
            T m_X;
            T m_Y;
        public:
            U(int x, int y) {  // (△) 비권장. 초기화 리스트가 없어서 멤버 변수를 기본 생성자로 생성합니다.  
                m_X = T(x); // (△) 비권장. 값 생성자로 임시 개체를 생성 후, 대입 연산자로 대입합니다.
                m_Y = T(y);
            }
        };        
    }
    {
        class T {
        public:
            T() {} // 기본 생성자
            explicit T(int val) {} // 값 생성자
        };

        class U {
            T m_X;
            T m_Y;
        public:
            U(int x, int y) : 
                m_X(x), 
                m_Y(y) {} // (O) 값 생성자로 멤버 변수를 초기화 합니다. 
        };        
    }
    // 필요한 인자를 모두 나열하고 초기화
    {
        class T {
            int m_X;
            int m_Y;
        public:
            explicit T(int x) : 
                m_X(x) {} // (△) 비권장. 멤버 변수 중 m_Y는 초기화하지 않았습니다.
            void SetX(int x) {m_X = x;}
            void SetY(int y) {m_Y = y;}
        };

        // (△) 비권장. 멤버 변수 중 m_Y는 초기화하지 않았습니다. 
        // 초기화를 위해 추가로 필요한 요소가 뭔지 T 클래스를 파악해 봐야 합니다.
        T t(10); 

        // (△) 비권장. m_Y를 함수를 별도로 호출해야 합니다.
        t.SetY(20);        
    }
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T(int x, int y) : 
                m_X(x), // (O) 생성자에서 모든 멤버 변수를 초기화 합니다.
                m_Y(y) {} 
        };

        // (O) 생성자에서 모든 멤버 변수를 초기화 합니다.
        // 필요한 요소가 뭔지 생성자만 봐도 알 수 있습니다. 
        T t(10, 20); 
    }
    // 멤버 변수 정의 순서와 초기화 리스트 순서
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
    // 멤버 변수명과 인자명이 같은 경우
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
    // 포인터 멤버 변수
    // ----    
    {
        class T {
            int* m_Ptr;
        public:
            T(int* ptr) : 
                m_Ptr(ptr) {}
        };
        int* ptr = new int;
        int val = 10;

        T t1(ptr); // (△) 비권장. ptr는 new 한 것이기 때문에 m_Ptr은 delete 되어야 합니다.
        delete ptr; // (△) 비권장. 그냥 밖에서 지워버렸습니다.

        T t2(&val); // (△) 비권장. val은 스택에 생성된 자동 변수 이므로 delete하면 안됩니다.

        t2 = t1; // (△) 비권장. 이미 지워버린 ptr을 가진 t1을 t2에 복사했습니다. 이런 실수 많이 하게 됩니다.
    }
    // ----
    // pImpl
    // ----  
    {
        class T {
        private:    
            class Impl { // 실제 멤버 변수들의 집합
            public:
                int m_X;
                int m_Y;
            };
            Impl* m_Impl; // 멤버 변수는 1개임
        };
    }
}
