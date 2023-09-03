#include "gtest/gtest.h" 


TEST(TestCppPattern, DefaultDeleteOverrideFinal) {
    // default와 delete
    {
        class T {
        public:
            T() = default; // 암시적 버전의 기본 생성자 사용
            T(const T&) = delete; // 암시적 버전의 복사 생성자 막음    
        };
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
        class Derived : public Base {
            virtual void Func1() override {}; // (O)
            virtual void Func2() override {}; // (O)
            // virtual void Func_2() override {}; // (X) 컴파일 오류. 부모 개체에 해당 멤버 없음
        };        
    }
    // final
    {
        class Base final {
        };
        // (X) 컴파일 오류. Base는 상속할 수 없습니다.
        // class Derived : public Base {
        // };
    }
    {
        class Base {
        public:
            virtual void Func1() {};
            virtual void Func2() final {};
        };
        class Derived1 : public Base {
            virtual void Func1() override final {}; // (O) Base를 오버라이드하고, 자식 개체에서는 오버라이드 못하게 합니다.
            // virtual void Func2() override {}; // (X) 컴파일 오류. Func2는 오버라이드 할 수 없습니다.
        }; 
        class Derived2 : public Derived1 {
            // virtual void Func1() override final {}; // (X) 컴파일 오류. Func1은 오버라이드 할 수 없습니다.
        };                
    }
}