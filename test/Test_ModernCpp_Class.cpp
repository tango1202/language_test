#include "gtest/gtest.h" 

namespace {

    // delete를 이용한 템플릿 인스턴스화 차단
    template<typename T> 
    void Func_11(T) {}

    template<> 
    void Func_11<char>(char) = delete; // char 버전 함수 삭제

}
TEST(TestModern, DefaultDeleteOverrideFinal) {
    // default와 delete
    {
        class T_11 {
        public:
            T_11() = default; // 암시적 버전의 기본 생성자 사용
            T_11(const T_11&) = delete; // 암시적 버전의 복사 생성자 막음    
        };
    }
    // delete를 이용한 암시적 형변환 차단
    {
        class T_11 {
        public:
            void Func_11(int) {}
            void Func_11(char) = delete;
        };

        T_11 t;
        t.Func_11(10);
        // t.Func_11('a'); // (X) 컴파일 오류. delete된 함수입니다.
    }
    // delete를 이용한 템플릿 인스턴스화 차단
    {
        Func_11(10);
        // Func_11('a'); // (X) 컴파일 오류. delete된 함수입니다.
    }
    // override
    {
        class Base {
        public:
            virtual void Func1() {};
            virtual void Func2() {};
        };
        class Derived : public Base {
            virtual void Func1() {}; // (O)
            virtual void Func2() {}; // (O)
            virtual void Func_2() {}; // (△) 비권장. 오타일까요? 새로운 가상 함수를 정의한 것일까요?
        };
    }
    {
        class Base {
        public:
            virtual void Func1() {};
            virtual void Func2() {};
        };
        class Derived_11 : public Base {
            virtual void Func1() override {}; // (O)
            virtual void Func2() override {}; // (O)
            // virtual void Func_2() override {}; // (X) 컴파일 오류. 부모 개체에 해당 멤버 없음
        };       
    }
    // final
    {
        class Base_11 final {
        };
        // (X) 컴파일 오류. Base_11은 상속할 수 없습니다.
        // class Derived_11 : public Base_11 {
        // };
    }
    {
        class Base_11 {
        public:
            virtual void Func1() {};
            virtual void Func2() final {};
        };
        class Derived1_11 : public Base_11 {
            virtual void Func1() override final {}; // (O) Base를 오버라이드하고, 자식 개체에서는 오버라이드 못하게 합니다.
            // virtual void Func2() override {}; // (X) 컴파일 오류. Func2는 오버라이드 할 수 없습니다.
        }; 
        class Derived2_11 : public Derived1_11 {
            // virtual void Func1() override final {}; // (X) 컴파일 오류. Func1은 오버라이드 할 수 없습니다.
        };               
    }
}

TEST(TestModern, DelegatingInheritedConstructor) {
    // 생성자 위임
    {
        class T {
            int m_X;
            int m_Y;
            int m_Z;
        public:
            T() : m_X{0}, m_Y{0}, m_Z{0} {}
            explicit T(int x) : m_X{x}, m_Y{0}, m_Z{0} {}
            T(int x, int y) : m_X{x}, m_Y{y}, m_Z{0} {}
            T(int x, int y, int z) : m_X{x}, m_Y{y}, m_Z{z} {}  
        };        
    }
    {
        class T_11 {
            int m_X;
            int m_Y;
            int m_Z;
        public:
            T_11() : T_11{0, 0, 0} {} // T_11(int x, int y, int z)에 위임
            explicit T_11(int x) : T_11{x, 0, 0} {} // T_11(int x, int y, int z)에 위임
            T_11(int x, int y) : T_11{x, y, 0} {} // T_11(int x, int y, int z)에 위임
            T_11(int x, int y, int z) : m_X{x}, m_Y{y}, m_Z{z} {} 
        };
    }
    // 생성자 상속
    {
        class Base {
            int m_X;
            int m_Y;
        public:
            Base(int x, int y) : m_X{x}, m_Y{y} {}
        };

        class Derived : public Base {
        public:
            // Derived 생성자에서 Base(x, y) 생성자를 명시적으로 호출
            Derived(int x, int y) : Base{x, y} {}
        };

        Derived d(10, 20);
    }
    {
        class Base {
            int m_X;
            int m_Y;
        public:
            Base(int x, int y) : m_X{x}, m_Y{y} {}
        };

        class Derived_11 : public Base {
        public:
            // 생성자 상속
            using Base::Base; 
        };

        Derived_11 d{10, 20};    
    }    
}