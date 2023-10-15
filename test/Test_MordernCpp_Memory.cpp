#include "gtest/gtest.h" 
#include <memory>

TEST(TestMordern, Memory) {
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
}