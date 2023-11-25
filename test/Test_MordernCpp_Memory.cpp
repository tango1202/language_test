#include "gtest/gtest.h" 
#include <memory>

TEST(TestMordern, Memory) {
#if 201703L <= __cplusplus // C++17~    
    // C++17 메모리 유틸리티 작업
    {
        class T {
        private:
            int m_X;
            int m_Y;
        public:
            T() : 
                m_X{10}, 
                m_Y{20} {
                std::cout << "T : Constructor" << std::endl;
            }    
            ~T() {
                std::cout << "T : Destructor" << std::endl;
            }
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };
                
        unsigned char buffer[sizeof(T) * 3]; // T 개체 3개를 저장할 수 있는 스택 영역을 확보합니다.

        // 3개의 생성자 호출
        T* t1 = new(static_cast<void*>(buffer + sizeof(T) * 0)) T;
        T* t2 = new(static_cast<void*>(buffer + sizeof(T) * 1)) T;
        T* t3 = new(static_cast<void*>(buffer + sizeof(T) * 2)) T;

        EXPECT_TRUE(t1->GetX() == 10 && t1->GetY() == 20);
        EXPECT_TRUE(t2->GetX() == 10 && t2->GetY() == 20);
        EXPECT_TRUE(t3->GetX() == 10 && t3->GetY() == 20);

        // 3개의 소멸자 호출
        t1->~T();
        t2->~T();
        t3->~T();        
    }
    {
        class T {
        private:
            int m_X;
            int m_Y;
        public:
            T() : 
                m_X{10}, 
                m_Y{20} {
                std::cout << "T : Constructor" << std::endl;
            }    
            ~T() {
                std::cout << "T : Destructor" << std::endl;
            }
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };

        unsigned char data[sizeof(T) * 3]; // T 개체 3개를 저장할 수 있는 스택 영역을 확보합니다.

        auto begin{reinterpret_cast<T*>(data)};
        auto end{begin + 3}; 

        // 요소 3개의 생성자 호출
        std::uninitialized_default_construct(begin, end);
        EXPECT_TRUE((begin + 0)->GetX() == 10 && (begin + 0)->GetY() == 20);
        EXPECT_TRUE((begin + 1)->GetX() == 10 && (begin + 1)->GetY() == 20);
        EXPECT_TRUE((begin + 2)->GetX() == 10 && (begin + 2)->GetY() == 20);

        // 요소 3개의 소멸자 호출
        std::destroy(begin, end);
    }
#endif

#if 201703L <= __cplusplus // C++17~
    // C++17 launder
    {
        class A {
        public: 
            const int m_X;  // const 멤버 입니다. 
            int m_Y;
            A(int x, int y) : m_X{x}, m_Y{y} {}
        };

        A* ptr = new A{1, 2};
        EXPECT_TRUE(ptr->m_X == 1 && ptr->m_Y == 2);

        // 위치 지정 생성으로 ptr 위치에 다시 생성합니다.
        A* newPtr = new(ptr) A{3, 4};
        EXPECT_TRUE(newPtr->m_X == 3 && newPtr->m_Y == 4);

        // 이전 포인터인 ptr로 다루려면, launder를 사용합니다.
        EXPECT_TRUE((ptr->m_X == 1 || ptr->m_X == 3)); // (△) 비권장. 컴파일러 최적화에 의해 const 변수는 이전값 1을 가질 수도 있습니다.
        EXPECT_TRUE(ptr->m_Y == 4); // (△) 비권장. 불법입니다만, 잘 동작합니다.
        EXPECT_TRUE(std::launder(ptr)->m_X == 3); // launder를 이용하여 합법적으로 사용할 수 있습니다.

        delete std::launder(ptr);
    }
#endif    
}