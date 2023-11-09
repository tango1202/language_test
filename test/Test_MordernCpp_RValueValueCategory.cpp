#include "gtest/gtest.h" 

namespace RValue_1 {

    // 관리하는 개체를 복사 생성이나 이동 생성 합니다.
    template<typename T>
    class CopyableMoveable_11 {
        T* m_Data;
    public:
        CopyableMoveable_11() : m_Data(nullptr) {}

        explicit CopyableMoveable_11(T* data) : m_Data(data) {}
        
        // 복사 생성
        CopyableMoveable_11(const CopyableMoveable_11& other) : 
            m_Data(other.m_Data != nullptr ? new T(*other.m_Data) : nullptr) {
            std::cout << "CopyableMoveable_11 : Copy Constructor" << std::endl;       
        } 

        // 이동 생성
        CopyableMoveable_11(CopyableMoveable_11&& other) : 
            m_Data(other.m_Data) {
            std::cout << "CopyableMoveable_11 : Move Constructor" << std::endl;   

            other.m_Data = nullptr; // other는 초기화 합니다.
        }  

        ~CopyableMoveable_11() {delete m_Data;}
    };
}
namespace RValue_2 {
    class A {};

        int f_11(A&) {return 1;} // 인자가 좌측값 참조이면 호출됩니다.
        int f_11(A&&) {return 2;} // 인자가 우측값 참조이면 호출됩니다.
    }
namespace RValue_3 {
    class A {};

    int f_11(A& val) {return 10;}
    int f_11(A&& val) {return 20;}

    int g_11(A& val) {
        return f_11(val) + 1;
    }
    int g_11(A&& val) {
        return f_11(val) + 2; // 여기서 val은 이름이 있으니 우측값이 아닙니다. 그래서 f_11(A& val)을 호출합니다.
    }
}
namespace RValue_4 {
    class A {};

    int f_11(A& val) {return 10;}
    int f_11(A&& val) {return 20;}

    int g_11(A& val) {
        return f_11(val) + 1;
    }
    int g_11(A&& val) {
        return f_11(std::move(val)) + 2; // 우측값을 참조하는 좌측값인 val을 다시 우측값으로 형변환 합니다.
    }
}
namespace RValue_5 {
    class T {}; // 암시적 복사 생성자, 암시적 이동 생성자, 암시적 대입 연산자, 임시적 이동 연산자, 암시적 소멸자가 제공됩니다.

    void Swap_11(T& left, T& right) {
        T temp(std::move(right)); // 이동 생성자가 호출됩니다.
        right = std::move(left); // 이동 대입 연산자가 호출됩니다.
        left = std::move(temp); // 이동 대입 연산자가 호출됩니다.
    }    
}
namespace RValue_6 {
    class T {
    public:
        // 복사 생성자를 사용자 정의하여 암시적 이동 생성자, 암시적 이동 대입 연산자가 정의되지 않습니다.
        T(const T& other) {}
    }; 

    void Swap_11(T& left, T& right) {
        T temp(std::move(right)); // 이동 생성자가 없으므로 복사 생성자가 호출됩니다.
        right = std::move(left); // 이동 대입 연산자가 없으므로 복사 대입 연산자가 호출됩니다.
        left = std::move(temp); // 이동 대입 연산자가 없으므로 복사 대입 연산자가 호출됩니다.
    }    
}
TEST(TestMordern, RValue) {

    // 좌측값(lvalue, left Value)과 우측값(rvalue, right value)
    {
        int a = 0; 
        int* ptr1 = &a; // a는 &로 위치 참조가 되므로 좌측값
        // int* ptr2 = &0; // (X) 컴파일 오류. 상수 0은 & 위치 참조가 안되므로 우측값
        // int b = &(a + 10); // (X) 컴파일 오류. 수식 (a + 10)은 & 위치 참조가 안되므로 우측값
    }
    {
        int a = 0; 
        int& ptr1 = a; // a는 &로 참조되므로 좌측값
        // int& ptr2 = 0; // (X) 컴파일 오류. 상수 0은 &로 참조가 안되므로 우측값
        // int& b = (a + 10); // (X) 컴파일 오류. 수식 (a + 10)은 & 참조가 안되므로 우측값        
    }
    // 이동 성능
    {
        class Big {
            size_t m_Size;
            char* m_Ptr; // 크기가 큰 데이터

        public:    
            explicit Big(size_t size) : 
                m_Size(size), 
                m_Ptr(new char[size]) {
                std::cout << "Big : Default Constructor" << std::endl; 
            }

            // 소멸자
            ~Big() {
                std::cout << "Big : Destructor" << std::endl; 
                delete[] m_Ptr;
            }

            // 복사 생성자
            Big(const Big& other) :
                m_Size(other.m_Size) {
                std::cout << "Big : Copy Constructor" << std::endl; 

                // 메모리 공간 할당
                m_Ptr = new char[m_Size];

                // 메모리 복사
                memcpy(m_Ptr, other.m_Ptr, m_Size);
            }

            // 복사 대입 연산자
            Big& operator =(const Big& other) {
                std::cout << "Big : operator =" << std::endl; 

                Big temp(other); // 복사 생성
                std::swap(m_Size, temp.m_Size); // 바꿔치기
                std::swap(m_Ptr, temp.m_Ptr);
                return *this;
            }

            size_t GetSize() const {return m_Size;}
        }; 
        Big a(10);
        Big b(20);
        a = b;
        EXPECT_TRUE(a.GetSize() == 20);
        EXPECT_TRUE(b.GetSize() == 20);     
    }
    // 이동 성능 : Move
    {
        class Big {
            size_t m_Size;
            char* m_Ptr; // 크기가 큰 데이터

        public:    
            explicit Big(size_t size) : 
                m_Size(size), 
                m_Ptr(new char[size]) {
                std::cout << "Big : Default Constructor" << std::endl; 
            }

            // 소멸자
            ~Big() {
                std::cout << "Big : Destructor" << std::endl; 
                delete[] m_Ptr;
            }

            // 복사 생성자
            Big(const Big& other) :
                m_Size(other.m_Size) {
                std::cout << "Big : Copy Constructor" << std::endl; 

                // 메모리 공간 할당
                m_Ptr = new char[m_Size];

                // 메모리 복사
                memcpy(m_Ptr, other.m_Ptr, m_Size);
            }

            // 복사 대입 연산자
            Big& operator =(const Big& other) {
                std::cout << "Big : operator =" << std::endl; 

                Big temp(other);
                std::swap(m_Size, temp.m_Size);
                std::swap(m_Ptr, temp.m_Ptr);
                return *this;
            }

            // 이동 함수. 포인터의 대입으로만 구현됩니다.
            Big& Move(Big& other) {
                std::cout << "Big : Move()" << std::endl; 

                delete[] m_Ptr; // 기존 것은 삭제하고,
                m_Size = other.m_Size; // other 것을 저장한뒤
                m_Ptr = other.m_Ptr;

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = nullptr;
                return *this;
            }
            
            size_t GetSize() const {return m_Size;}
        }; 
        Big a(10);
        Big b(20);
        a.Move(b);
        EXPECT_TRUE(a.GetSize() == 20);   
        EXPECT_TRUE(b.GetSize() == 0);   
    }
    // 이동 대입 연산자 
    {
        class Big_11 {
            size_t m_Size;
            char* m_Ptr; // 크기가 큰 데이터

        public:    
            explicit Big_11(size_t size) : 
                m_Size(size), 
                m_Ptr(new char[size]) {}

            // 소멸자
            ~Big_11() {delete[] m_Ptr;}

            // 복사 생성자
            Big_11(const Big_11& other) :
                m_Size(other.m_Size) {
                // 메모리 공간 할당
                m_Ptr = new char[m_Size];

                // 메모리 복사
                memcpy(m_Ptr, other.m_Ptr, m_Size);
            }

            // 복사 대입 연산자. 좌측값(lvalue) 대입
            Big_11& operator =(const Big_11& other) {
                Big_11 temp(other);
                std::swap(m_Size, temp.m_Size);
                std::swap(m_Ptr, temp.m_Ptr);
                return *this;
            }
            // 이동 생성자
            Big_11(Big_11&& other) : 
                m_Size(other.m_Size),
                m_Ptr(other.m_Ptr) {

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = nullptr;
            }            
            // 이동 대입 연산자. 우측값(rvalue) 이동
            Big_11& operator =(Big_11&& other) {
                delete[] m_Ptr; // 기존 것은 삭제하고,
                m_Size = other.m_Size; // other 것을 저장한뒤
                m_Ptr = other.m_Ptr;

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = nullptr;
                return *this;
            }
            
            size_t GetSize() const {return m_Size;}
        }; 

        Big_11 a(10);
        Big_11 b(20);

        a = b; // b는 이름이 있는 lvalue. 복사 대입 연산자 호출
        EXPECT_TRUE(a.GetSize() == 20);   
        EXPECT_TRUE(b.GetSize() == 20);  

        a = static_cast<Big_11&&>(b); // b 를 이름이 없는 rvalue로 변환. 이동 대입 연산자 호출

        EXPECT_TRUE(a.GetSize() == 20);   
        EXPECT_TRUE(b.GetSize() == 0); // b는 이동되어 더이상 쓸 수 없음  

        Big_11 c(30);
        a = std::move(c); // c 를 이름이 없는 rvalue로 변환. static_cast<Big&&>(c)와 동일. 이동 대입 연산자 호출
        EXPECT_TRUE(a.GetSize() == 30);

        a = Big_11(40); // Big(40) 이름이 없는 rvalue를 생성. 이동 대입 연산자 호출
        EXPECT_TRUE(a.GetSize() == 40);

        Big_11 d(50);
        Big_11 e(std::move(d)); // 이동 생성자 호출
        EXPECT_TRUE(d.GetSize() == 0); // d는 이동되어 더이상 쓸 수 없음  
        EXPECT_TRUE(e.GetSize() == 50); 
    }
    // 우측값 참조
    {
        class A {};

        A val;

        // A&& rref1 = val; // (X) 컴파일 오류. val은 좌측값입니다. 좌측값을 직접 참조할 수 없습니다.
        A&& rref2 = static_cast<A&&>(val); // 좌측값을 형변환 하여 참조할 수 있습니다.
        A&& rref3 = std::move(val); // 좌측값을 move를 이용하여 형변환하여 참조할 수 있습니다.
        A&& rref4 = A(); // 임시 개체는 우측값이므로 참조할 수 있습니다.
    }
    {
        class A {}; 

        A val;    
        A& ref = val;  
        A&& rref = std::move(val); 

        // A&& rref1 = ref; // (X) 컴파일 오류. 우측값 참조는 좌측값 참조를 참조할 수 없습니다.
        A&& rref2 = static_cast<A&&>(ref); // 좌측값 참조를 형변환하여 참조할 수 있습니다.
        A&& rref3 = std::move(ref); // 좌측값 참조를 move를 이용하여 형변환하여 참조할 수 있습니다.

        // A& ref1 = std::move(val); // (X) 컴파일 오류. 좌측값 참조는 우측값을 참조할 수 없습니다.
        // A& ref2 = static_cast<A&>(std::move(val)); // (X) 컴파일 오류. 좌측값 참조는 우측값을 참조할 수 없습니다.
        // A& ref3 = std::move(val); // (X) 컴파일 오류. 좌측값 참조는 우측값을 참조할 수 없습니다.
        // A& ref4 = A(); // (X) 컴파일 오류. 좌측값 참조는 임시 개체인 우측값을 참조할 수 없습니다.
        
        A& ref5 = rref; // 이름이 부여된 우측값 참조는 좌측값이기에 좌측값 참조로 참조할 수 있습니다.
    }
    // 이름이 부여된 우측값
    {
        using namespace RValue_2;
 
        A lvalue;
        A&& rvalue_11 = std::move(lvalue); // rvalue는 이름이 부여됐으므로 좌측값입니다.
        EXPECT_TRUE(f_11(rvalue_11) == 1); // f_11(A&)를 호출합니다.
        EXPECT_TRUE(f_11(std::move(lvalue)) == 2); // f_11(A&&)를 호출합니다.
    }
    {
        using namespace RValue_3;
   
        A lvalue;

        // g_11(A& val)와 f_11(A& val)가 호출되어 11입니다.
        EXPECT_TRUE(g_11(lvalue) == 11); 

        // g_11(A&& val)와 f_11(A& val)가 호출되어 12입니다.
        EXPECT_TRUE(g_11(std::move(lvalue)) == 12); 
    }
    {
        using namespace RValue_4;
   
        A lvalue;

        // g_11(A& val)와 f_11(A& val)가 호출되어 11입니다.
        EXPECT_TRUE(g_11(lvalue) == 11); 

        // g_11(A&& val)와 f_11(A&& val)가 호출되어 22입니다.
        EXPECT_TRUE(g_11(std::move(lvalue)) == 22); 
    }    
    // 암시적 이동 생성자와 암시적 이동 대입 연산자의 default 정의
    {
        using namespace RValue_1;

        class A_11 {
        private:
            CopyableMoveable_11<int> m_Data;
        public:
            explicit A_11(int* data) : m_Data(data) {}
        };

        A_11 a(new int(10));
        A_11 b(a); // 복사 생성자 호출
        A_11 c(std::move(a)); // 이동 생성자 호출
    } 
    {
        using namespace RValue_1;

        class A_11 {
        private:
            CopyableMoveable_11<int> m_Data;
        public:
            explicit A_11(int* data) : m_Data(data) {}
            ~A_11() {} // 암시적 이동 생성자와 암시적 이동 대입 연산자가 만들어 지지 않습니다.
        };

        A_11 a(new int(10));
        A_11 b(a); // 복사 생성자 호출
        A_11 c(std::move(a)); // (△) 비권장. 복사 생성자 호출
    }    
    {
        using namespace RValue_1;

        class A_11 {
        private:
            CopyableMoveable_11<int> m_Data;
        public:
            explicit A_11(int* data) : m_Data(data) {}
            A_11(A_11&) = default; // 암시적 복사 생성자를 사용합니다.
            A_11(A_11&&) = default; // 암시적 이동 생성자를 사용합니다.
            ~A_11() {} // 암시적 이동 생성자와 암시적 이동 대입 연산자가 만들어 지지 않습니다.
        };

        A_11 a(new int(10));
        A_11 b(a); // 복사 생성자 호출
        A_11 c(std::move(a)); // 이동 생성자 호출
    } 
    // 이동 연산을 이용한 리팩토링
    {
        class T {
        public:
            // 복사 대입 연산자를 정의합니다. 이에 따라 암시적 이동 대입 연산자는 정의되지 않습니다.
            T& operator =(const T& other) {
                std::cout << "T : Copy" << std::endl;
                return *this;
            }
        };

        T a;
        T b;

        b = a; // 복사 대입 합니다.
        b = T(); // T()는 임시 개체인 우측값입니다. 복사 대입 연산자가 정의되어 이동 대입 연산자가 암시적으로 정의되지 않았고, 따라서 복사 대입 합니다.
    } 
    
    {
        class T_11 {
        public:
            // 복사 대입 연산자입니다.
            T_11& operator =(const T_11& other) {
                std::cout << "T : Copy" << std::endl;
                return *this;
            }
            // 이동 대입 연산자를 정의합니다.
            T_11& operator =(T_11&& other) noexcept {
                std::cout << "T : Move" << std::endl;
                return *this;        
            }
        };

        T_11 a;
        T_11 b;

        b = a; // 복사 대입 합니다.
        b = T_11(); // T()는 임시 개체인 우측값입니다. 이동 대입 합니다.
    }

    // move
    {
        class A{};
        class T {
        public:
            static int Func_11(A&) {return 1;} // #1. lvalue
            static int Func_11(A&&) {return 2;} // #2. rvalue
        };

        A a;

        EXPECT_TRUE(T::Func_11(a) == 1); // 이름이 있는 lvalue
        EXPECT_TRUE(T::Func_11(static_cast<A&&>(a)) == 2); // 강제 형변환 해서 rvalue
        EXPECT_TRUE(T::Func_11(std::move(a)) == 2); // lvalue를 move 해서 rvalue

        A& b = a;
        EXPECT_TRUE(T::Func_11(b) == 1);  
        EXPECT_TRUE(T::Func_11(static_cast<A&&>(b)) == 2);
        EXPECT_TRUE(T::Func_11(std::move(b)) == 2);

        EXPECT_TRUE(T::Func_11(A()) == 2); // 임시 개체는 rvalue
        EXPECT_TRUE(T::Func_11(std::move(A())) == 2); // 임시 개체를 move 해도 rvalue
    }

    // move_no_except
    {
        class A_11 {
        public:
            A_11() {}
            // noexcept가 없어서 예외가 발생할 수도 있습니다.
            A_11(const A_11&) {std::cout << "A_11 : Copy Constructor" << std::endl;}
            A_11(A_11&&) {std::cout << "A_11 : Move Constructor" << std::endl;;} 
        };
        class B_11 {
        public:
            B_11() {}
            // nothrow 보증합니다.
            B_11(const B_11&) noexcept {std::cout << "B_11 : Copy Constructor" << std::endl;;}
            B_11(B_11&&) noexcept {std::cout << "B_11 : Move Constructor" << std::endl;;} 
        };

        A_11 a1;
        A_11 a2 = std::move_if_noexcept(a1); // A_11(A_11&&)가 nothrow 예외 보증이 되지 않아 그냥 A(const A&)를 호출합니다.

        B_11 b1;
        B_11 b2 = std::move_if_noexcept(b1); // B_11(B_11&&)가 nothrow 예외 보증이 되어 B_11(B_11&&)를 호출합니다.    }
    }

}
