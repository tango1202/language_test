#include "gtest/gtest.h" 

namespace {
    // ----
    // 전방 선언
    // ----
    // 1. YourClass 전방 선언
    class YourClass; 

    // 2. MyClass 선언
    class MyClass {
        // (O) 전방 선언을 통해 YourClass가 대충 클래스라는 걸 압니다. 
        // 반드시 포인터나 참조자와 같은 참조 형식이어야 합니다.
        YourClass* m_Your; 

        // YourClass의 구체 정의가 필요하여 선언만 합니다.
        void f(); 
    };

    // 3. YourClass 선언
    class YourClass {
        // MyClass는 상위에 정의되어 사용할 수 있습니다.
        MyClass m_My; 

    public:
        void g() {}
    }; 

    // 4. MyClass 정의 - YourClass를 사용하고 있어 YourClass 선언 후 작성합니다.
    void MyClass::f() {
        m_Your->g(); 
    }

    // ----
    // 멤버 사양
    // ----
    class T {
        // 멤버 변수
        int m_D1; 

        // 정적 멤버 변수. 단 중첩 클래스, 함수 내부 로컬 클래스에서는 사용 못함
        static const int s_D2 = 1; 

        // ----
        // 멤버 함수
        // ----
        T() {} // 기본 생성자
        T(const T& other) {} // 복사 생성자
        ~T() {} // 소멸자
        T& operator =(const T& other) {return *this;} // 대입 연산자
        
        operator int() const {return 0;} // 형변환 연산자

        void f1(int) {} // 멤버 함수
        void f2(int) {} // 상수 멤버 함수

        virtual void f3(int) {} // 가상 함수    
        virtual void f4(int) = 0; // 순가상 함수

        static void f5(int) {} // 정적 멤버 함수
        // ----

        // 열거형 상수
        enum {Left, Top, Right, Bottom}; 
    
        // 중첩 클래스(구조체)
        class NestedClass { 
            int m_D3;
        };
        
        // 타입 재정의
        typedef NestedClass Inner; 
    }; 
}

TEST(TestClassicCpp, StructClassUnion) {
    // ----
    // 개요
    // ----
    {
        struct S {
            int x; // 기본적으로 public
            int y;
        };

        S s = {10, 20}; // 구조체는 중괄호 초기화 지원

        class C {
            int m_X; // 기본적으로 private
            int m_Y;
        public:
            C(int x, int y) {} // 값 생성자 정의
        };
        C c(10, 20); // 클래스는 값 생성자만 가능. 중괄호 초기화 미지원      
    }
    // ----
    // using 선언
    // ----   
    {
        class Base {
        protected:
            int m_Val;
        };
        
        class Derived : public Base {
        public:
            using Base::m_Val; // (△) 비권장. Base m_Val을 public으로 변경합니다.
        };
        Derived d;
        d.m_Val = 10;
        EXPECT_TRUE(d.m_Val == 10); // 이제 public이라 접근 가능합니다.
    }
    // ----
    // 함수 내부의 로컬 클래스
    // ----   
    {
        class T {
        public:
            int m_X;
        };
        T t; // 함수내에서만 사용 가능
        t.m_X = 10; 
        EXPECT_TRUE(t.m_X == 10);
    }
    // ----
    // 공용체
    // ----  
    {
        class C {
        private:
            int m_Val1;
            int m_Val2;
        public:
            // C(int v1, int v2) {} // (X) 컴파일 오류. 공용체 멤버는 생성자를 가질 수 없음
            // ~C() {} // (X) 컴파일 오류. 공용체 멤버는 소멸자를 가질 수 없음
            int GetVal1() const {return m_Val1;} // 멤버 함수가 있어도 되나, virtual 이면 안됨
            void SetVal1(int val) {m_Val1 = val;}
        };

        struct S1 {
            int x;
            int y;   
        };
        struct S2 {
            int x;
            int y;
            int z;        
        };    
        union U {
            C c; // 멤버 변수들끼리 메모리를 공유함
            S1 s1;
            S2 s2;
        public:
            int GetX() const {return s1.x;} // 공용체도 함수를 가질 수 있음 
        };

        U u;
        EXPECT_TRUE(sizeof(u) == sizeof(S2));

        u.s1.x = 10; // s1을 바꿨지만, c와 s2도 변경됩니다.
        EXPECT_TRUE(u.GetX() == 10);
        EXPECT_TRUE(u.c.GetVal1() == 10);
        EXPECT_TRUE(u.s2.x == 10); 

        u.c.SetVal1(20); // c를 바꿨지만, s1과 s2도 변경됩니다.
        EXPECT_TRUE(u.c.GetVal1() == 20);
        EXPECT_TRUE(u.s1.x == 20);
        EXPECT_TRUE(u.s2.x == 20);
    }
}
