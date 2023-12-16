#include "gtest/gtest.h" 
#include <algorithm>

TEST(TestLegacyCpp, AssignmentOperator) {
    // ----
    // 복사 대입 연산자
    // ----
    {
        class T {
        public:
            T& operator =(const T& other) {
                return *this;
            }
            void f() {}
        };
        T a, b, c;
        (a = b = c).f();
    }
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
            
            // 암시적 복사 대입 연산자의 기본 동작은 멤버별 복사 대입입니다.    
            // T& operator =(const T& other) {
            //     m_X = other.m_X;
            //     m_Y = other.m_Y;
            // }

            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };
        T t1(10, 20);
        T t2(1, 2); 
        t2 = t1; // (O) 암시적 복사 대입 연산자 호출

        EXPECT_TRUE(t2.GetX() == 10 && t2.GetY() == 20);
    }
    // ----
    // swap을 이용한 예외 안전 복사 대입 연산자
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
            
            T& operator =(const T& other) {

                // other를 복제한 임시 개체를 만듭니다.
                T temp(other); // (O) 생성시 예외가 발생하더라도 this는 그대로 입니다.

                // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
                // this는 이제 other를 복제한 값을 가집니다.
                Swap(temp); 

                return *this;
                
            } // temp는 지역 변수여서 자동으로 소멸됩니다.

            // 멤버 변수들의 값을 바꿔치기 합니다.
            void Swap(T& other) {
                // (△) 비권장. int 형이라 복사 부하가 크지는 않습니다만, 
                // 조금 큰 개체라면 복사 부하가 있고 예외를 발생할 수 있습니다.
                std::swap(this->m_X, other.m_X); 
                std::swap(this->m_Y, other.m_Y);
            }

            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };
        T t1(10, 20);
        T t2(1, 2); 
        t2 = t1; // (O) swap 버전 복사 대입 연산자 호출

        EXPECT_TRUE(t2.GetX() == 10 && t2.GetY() == 20);
    }
    // swap 복사 부하
    {
        class T {
        public:
            T() {}
            T(const T& other) {
                std::cout << "T::T(const T& other)" << std::endl;   
            }
            T& operator =(const T& other) {
                std::cout << "T::operator =()" << std::endl;
                return *this; 
            }
        };
        T t1;
        T t2;
        t1 = t2; // 복사 대입 1회
        std::swap(t1, t2); // 복사 생성 1회 복사 대입 2회
    }
    // nothrow swap
    {
        class Big {
            int m_Val; // #1. 실제로는 복사 부하가 큰 데이터라고 생각해 주세요.
        public:
            explicit Big(int val) : 
                m_Val(val) {}
            Big(const Big& other) : 
                m_Val(other.m_Val) {
                std::cout << "Big::Big(const Big& other)" << std::endl; // #2 
            }
            Big& operator =(const Big& other) {
                m_Val = other.m_Val;
                std::cout << "Big::operator =(const Big& other)" << std::endl; // #2  
                return *this;
            }    
            int GetVal() const {return m_Val;}
            void SetVal(int val) {m_Val = val;}
        };
        class T {
            Big* m_Big; // #3. 복사 부하가 큰 데이터는 포인터로 관리합니다.
        public:
            explicit T(Big* big) : 
                m_Big(big) {} 
            // NULL 포인터가 아니라면 복제합니다.
            T(const T& other) :
                m_Big(other.m_Big != NULL ? new Big(*other.m_Big) : NULL) { // #4
            }
            // 힙 개체를 메모리에서 제거 합니다.
            ~T() {
                delete m_Big; // #4
            }
            
            T& operator =(const T& other) { // #5

                // other를 복제한 임시 개체를 만듭니다.
                T temp(other); // (O) 생성시 예외가 발생하더라도 this는 그대로 입니다.

                // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
                // this는 이제 other를 복제한 값을 가집니다.
                Swap(temp); 

                return *this;
                
            } // temp는 지역 변수여서 자동으로 소멸됩니다.

            // 멤버 변수들의 값을 바꿔치기 합니다.
            void Swap(T& other) { // #6
                // (O) 포인터 변수끼리의 복사/대입이라 복사 부하가 크지 않습니다.
                // 예외가 발생할 확률도 낮습니다.
                std::swap(this->m_Big, other.m_Big); 
            }

            const Big* GetBig() const {return m_Big;}
        };
        T t1(new Big(10));
        T t2(new Big(1)); 
        t2 = t1; // (O) swap 버전 복사 대입 연산자 호출

        EXPECT_TRUE(t2.GetBig()->GetVal() == 10);
    }
    // ----
    // 복사 대입 연산자까지 지원하는 스마트 포인터
    // ----
    {
        // 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
        // 복사 대입 연산은 임시 개체 생성 후 swap 합니다.
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

            IntPtr& operator =(const IntPtr& other) {

                // other의 힙 개체를 복제한 임시 개체를 만듭니다.
                IntPtr temp(other); // (O) 생성시 예외가 발생하더라도 this는 그대로 입니다.

                // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
                // this는 이제 other의 힙 개체를 복제한 값을 가집니다.
                Swap(temp); // (O) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.

                return *this;
                // temp는 지역 변수여서 자동으로 소멸됩니다.
                // 소멸되면서 this가 이전에 가졌던 힙 개체를 소멸합니다.
            }
            // 멤버 변수들의 값을 바꿔치기 합니다.
            void Swap(IntPtr& other) {
                std::swap(this->m_Ptr, other.m_Ptr); // (O) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.   
            }

            // 포인터 연산자 호출시 m_Ptr에 접근할 수 있게 합니다.
            const int* operator ->() const {return m_Ptr;}
            int* operator ->() {return m_Ptr;}

            const int& operator *() const {return *m_Ptr;}
            int& operator *() {return *m_Ptr;}

            // 유효한지 검사합니다.
            bool IsValid() const {return m_Ptr != NULL ? true : false;}    
        };

        class T {
            // (O) IntPtr로 복사 생성과 복사 대입시 포인터의 복제본을 만들고, 소멸시 IntPtr에서 delete 합니다.
            // (O) 암시적 복사 생성자에서 정상 동작하므로, 명시적으로 복사 생성자를 구현할 필요가 없습니다.
            // (O) 포인터 멤버 변수가 1개 있고, 내부적으로 복사 대입 연산시 swap하므로 대입 연산자를 구현할 필요가 없습니다.
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
        // (O) 복사 대입 연산 시에도 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10));
            T t2(new int(20));
            t2 = t1; // (O) swap 버전 복사 대입 연산자 호출
            EXPECT_TRUE(t2.GetVal() == 10);
        }
    }
    // ----
    // 멤버 변수가 2개 이상인 경우 스마트 포인터와 대입 연산자와의 호환성
    // ----
    {
        // 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
        // 복사 대입 연산은 임시 개체 생성 후 swap 합니다.
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

            IntPtr& operator =(const IntPtr& other) {

                // other의 힙 개체를 복제한 임시 개체를 만듭니다.
                IntPtr temp(other); // (O) 생성시 예외가 발생하더라도 this는 그대로 입니다.

                // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
                // this는 이제 other의 힙 개체를 복제한 값을 가집니다.
                Swap(temp); // (O) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.

                return *this;
                // temp는 지역 변수여서 자동으로 소멸됩니다.
                // 소멸되면서 this가 이전에 가졌던 힙 개체를 소멸합니다.
            }
            // 멤버 변수들의 값을 바꿔치기 합니다.
            void Swap(IntPtr& other) {
                std::swap(this->m_Ptr, other.m_Ptr); // (O) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.   
            }

            // 포인터 연산자 호출시 m_Ptr에 접근할 수 있게 합니다.
            const int* operator ->() const {return m_Ptr;}
            int* operator ->() {return m_Ptr;}

            const int& operator *() const {return *m_Ptr;}
            int& operator *() {return *m_Ptr;}

            // 유효한지 검사합니다.
            bool IsValid() const {return m_Ptr != NULL ? true : false;}    
        };

        class T {
            // (O) IntPtr로 복사 생성과 복사 대입시 포인터의 복제본을 만들고, 소멸시 IntPtr에서 delete 합니다.
            // (O) 암시적 복사 생성자에서 정상 동작하므로, 명시적으로 복사 생성자를 구현할 필요가 없습니다.
            // (O) 포인터 멤버 변수가 2개 있어, 예외에 안전하지 않으므로 swap으로 대입 연산자를 구현합니다.
            IntPtr m_Val1;
            IntPtr m_Val2;
        public:
            // val1, val2 : new 로 생성된 것을 전달하세요.
            T(int* val1, int* val2) :
                m_Val1(val1),
                m_Val2(val2) {}
            T& operator =(const T& other) {
                T temp(other); // (O) 생성시 예외가 발생하더라도 this는 그대로 입니다.
                Swap(temp); // (O) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.
                return *this;
            } 
            void Swap(T& other) {
                m_Val1.Swap(other.m_Val1); // 포인터 끼리의 값 변경이므로 복사 부하도 없고, 예외가 발생하지 않습니다. 
                m_Val2.Swap(other.m_Val2);
            }

            int GetVal1() const {return *m_Val1;}
            int GetVal2() const {return *m_Val2;}
        };
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10), new int(20));
            T t2(t1); // 새로운 int형 개체를 만들고 10, 20을 복제합니다.

            EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
        } 
        // (O) 복사 대입 연산 시에도 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10), new int(20));
            T t2(new int(1), new int (2));
            t2 = t1; // (O) swap 버전 복사 대입 연산자 호출
            EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
        }
    }  
}

