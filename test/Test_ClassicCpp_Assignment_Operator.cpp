#include "gtest/gtest.h" 
#include <algorithm>

TEST(TestClassicCpp, AssignmentOperator) {
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
            // 암시적 복사 생성자의 기본 동작은 멤버별 복사 생성자 호출입니다.    
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
    // 포인터 멤버 변수의 소유권 분쟁과 개체 핸들러
    // ----
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

            T& operator =(const T& other) {

                // (△) 비권장. 기존에 관리하는 포인터는 삭제합니다. 사실 미리 삭제하는 건 예외 안정에 좋지 않습니다. `swap`을 이용한 대입 연산자 구현 참고
                delete m_Val; 

                if (other.m_Val != NULL) { 
                    m_Val = new int(*other.m_Val);
                }
                else {
                    m_Val = NULL;
                }

                return *this;
            }

            // 힙 개체를 메모리에서 제거 합니다.
            ~T() {delete m_Val;} 
        };
        // (O) 힙 개체를 복제하여 소유권 분쟁이 없습니다.
        {
            T t1(new int(10));
            T t2(t1); // 새로운 int형 개체를 만들고 10을 복제합니다.
            T t3(new int(20)); 
            t3 = t2; // (O) t3의 힙 개체는 delete 하고, t2의 힙개체를 복제합니다.
        }    
    }
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
                // (△) 비권장. 기존에 관리하는 포인터는 삭제합니다. 사실 미리 삭제하는 건 예외 안정에 좋지 않습니다. `swap`을 이용한 대입 연산자 구현 참고
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
            // 암시적 복사 생성자와 암시적 대입 연산자에서 정상 동작하므로, 명시적으로 복사 생성자와 대입 연산자를 구현할 필요가 없습니다.
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
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10));
            T t2(new int(20)); 
            t2 = t1;// t2의 힙 개체를 delete 하고, t1의 힙 개체를 복제합니다.
        }         
    }
    // ----
    // swap을 이용한 예외 안정 대입 연산자 구현
    // ----
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

            // (O) 예외에 안정적이도록 swap을 이용하여 대입합니다.   
            Handler& operator =(const Handler& other) {

                // other의 힙 개체를 복제한 임시 개체를 만듭니다.
                Handler temp(other); // (O) 생성시 예외가 발생하더라도 this는 그대로 입니다.

                // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
                // this는 이제 other의 힙 개체를 복제한 값을 가집니다.
                Swap(temp); // (O) 포인터 끼리의 값 변경이므로 예외가 발생하지 않습니다.

                return *this;
                // temp는 지역 변수여서 자동으로 소멸됩니다.
                // 소멸되면서 this가 이전에 가졌던 힙 개체를 소멸합니다.
            }

            // 힙 개체를 메모리에서 제거 합니다.
            ~Handler() {delete m_Ptr;}

            // 멤버 변수들의 값을 바꿔치기 합니다.
            void Swap(Handler& other) {
                std::swap(this->m_Ptr, other.m_Ptr); // (O) 포인터 끼리의 값 변경이므로 예외가 발생하지 않습니다.   
            }
        };

        class T {
            // (O) Handler를 이용하여 복사 생성과 대입시 포인터의 복제본을 만들고, 소멸시 Handler에서 delete 합니다.
            // 암시적 복사 생성자와 암시적 대입 연산자에서 정상 동작하므로, 명시적으로 복사 생성자와 대입 연산자를 구현할 필요가 없습니다.
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
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10));
            T t2(new int(20)); 
            t2 = t1; // t1의 힙 개체를 복제후 대입하고, t2의 이전 힙 개체를 delete 합니다.
        }        
    }
}
