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

TEST(TestLegacyCpp, MemberVariable) {
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
            U(int x, int y) {  // (△) 비권장. 초기화 리스트가 없어서 m_X, m_Y를 기본 생성자로 생성합니다.  
                m_X = T(x); // (△) 비권장. 값 생성자로 임시 개체를 생성 후, 복사 대입 연산자로 대입합니다.
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
            int m_A; // m_A, m_B, m_C의 순서로 초기화 됩니다.
            int m_B;
            int m_C;
        public:
            // 초기화 리스트 순서가 아닌 멤버 변수 선언 순서로 초기화 됩니다.
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
    // ----
    // 메모리 정렬
    // ----
    {
        class T1 {
            int m_X;
            int m_Y;
        };
        EXPECT_TRUE(sizeof(T1) == sizeof(int) * 2); // 8

        class T2 { // 멤버 변수중 가장 큰 int 에 맞춰 정렬
            char m_X; // 1byte. 3 byte 패딩
            int m_Y;
        };
        EXPECT_TRUE(sizeof(T2) == sizeof(int) * 2); // 8
        
        class T3 { // 멤버 변수중 가장 큰 double에 맞춰 정렬
            char m_X; // 1byte. 7byte 패딩
            double m_Y;
        };
        EXPECT_TRUE(sizeof(T3) == sizeof(double) * 2); // 16

        struct T4 { // 멤버 변수중 가장 큰 double에 맞춰 정렬.
            char m_X; // 1byte. 3byte 패딩
            int m_Y; // 4byte. 
            double m_Z;
        };
        EXPECT_TRUE(sizeof(T3) == sizeof(double) * 2); // 16
    }
    {
        class Empty {}; // 빈 클래스는 강제로 1byte
        EXPECT_TRUE(sizeof(Empty) == 1);

        class Composite {
            int m_X;
            Empty m_Empty; // 1byte 이지만 3byte 패딩됨
        };
        EXPECT_TRUE(sizeof(Composite) == sizeof(int) + sizeof(int));

        class EmptyDerived : public Empty { // 빈 클래스를 상속받으면, 강제 1byte는 빼고 크기가 설정됨
            int m_X;
        };
        EXPECT_TRUE(sizeof(EmptyDerived) == sizeof(int));

        class Base { // 멤버 변수는 없지만 virtual 이 있어 가상 함수 테이블이 생성됨
        public:
            virtual ~Base() {}
        };
        EXPECT_TRUE(sizeof(Base) == 8);

        class Derived : public Base { // 가상 함수 테이블 크기로 정렬됨
            char m_X;
        };
        EXPECT_TRUE(sizeof(Derived) == 8 + 8); 
        std::cout << sizeof(Derived) << std::endl;        
    }
    // 메모리 접근 편의를 위해 4byte단위로 멤버 변수를 할당. 4byte 단위로 읽을 수 있도록 빈공간 할당(패딩)
    {   
        class T {
            char m_Char1; // 1byte, 3byte 패딩 
            int m_Int1; // 4byte
            char m_Char2; // 1byte, 3byte 패딩
            int m_Int2; // 4byte
        };
        EXPECT_TRUE(sizeof(T) == 16);
    }
    {
        class T {
            char m_Char1; // 1byte
            char m_Char2; // 1byte, 2byte 패딩
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

        T t2(&val); 

        t2 = t1; // (△) 비권장. 이미 지워버린 ptr을 가진 t1을 t2에 복사했습니다. 이런 실수 많이 하게 됩니다.
    }
}
