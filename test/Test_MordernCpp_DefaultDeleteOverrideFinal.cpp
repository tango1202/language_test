#include "gtest/gtest.h" 

namespace {

    // delete를 이용한 템플릿 인스턴스화 차단
    template<typename T> 
    void Func_11(T) {}

    template<> 
    void Func_11<char>(char) = delete; // char 버전 함수 삭제

}
TEST(TestMordern, DefaultDeleteOverrideFinal) {
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