#include "gtest/gtest.h" 

namespace {
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
        T& operator =(const T& other) {return *this;} // 복사 대입 연산자
        
        operator int() const {return 0;} // 형변환 연산자

        void f1(int) {} // 멤버 함수
        void f2(int) const {} // 상수 멤버 함수

        virtual void f3(int) {} // 가상 함수    
        virtual void f4(int) = 0; // 순가상 함수

        static void f5(int) {} // 정적 멤버 함수
        // ----

        // 열거형 상수
        enum {Left, Top, Right, Bottom}; 
    
        // 중첩 클래스/구조체
        class NestedClass { 
            int m_D3;
        };
        
        // 타입 재정의
        typedef NestedClass Inner; 
    }; 
    // ----
    // friend
    // ----
    class FriendT {
        friend class U; // U 의 전방 선언이 없어도 됨
        friend void Func(); // Func의 전방 선언이 없어도 됨
    private:
        int m_Val1;
        void f() {}
    protected:
        int m_Val2;
        void g() {}
    
    };
    void Func() {
        FriendT t;
        t.m_Val1; // (△) 비권장. private 접근
        t.f();
        t.m_Val2; // (△) 비권장. protected 접근
        t.g();
    }
    class U {
        friend class W;
        void f() {
            FriendT t;
            t.m_Val1; // (△) 비권장. private 접근
            t.f();
            t.m_Val2; // (△) 비권장. protected 접근
            t.g();            
        }
    };
    class V : public U {
        void f() {
            FriendT t;
            // t.m_Val1; // (X) 컴파일 오류. 상속받은 클래스에서는 접근 불가
            // t.f();
            // t.m_Val2; // (X) 컴파일 오류. 상속받은 클래스에서는 접근 불가
            // t.g();            
        }
    };
    class W { // T의 friend인 U 의 friend
        void f() {
            FriendT t;
            // t.m_Val1; // (X) 컴파일 오류. friend의 friend는 접근 불가
            // t.f();
            // t.m_Val2; // (X) 컴파일 오류. friend의 friend는 접근 불가
            // t.g();   
        }
    }; 
    // ----
    // 함수 내부의 로컬 클래스
    // ----   
    void OuterFunc() {
        class InnerClass { // 클래스 가능
        public:
            int m_X;
        };
        struct InnerStruct {}; // 구조체 가능
        union InnerUnion {}; // 공용체 가능

        InnerClass t; // 함수내에서만 사용 가능
        t.m_X = 10; 
        EXPECT_TRUE(t.m_X == 10);
    }
    // ----
    // 중첩 클래스
    // ----  
    class Outer {
    private:
        int m_Val;
        static int s_Val;
    public:
        class Nested {
        public:
            void f(Outer *outer) {
                s_Val; // (O) Outer의 private 정적 멤버 변수 접근 가능
                outer->m_Val; // (O) outer 개체 참조로 private 접근 가능
            }
        };
        void g() {
            Nested nested;
            nested.f(this); // (O) 개체를 인스턴스화 하여 접근 가능
        }
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

        S s = {10, 20}; // 구조체는 집합 중괄호 초기화 지원

        class C {
            int m_X; // 기본적으로 private
            int m_Y;
        public:
            C(int x, int y) {} // 값 생성자 정의
        };
        C c(10, 20); // 클래스는 값 생성자만 가능. 중괄호 집합 초기화 미지원      
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
    // ----
    // 비트 필드
    // ----
    {
        class Flag {
        public:
            unsigned char m_Val1 : 2; // 2bit 00(0), 01(1), 10(2), 11(3)
            unsigned char m_Val2 : 3; // 3bit 000(0), 001(1), 010(2), 011(3), 100(4), 101(5), 110(6), 111(7)
        };

        Flag flag;
        EXPECT_TRUE(sizeof(flag) == sizeof(unsigned char));

        // 주어진 비트 범위내의 데이터는 잘 저장함
        flag.m_Val1 = 3; // 0~3 저장
        flag.m_Val2 = 7; // 0~7 저장
        
        EXPECT_TRUE(flag.m_Val1 == 3);
        EXPECT_TRUE(flag.m_Val2 == 7);
 
        // 저장 공간이 부족하면 상위 비트를 버림
        // flag.m_Val1 = 5; // (△) 비권장. 101을 대입하면 앞의 1은 저장하지 못하고 01만 저장됨  
        // flag.m_Val2 = 15; // (△) 비권장. 1111을 대입하면 앞의 1은 저장하지 못하고 111만 저장됨

        // EXPECT_TRUE(flag.m_Val1 == 1);
        // EXPECT_TRUE(flag.m_Val2 == 7);

        // unsigned char* ptr =  &flag.m_Val1; // (X) 컴파일 오류. 비트 필드는 포인터를 지원하지 않습니다.
        // unsigned char& ref =  flag.m_Val1; // (X) 컴파일 오류. 비트 필드는 레퍼런스를 지원하지 않습니다.
    }    
    // ----
    // 중첩 클래스
    // ----
    {
        // 클래스에서 가능
        class Class {
            class NestedClass {}; // 클래스 가능
            struct NestedStruct {}; // 구조체 가능
            union NestedUnion {}; // 공용체 가능
        };
        // 구조체에서 가능
        struct Struct {
            class NestedClass {};
            struct NestedStruct {};
            union NestedUnion {};
        };
        // 공용체에서 가능
        union Union {
            class NestedClass {}; 
            struct NestedStruct {}; 
            union NestedUnion {}; 
        };
    }
    {
        Outer::Nested nested; // (O) 외부에서 접근할 경우 :: 사용
    }

    // ----
    // this
    // ---- 
    {
        class Outer {
            int m_OuterVal;
            void f() {
                class Inner {
                    int m_InnerVal;
                    void f() {
                        this->m_InnerVal; // Inner 클래스의 this
                    }
                };
                this->m_OuterVal; // Outer 클래스의 this
            }
            class Nested {
                int m_NestedVal;
                void f() {
                    this->m_NestedVal; // Nested 클래스의 this
                }
            }; 
            static void Func() {
                // this->m_OuterVal; // (X) 컴파일 오류. 정적 멤버 함수에서는 접근 불가
            }
        };
    }
    {
        class T {
            int m_Val;
        public:
            explicit T(int val) :
                // this->m_Val(val), // (X) 컴파일 오류. 초기화 리스트에서 사용 불가능
                m_Val(this->m_Val) { // (O) 초기화 리스트에서 대입값으로는 사용 가능 
                this->m_Val; // (O) 생성자 본문에서 사용 가능
            }
        };        
    }
    {
        class T {
        public:
            ~T() {
                std::cout << "T::~T();" << std::endl;
            }
            void Release() {
                delete this; // (O) 자기 자신을 소멸 시킴(소멸자가 호출됨)
            }
        };

        T t1; // 스택에 생성된 변수
        // t1.Release(); // (X) 예외 발생. 스택에 생성된 개체인데 delete 함

        T* t2 = new T; // 힙에 생성된 변수
        t2->Release(); // (O)      
    }
}

