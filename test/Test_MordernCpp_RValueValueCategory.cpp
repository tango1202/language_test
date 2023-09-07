#include "gtest/gtest.h" 

namespace RValue_1 {
    class T {};

    int f(T& val) {return 10;}
    int f(T&& val) {return 20;}

    int g(T& val) {return f(val) + 1;}
    int g(T&& val) {return f(val) + 2;} // 여기서 val은 이름이 있으니 우측값이 아닙니다. 그래서 f(T& val)을 호출합니다.
}
namespace RValue_2 {
    class T {};

    int f(T& val) {return 10;}
    int f(T&& val) {return 20;}

    int g(T& val) {return f(val) + 1;}
    // int g(T&& val) {return f(std::move(val)) + 2;} // (△) 비권장. 이동 연산을 수행한다는 느낌이어서 가독성이 떨어집니다.
    int g(T&& val) {return f(std::forward<T>(val)) + 2;} // 값 카테고리를 유지한채 전달한다는 의미로 forward 사용
}

TEST(TestCppPattern, RValue) {

    // 왼쪽값(lvalue, left Value)과 우측값(rvalue, right value)
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
                std::cout<<"Big : Default Constructor"<<std::endl; 
            }

            // 소멸자
            ~Big() {
                std::cout<<"Big : Destructor"<<std::endl; 
                delete[] m_Ptr;
            }

            // 복사 생성자
            Big(const Big& other) :
                m_Size(other.m_Size) {
                std::cout<<"Big : Copy Constructor"<<std::endl; 

                // 메모리 공간 할당
                m_Ptr = new char[m_Size];

                // 메모리 복사
                memcpy(m_Ptr, other.m_Ptr, m_Size);
            }

            // 대입 연산자
            Big& operator =(const Big& other) {
                std::cout<<"Big : operator ="<<std::endl; 

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
                std::cout<<"Big : Default Constructor"<<std::endl; 
            }

            // 소멸자
            ~Big() {
                std::cout<<"Big : Destructor"<<std::endl; 
                delete[] m_Ptr;
            }

            // 복사 생성자
            Big(const Big& other) :
                m_Size(other.m_Size) {
                std::cout<<"Big : Copy Constructor"<<std::endl; 

                // 메모리 공간 할당
                m_Ptr = new char[m_Size];

                // 메모리 복사
                memcpy(m_Ptr, other.m_Ptr, m_Size);
            }

            // 대입 연산자
            Big& operator =(const Big& other) {
                std::cout<<"Big : operator ="<<std::endl; 

                Big temp(other);
                std::swap(m_Size, temp.m_Size);
                std::swap(m_Ptr, temp.m_Ptr);
                return *this;
            }

            Big& Move(Big& other) {
                std::cout<<"Big : Move()"<<std::endl; 

                delete[] m_Ptr; // 기존 것은 삭제하고,
                m_Size = other.m_Size; // other 것을 저장한뒤
                m_Ptr = other.m_Ptr;

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = NULL;
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
        class Big {
            size_t m_Size;
            char* m_Ptr; // 크기가 큰 데이터

        public:    
            explicit Big(size_t size) : 
                m_Size(size), 
                m_Ptr(new char[size]) {}

            // 소멸자
            ~Big() {delete[] m_Ptr;}

            // 복사 생성자
            Big(const Big& other) :
                m_Size(other.m_Size) {
                // 메모리 공간 할당
                m_Ptr = new char[m_Size];

                // 메모리 복사
                memcpy(m_Ptr, other.m_Ptr, m_Size);
            }

            // 대입 연산자. 좌측값(lvalue) 대입
            Big& operator =(const Big& other) {
                Big temp(other);
                std::swap(m_Size, temp.m_Size);
                std::swap(m_Ptr, temp.m_Ptr);
                return *this;
            }
            // 이동 생성자
            Big(Big&& other) : 
                m_Size(other.m_Size),
                m_Ptr(other.m_Ptr) {

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = NULL;
            }            
            // 이동 대입 연산자. 우측값(rvalue) 이동
            Big& operator =(Big&& other) {
                delete[] m_Ptr; // 기존 것은 삭제하고,
                m_Size = other.m_Size; // other 것을 저장한뒤
                m_Ptr = other.m_Ptr;

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = NULL;
                return *this;
            }
            
            size_t GetSize() const {return m_Size;}
        }; 

        Big a(10);
        Big b(20);

        a = b; // b는 이름이 있는 lvalue. 대입 연산자 호출
        EXPECT_TRUE(a.GetSize() == 20);   
        EXPECT_TRUE(b.GetSize() == 20);  

        a = static_cast<Big&&>(b); // b 를 이름이 없는 rvalue로 변환. 이동 대입 연산자 호출

        EXPECT_TRUE(a.GetSize() == 20);   
        EXPECT_TRUE(b.GetSize() == 0); // b는 이동되어 더이상 쓸 수 없음  

        a = Big(30); // Big(30) 이름이 없는 rvalue를 생성. 이동 대입 연산자 호출
        EXPECT_TRUE(a.GetSize() == 30); 

        Big c(40);
        a = std::move(c); // c 를 이름이 없는 rvalue로 변환. static_cast<Big&&>(c)와 동일. 이동 대입 연산자 호출
        EXPECT_TRUE(a.GetSize() == 40);

        Big d(50);
        Big e(std::move(d));
        EXPECT_TRUE(d.GetSize() == 0); // d는 이동되어 더이상 쓸 수 없음  
        EXPECT_TRUE(e.GetSize() == 50); 
    }
    // forward()
    {
        using namespace RValue_1;
        T t;

        // g(T& val)와 f(T& val)가 호출되어 11입니다.
        EXPECT_TRUE(g(t) == 11); 

        // g(T&& val)와 f(T& val)가 호출되어 12입니다.
        EXPECT_TRUE(g(std::move(t)) == 12);     
    }
    {
        using namespace RValue_2;
        T t;

        // g(T& val)와 f(T& val)가 호출되어 11입니다.
        EXPECT_TRUE(g(t) == 11); 

        // g(T&& val)와 f(T&& val)가 호출되어 22입니다.
        EXPECT_TRUE(g(std::move(t)) == 22);     
    }
}
