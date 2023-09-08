#include "gtest/gtest.h" 

TEST(TestCppPattern, DelegatingInheritedConstructor) {
    // 생성자 위임
    {
        class T {
            int m_X;
            int m_Y;
            int m_Z;
        public:
            T() : m_X(0), m_Y(0), m_Z(0) {}
            explicit T(int x) : m_X(x), m_Y(0), m_Z(0) {}
            T(int x, int y) : m_X(x), m_Y(y), m_Z(0) {}
            T(int x, int y, int z) : m_X(x), m_Y(y), m_Z(z) {}  
        };        
    }
    {
        class T {
            int m_X;
            int m_Y;
            int m_Z;
        public:
            T() : T(0, 0, 0) {} // T(int x, int y, int z)에 위임
            explicit T(int x) : T(x, 0, 0) {} // T(int x, int y, int z)에 위임
            T(int x, int y) : T(x, y, 0) {} // T(int x, int y, int z)에 위임
            T(int x, int y, int z) : m_X(x), m_Y(y), m_Z(z) {} 
        };
    }
    // 생성자 상속
    {
        class Base {
            int m_X;
            int m_Y;
        public:
            Base(int x, int y) : m_X(x), m_Y(y) {}
        };

        class Derived : public Base {
        public:
            // Derived 생성자에서 Base(x, y) 생성자를 명시적으로 호출
            Derived(int x, int y) : Base(x, y) {}
        };

        Derived d(10, 20);
    }
    {
        class Base {
            int m_X;
            int m_Y;
        public:
            Base(int x, int y) : m_X(x), m_Y(y) {}
        };

        class Derived : public Base {
        public:
            // 생성자 상속
            using Base::Base; 
        };

        Derived d(10, 20);    
    }    
}