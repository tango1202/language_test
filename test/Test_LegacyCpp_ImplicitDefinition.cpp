#include "gtest/gtest.h" 

TEST(TestLegacyCpp, ImplicitDefinition) {
    {
        class Object {
            int m_X;
        };
        class T {
            int m_X;
            int m_Y;
        };
    }
    // T에서 암시적으로 만들어 지는 것들
    {
        class T {
            int m_X;
            int m_Y;
        public:
            // 기본 생성자
            T() {}

            // 복사 생성자 - 멤버별 복사 생성자 호출
            T(const T& other) :
                m_X(other.m_X),
                m_Y(other.m_Y) {}

            // 복사 대입 연산자 - 멤버별 복사 대입
            T& operator =(const T& other) {
                m_X = other.m_X;
                m_Y = other.m_Y;
            
                return *this;
            }
            // 소멸자 - 특별한 작업 없음
            ~T() {}
        };
    }

}