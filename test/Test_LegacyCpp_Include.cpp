#include "gtest/gtest.h" 

namespace Forward_1 {
    // ----
    // 전방 선언
    // ----
    class MyClass; // 전방 선언

    class A {
        MyClass* m_MyClass; // MyClass 선언을 include 하지 않고 전방 선언만 해도 됩니다.
    public:
        A();
        ~A();
        int f() const;
    };


    class MyClass {
    private:
        int m_Val;
    public:
        int Func() const; // Func() 멤버 함수 선언   
    };

    A::A() : m_MyClass(new MyClass()) {} // 생성합니다.
    A::~A() {delete m_MyClass;} // 소멸합니다.
    int A::f() const {return m_MyClass->Func();} // MyClass를 사용합니다.

    // Func 함수 정의 
    int MyClass::Func() const {
        return m_Val;
    }
}
namespace Forward_2 {


    // ----
    // 전방 선언 상호 참조
    // ----
    // 1. YourClass 전방 선언
    class YourClass; 

    // 2. MyClass 선언
    class MyClass {
        // (O) 전방 선언을 통해 YourClass가 대충 클래스라는 걸 압니다. 
        // 반드시 포인터나 참조자와 같은 참조 형식이어야 합니다.
        YourClass* m_Your; 

        // YourClass의 구체 정의가 필요하여 선언만 합니다.
        void f(); 
    };

    // 3. YourClass 선언
    class YourClass {
        // MyClass는 상위에 정의되어 사용할 수 있습니다.
        MyClass m_My; 

    public:
        void g() {}
    }; 

    // 4. MyClass 정의 - YourClass를 사용하고 있어 YourClass 선언 후 작성합니다.
    void MyClass::f() {
        m_Your->g(); 
    } 
    // 중첩 클래스의 전방 선언
    class T; // (O)
    // class T::Nested; // (X) 컴파일 오류. 중첩 클래스는 클래스 바깥에서 전방 선언을 할 수 없습니다.

    class T {
    public:
        class Nested; // (O) 클래스 내에서 전방 선언 해야 합니다.
        class Nested {};
    };
 
}

TEST(TestLegacyCpp, Include) {

    // 중첩 클래스의 전방 선언
    {
        class T {
        private:
            class Nested; // 전방 선언을 private로 했습니다.
        private:
            class Nested {}; // (O)
        public:
            // class Nested {}; // (X) 컴파일 오류. 전방 선언에선 private이고 실제 선언은 public 입니다.
        };  
    }
}

