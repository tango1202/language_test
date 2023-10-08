#include "gtest/gtest.h" 


TEST(TestMordern, DefaultDeleteOverrideFinal) {
    // default와 delete
    {
        class T_11 {
        public:
            T_11() = default; // 암시적 버전의 기본 생성자 사용
            T_11(const T_11&) = delete; // 암시적 버전의 복사 생성자 막음    
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
            // virtual void Func_2_11() override {}; // (X) 컴파일 오류. 부모 개체에 해당 멤버 없음
        };       
    }
    // final
    {
        class Base_11 final {
        };
        // (X) 컴파일 오류. Base는 상속할 수 없습니다.
        // class Derived : public Base_11 {
        // };
    }
    {
        class Base {
        public:
            virtual void Func1_11() {};
            virtual void Func2_11() final {};
        };
        class Derived1 : public Base {
            virtual void Func1_11() override final {}; // (O) Base를 오버라이드하고, 자식 개체에서는 오버라이드 못하게 합니다.
            // virtual void Func2_11() override {}; // (X) 컴파일 오류. Func2_11은 오버라이드 할 수 없습니다.
        }; 
        class Derived2 : public Derived1 {
            // virtual void Func1_11() override final {}; // (X) 컴파일 오류. Func1_11은 오버라이드 할 수 없습니다.
        };               
    }
}