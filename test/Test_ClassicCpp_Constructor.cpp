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
        // T t(); // (X) T를 리턴하는 함수 t() 선언
    }
    {
        class T1 {
            int m_Val; // (△) 암시적으로 기본 생성자가 자동 제로 초기화된다고 하는데, GCC 디버그 모드에서는 0이 아닙니다. 명시적으로 초기화 하세요.
        };
        T1 t1; // (O) 컴파일러가 암시적으로 정의한 기본 생성자
       
        class T2 {
        private:
            const int& m_Val; // 멤버 변수에 참조자가 있어, 암시적으로 생성한 기본 생성자에서 초기화 할 수 없음
        };
        // T2 t2; // (X) 컴파일 오류. 기본 생성자에서 멤버 변수 초기화 안됨
        
        class T3 {
            const T1 m_Val; // 멤버 변수에 상수형 개체가 있어, 암시적으로 생성한 기본 생성자에서 초기화 할 수 없음
        };
        // T3 t3; // (X) 컴파일 오류. 기본 생성자에서 멤버 변수 초기화 안됨 
   
        class T4 {
        public:
            T4() {} // 사용자 정의 기본 생성자가 있음
        };
        T4 t4; // (O) 사용자가 정의한 기본 생성자 사용
        
        class T5 {
        public:
            T5(int, int) {} // 값 생성자가 있어 암시적 기본 생성자가 정의되지 않음
        };
        // T5 t5; // (X) 컴파일 오류. 기본 생성자 정의 안됨
        
        class T6 {
        public:
            T6(const T6& other) {*this = other;} // 복사 생성자가 있어 암시적 기본 생성자가 정의되지 않음
        };
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
            explicit T(int val = 0) : // 값 생성자의 기본값을 이용해 암시적 기본 생성자를 없앴습니다.
                m_Val(val) {}
        };
        T t1; // (O) 기본값으로 값 생성자 호출
        T t2(0); // (O) 기본값과 동일한 값으로 값 생성자 호출
        T t3(10); // (O) 임의 값으로 값 생성자 호출
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
            T(int x, int y) : // 필요한 모든 인자를 나열
                m_X(x), // 초기화 리스트를 이용하여 모든 멤버 변수 초기화
                m_Y(y) {}
        };
        T t(10, 20); // (O) 개체 정의(인스턴스화)
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
            // 암시적 복사 생성자의 기본 동작은 멤버별 복사 생성자 호출입니다.    
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

            // 암시적 복사 생성자의 기본 동작은 멤버별 복사 생성자 호출입니다.    
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
            T(const T& other) : 
                m_Val(other.m_Val != NULL ? new int(*other.m_Val) : NULL) {}

            // 힙 개체를 메모리에서 제거 합니다.
            ~T() {delete m_Val;} 
        };
        // (O) 힙 개체를 복제하여 소유권 분쟁이 없습니다.
        {
            T t1(new int(10));
            T t2(t1); // 새로운 int형 개체를 만들고 10을 복제합니다.
        }      
    }
    // 스마트 포인터 사용
    {
        // 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.(대입 연산은 지원하지 않습니다.)
        class IntPtr {
        private:
            int* m_Ptr; // new로 생성된 개체입니다.
        public: 
            explicit IntPtr(int* ptr) :
                m_Ptr(ptr) {}

            // (O) NULL 포인터가 아니라면 복제합니다.    
            IntPtr(const IntPtr& other) :
                m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {}

            // 힙 개체를 메모리에서 제거 합니다.
            ~IntPtr() {delete m_Ptr;}

            // 포인터 연산자 호출시 m_Ptr에 접근할 수 있게 합니다.
            const int* operator ->() const {return m_Ptr;}
            int* operator ->() {return m_Ptr;}

            const int& operator *() const {return *m_Ptr;}
            int& operator *() {return *m_Ptr;}

            // 유효한지 검사합니다.
            bool IsValid() const {return m_Ptr != NULL ? true : false;}    
        };

        class T {
            // (O) IntPtr을 이용하여 복사 생성시 포인터의 복제본을 만들고, 소멸시 IntPtr에서 delete 합니다.
            // (O) 암시적 복사 생성자에서 정상 동작하므로, 명시적으로 복사 생성자를 구현할 필요가 없습니다.
            IntPtr m_Val;
        public:
            // val : new 로 생성된 것을 전달하세요.
            explicit T(int* val) :
                m_Val(val) {}
            int GetVal() const {return *m_Val;}
        };
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10));
            T t2(t1); // 새로운 int형 개체를 만들고 10을 복제합니다.

            EXPECT_TRUE(t2.GetVal() == 10);
        } 
        // (X) 예외 발생. 2번 delete 합니다. 아직 대입 연산은 지원하지 않습니다.
        {
            T t1(new int(10));
            T t2(new int(20));
            // t2 = t1; // 아직 대입 연산은 지원하지 않습니다.
        }
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
                // Derived::SetVal() 이 호출되길 기대하지만, 
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
    // 생성자 사용 제한
    {
        class T {
        public:
            T(int, int) {} // (O) 값 생성자를 정의하면 암시적 기본 생성자를 사용할 수 없습니다.
        private:
            T(const T& other) {} // (O) private여서 외부에서 복사 생성자 사용할 수 없습니다.
        };

        // T t1; // (X) 컴파일 오류. 
        T t2(0, 0); // (O)
        // T t3(t1); // (X) 컴파일 오류. 복사 생성자를 사용할 수 없게 private로 하여 단단히 코딩 계약을 했습니다.    
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
    // ----
    // 생성자 접근 차단 - private 생성자
    // ----
    {
        class T {
        private:
            T(int a, int b, int c) {} // 외부에서는 접근 불가
        public:
            static T CreateFromA(int a) {return T(a, 0, 0);} // a값만 가지고 생성
            static T CreateFromB(int b) {return T(0, b, 0);} // b값만 가지고 생성
            static T CreateFromC(int c) {return T(0, 0, c);} // c값만 가지고 생성
        };

        // T t(10, 0, 0); // (X) 컴파일 오류
        // T* p = new T(10, 0, 0); // (X) 컴파일 오류
        // delete p;

        class U : public T {};
        // U u; // (X) 컴파일 오류. 상속해서 생성할 수 없음
        // U* p = new u; // (X) 컴파일 오류   

        T t(T::CreateFromA(10)); // (O) T를 복사 생성            
    }
}
