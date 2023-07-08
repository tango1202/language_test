#include "gtest/gtest.h" 

TEST(TestClassicCpp, Constructor) {
 
    // ----
    // 기본 생성자
    // ----
    {
        class T {
        public:
            T() {}
        };
        T t; // (O) 개체 정의(인스턴스화)
        // T t(); // (X) T를 리턴하는 함수 선언
    }
    {
        class T1 {
            int m_Val; // 암시적으로 기본 생성자가 정의됨. 자동 제로 초기화됨
        };

        class T2 {
        private:
            const int& m_Val; // 멤버 변수에 참조자가 있어, 암시적으로 생성한 기본 생성자에서 초기화 할 수 없음
        };

        class T3 {
            const T1 m_Val; // 멤버 변수에 상수형 개체가 있어, 암시적으로 생성한 기본 생성자에서 초기화 할 수 없음
        };

        class T4 {
        public:
            T4() {} // 사용자 정의 기본 생성자가 있음
        };

        class T5 {
        public:
            T5(int, int) {} // 값 생성자가 있어 암시적 기본 생성자가 정의되지 않음
        };
        
        class T6 {
        public:
            T6(const T6& other) {*this = other;} // 복사 생성자가 있어 암시적 기본 생성자가 정의되지 않음
        };

        T1 t1; // (O) 컴파일러가 암시적으로 정의한 기본 생성자
        // T2 t2; // (X) 컴파일 오류. 기본 생성자에서 멤버 변수 초기화 안됨
        // T3 t3; // (X) 컴파일 오류. 기본 생성자에서 멤버 변수 초기화 안됨 
        T4 t4; // (O) 사용자가 정의한 기본 생성자 사용
        // T5 t5; // (X) 컴파일 오류. 기본 생성자 정의 안됨
        // T6 t6; // (X) 컴파일 오류. 기본 생성자 정의 안됨
    }
    {
        class T {
            int m_Val;
        public:
            explicit T(int val) : // 값 생성자가 추가됨
                m_Val(val) {}
        };
        // T t; // (X) 컴파일 오류. 암시적 기본 생성자 없음     
    }
    // 명시적 기본 생성자
    {
        class T {
            int m_Val;
        public:
            T() : 
                m_Val(0) {} // (O) 명시적으로 기본 생성자를 만듭니다.
            explicit T(int val) : // 값 생성자가 추가됨
                m_Val(val) {}
        };
        T t; // (O) 명시적 기본 생성자 호출 
    }
    // ----
    // 값 생성자
    // ----
    {
        class T {
        private:
            int m_X;
            int m_Y;
        public:
            T(int x, int y) :
                m_X(x),
                m_Y(y) {
            }
        };
        T t(10, 20); // (O) 개체 정의(인스턴스화)
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
            U(int x, int y) { // (△) 비권장. 멤버 변수를 기본 생성자로 생성 합니다.
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
    // 초기화 순서
    {
        class T {
            int m_A;
            int m_B;
            int m_C;
        public:
            T(int a, int b, int c) :
                m_C(c + m_B), // 3
                m_B(b + m_A), // 2
                m_A(a) {} // 1
            int GetA() const {return m_A;}
            int GetB() const {return m_B;}
            int GetC() const {return m_C;}
        };
        T t(10, 20, 30);
        EXPECT_TRUE(t.GetA() == 10 && t.GetB() == 30 && t.GetC() == 60);        
    }
    // ----
    // 복사 생성자
    // ----
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T(int x, int y) : 
                m_X(x), 
                m_Y(y) {} 
            // 암시적 복사 생성자의 기본 동작은 멤버별 복사입니다.    
            // T(const T& other) :
            //     m_X(other.m_X),
            //     m_Y(other.m_Y) {}
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };
        T t1(10, 20);
        T t2 = t1;// (O) 타입이 같다면 복사 생성자 호출
        T t3(t1); // (O) 명시적으로 복사 생성자 호출

        EXPECT_TRUE(t2.GetX() == 10 && t2.GetY() == 20);
        EXPECT_TRUE(t3.GetX() == 10 && t3.GetY() == 20);
    }
    // 포인터 멤버 변수의 소유권 분쟁과 개체 핸들러
    {
        class T {
            int* m_Val;
        public:
            // val : new 로 생성된 것을 전달하세요.
            explicit T(int* val) :
                m_Val(val) {}

            // 암시적 복사 생성자의 기본 동작은 멤버별 복사입니다.    
            // T(const T& other) : 
            //     m_Val(other.m_Val) {} // !!동일한 힙 개체를 참조합니다.

            // 힙 개체를 메모리에서 제거 합니다.
            ~T() {delete m_Val;} 
        };
        // (X) 예외 발생. t1이 delete 한 것을 t2도 delete 합니다.
        {
            // T t1(new int(10));
            // T t2(t1); // 복사 생성의 결과 t1과 t2가 동일한 힙 개체를 참조합니다.
        } 
    }
    {
        class T {
            int* m_Val;
        public:
            // val : new 로 생성된 것을 전달하세요.
            explicit T(int* val) :
                m_Val(val) {}

            // (O) NULL 포인터가 아니라면 복제합니다.
            T(const T& other) {
                if (other.m_Val != NULL) { 
                    m_Val = new int(*other.m_Val);
                }
                else {
                    m_Val = NULL;
                }
            }

            // 힙 개체를 메모리에서 제거 합니다.
            ~T() {delete m_Val;} 
        };
        // (O) 힙 개체를 복제하여 소유권 분쟁이 없습니다.
        {
            T t1(new int(10));
            T t2(t1); // 새로운 int형 개체를 만들고 10을 복제합니다.
        }      
    }
    // Handler 사용
    {
        // 복사 생성이나 대입시 m_Ptr을 복제하고, 소멸시 delete 합니다.
        class Handler {
        private:
            int* m_Ptr; // new로 생성된 개체입니다.
        public: 
            Handler(int* ptr) :
                m_Ptr(ptr) {}

            // (O) NULL 포인터가 아니라면 복제합니다.    
            Handler(const Handler& other) {
                if (other.m_Ptr != NULL) { 
                    m_Ptr = new int(*other.m_Ptr); 
                }
                else {
                    m_Ptr = NULL;
                }
            }

            // (O) NULL 포인터가 아니라면 복제합니다.     
            Handler& operator =(const Handler& other) {
                // 기존에 관리하는 포인터는 삭제합니다. 사실 미리 삭제하는 건 예외 안정에 좋지 않습니다. 예외 안정 swap 참고
                delete m_Ptr;  

                if (other.m_Ptr != NULL) {
                    m_Ptr = new int(*other.m_Ptr);
                }
                else {
                    m_Ptr = NULL;
                }
                return *this;
            }

            // 힙 개체를 메모리에서 제거 합니다.
            ~Handler() {delete m_Ptr;}
        };

        class T {
            // (O) Handler를 이용하여 복사 생성과 대입시 포인터의 복제본을 만들고, 소멸시 Handler에서 delete 합니다.
            // 암시적 복사 생성자, 암시적 대입 연산자에서 정상 동작하므로, 명시적으로 구현할 필요가 없습니다.
            Handler m_Val;
        public:
            // val : new 로 생성된 것을 전달하세요.
            explicit T(int* val) :
                m_Val(val) {}
        };
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10));
            T t2(t1); // 새로운 int형 개체를 만들고 10을 복제합니다.
        } 
    }
    // 복사 생성자 사용 제한
    {
        class T {
        public:
            T() {}
        private:
            T(const T& other) {}
        };

        T t1;
        // T t2(t1); // (X) 컴파일 오류. 복사 생성자를 사용할 수 없게 private로 하여 단단히 코딩 계약을 했습니다.        
    }
    // ----
    // 대입 연산자
    // ----
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T(int x, int y) : 
                m_X(x), 
                m_Y(y) {} 
            // 암시적 복사 생성자의 기본 동작은 멤버별 복사입니다.    
            // T(const T& other) :
            //     m_X(other.m_X),
            //     m_Y(other.m_Y) {}
            // 암시적 대입 연산자의 기본 동작은 멤버별 대입입니다.    
            // T& operator =(const T& other) {
            //     m_X = other.m_X;
            //     m_Y = other.m_Y;
            // }

            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };
        T t1(10, 20);
        T t2(1, 2); 
        t2 = t1; // (O) 암시적 대입 연산자 호출

        EXPECT_TRUE(t2.GetX() == 10 && t2.GetY() == 20);
    }
    // ----
    // 생성자에서 가상 함수 호출 금지
    // ----
    {
        class Base {
        protected:    
            int m_Val;
        public:
            Base() : 
                m_Val(0) {
                // (△) 비권장. 가상 함수를 생성자에서 호출합니다.
                //  Derived::SetVal() 이 호출되길 기대하지만, 
                // Base::SetVal()이 호출됩니다.        
                SetVal(); 
            }
            virtual void SetVal() {
                m_Val = 1; // Base 에서는 1
            }
            int GetVal() const {return m_Val;}
        };

        class Derived : public Base {
        public:
            Derived() :
                Base() {} // Base의 기본 생성자를 호출하면서 가상 함수 SetVal()이 호출됩니다.
            virtual void SetVal() {
                m_Val = 2; // Derived 에서는 2
            }
        };

        Derived d;
        // (X) 오동작. Base 생성자에서 가상함수인 SeVal() 을 호출하면, 
        // Derived::SetVal() 이 호출되길 기대하나,
        // 아직 Derived가 완전히 생성되지 않은 상태이기에,
        // Base::SetVal() 이 호출됨
        EXPECT_TRUE(d.GetVal() == 1); 
    }
    // ----
    // 상속 전용 기반 클래스 - `protected` 생성자
    // ----
    {
        class Base {
        protected: // 개체 정의(인스턴스화)에서는 사용할 수 없고, 상속해서만 사용할 수 있습니다.
            Base() {} 
        public:
            virtual void f() {}
        };
        class Derived : Base {
            virtual void f() {}
        };

        // Base b; // (X) 컴파일 오류
        Derived d;
    }
}
