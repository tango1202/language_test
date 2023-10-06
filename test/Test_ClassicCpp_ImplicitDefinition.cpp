#include "gtest/gtest.h" 

TEST(TestClassicCpp, ImplicitDefinition) {
    {
        class Object {
            int m_X;
        };
        class T {
            int m_X;
            int m_Y;
            Object m_Object;
        };
    }
    // T에서 암시적으로 만들어 지는 것들
    {
        class Object {
            int m_X;
        };        
        class T {
            int m_X;
            int m_Y;
            Object m_Object;
        public:
            // 기본 생성자
            // (△) 암시적으로 기본 생성자가 자동 제로 초기화된다고 하는데, GCC 디버그 모드에서는 0이 아닙니다. 
            T() {}

            // 복사 생성자 - 멤버별 복사 생성자 호출
            T(const T& other) :
                m_X(other.m_X),
                m_Y(other.m_Y),
                m_Object(other.m_Object) {}

            // 복사 대입 연산자 - 멤버별 복사 대입
            T& operator =(const T& other) {
                m_X = other.m_X;
                m_Y = other.m_Y;
                m_Object = other.m_Object;
            
                return *this;
            }
            // 소멸자 - 특별한 작업 없음
            ~T() {}
        };
    }

}