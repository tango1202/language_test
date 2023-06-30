#include "gtest/gtest.h" 

namespace {
    // ----
    // 전방 선언
    // ----
    class YourClass; // 전방 선언
    class MyClass {
        // (O) 전방 선언을 통해 YourClass가 대충 클래스라는 걸 압니다. 
        // 반드시 포인터나 참조자와 같은 참조 형식이어야 합니다.
        // 구체 정의를 사용하려면, 헤더 파일과 cpp 파일을 분리하고, cpp 부분에서 구체 정의를 사용하세요.
        YourClass* your;
        
        void f(); // 선언만 하고, cpp에서 yourClass를 사용할 겁니다.
    };
    class YourClass {
        MyClass my; // MyClass는 상위에 정의되어 사용할 수 있습니다.
        
    public:
        void g() {}
    }; 

    // cpp 파일에서 YourClass의 구체 정의 사용
    void MyClass::f() {
        your->g(); 
    } 

    // ----
    // 멤버 사양
    // ----
    class T {
        int m_D1; // 멤버 변수
        static const int s_D2 = 1; // 정적 멤버 변수. 단 nested 클래스에서는 사용 못함
    
        virtual void f1(int) = 0; // 순가상 함수
        virtual void f2(int) {} // 가상 함수    
        void f3(int) {} // 멤버 함수
        static void f4(int) {} // 정적 멤버 함수
    
        enum {Left, Top, Right, Bottom}; // 열거형 상수
    
        class NestedClass { // 중첩 클래스(구조체)
            int m_D3;
        };
        typedef NestedClass Inner; // 형식 재정의
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

        S s = {10, 20}; // 중괄호 초기화 지원

        class C {
            int x; // 기본적으로 private
            int y;
        public:
            C(int x, int y) {} // 값 생성자 정의
        };
        C c(10, 20); // 값 생성자를 사용       
    }
    // ----
    // using 선언
    // ----   
    {
        class Base {
        protected:
            int val;
        };
        
        class Derived : public Base {
        public:
            using Base::val; // (△) 비권장. Base val을 public으로 변경합니다.
        };
        Derived d;
        d.val = 10;
        EXPECT_TRUE(d.val == 10); // 이제 public이라 접근 가능합니다.
    }
    // ----
    // 함수 내부의 로컬 클래스
    // ----   
    {
        class T {
        public:
            int x;
        };
        T t; // 함수내에서만 사용 가능
        t.x = 10; 
        EXPECT_TRUE(t.x == 10);
    }
    // ----
    // 공용체
    // ----  
    {
        class C {
        private:
            int val1;
            int val2;
        public:
            // C(int v1, int v2) {} // (X) 컴파일 오류. 공용체 멤버는 생성자를 가질 수 없음
            // ~C() {} // (X) 컴파일 오류. 공용체 멤버는 소멸자를 가질 수 없음
            int GetVal1() const {return val1;} // 멤버 함수가 있어도 되나, virtual 이면 안됨
            void SetVal1(int val) {val1 = val;}
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
