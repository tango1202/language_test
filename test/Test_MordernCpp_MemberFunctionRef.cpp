#include "gtest/gtest.h" 

namespace {

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

            std::cout << "Big_11 : Copy Constructor" << std::endl;

            // 메모리 공간 할당
            m_Ptr = new char[m_Size];

            // 메모리 복사
            memcpy(m_Ptr, other.m_Ptr, m_Size);
        }
        // 이동 생성자
        Big_11(Big_11&& other) : 
            m_Size(other.m_Size),
            m_Ptr(other.m_Ptr) {

            std::cout << "Big_11 : Move Constructor" << std::endl;

            other.m_Size = 0; // other는 초기화 합니다.
            other.m_Ptr = nullptr;
        }  
        
        size_t GetSize() const {return m_Size;}
    };     
}

TEST(TestMordern, MemberFunctionRef) {
    {
        class T {
        public:
            int Func_11() & {return 1;} // #1. 좌측값에서 호출
            int Func_11() && {return 2;} // #2. 우측값에서 호출
        };

        T t;
        EXPECT_TRUE(t.Func_11() == 1); // 좌측값이므로 #1 호출
        EXPECT_TRUE(std::move(t).Func_11() == 2); // move는 우측값이므로 #2 호출
        EXPECT_TRUE(T().Func_11() == 2); // T() 는 임시 개체(우측값)이므로 #2 호출   
    }
    
    {

        class A_11 {
            Big_11 m_Data;
        public:
            A_11() : m_Data(10) {}
            const Big_11& GetData() const {
                return m_Data;
            }
        };

        {
            // a은 좌측값. Big_11의 복사 생성자 호출
            A_11 a;
            Big_11 big = a.GetData(); 
        }
        {
             // a은 좌측값. std::move(a_11)는 우측값. Big_11의 복사 생성자 호출
            A_11 a;
            Big_11 big = std::move(a).GetData();            
        }
        {
            // A_11()은 임시 개체인 우측값. Big_11의 복사 생성자 호출
            Big_11 big = A_11().GetData(); 
        }  
 
        {
            // A_11().GetData()는 const Big_11&를 리턴하고, move() 는 const Big_11&& 을 리턴. 이동 생성자 인자 타입과 다르므로, 그냥 복사 생성자 호출 
            Big_11 big = std::move(A_11().GetData()); 
        }    
        {
            // Big_11의 이동 생성자 호출
            Big_11 big = std::move(const_cast<Big_11&>(A_11().GetData())); 
        }                 
    }
    {

        class A_11 {
            Big_11 m_Data;
        public:
            A_11() : m_Data(10) {}
            const Big_11& GetData() const & { // #1
                return m_Data;
            }

            Big_11&& GetData() && {
                return std::move(m_Data); // #2
            }
        };

        {
            // a은 좌측값. #1이 호출되어 const Big_11&을 리턴하고, Big_11의 복사 생성자 호출
            A_11 a;
            Big_11 big = a.GetData(); 
        }
        {
             // a_11은 좌측값. std::move(a_11)는 우측값. #2가 호출되어 Big_11&&을 리턴하고, Big_11의 이동 생성자 호출
            A_11 a;
            Big_11 big = std::move(a).GetData();            
        }
        {
            // A_11()은 임시 개체인 우측값. #2가 호출되어 Big_11&&을 리턴하고, Big_11의 이동 생성자 호출
            Big_11 big = A_11().GetData(); 
        }        
    }
}

