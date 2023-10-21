#include "gtest/gtest.h" 

namespace {

    // ----
    // 전방 선언
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
    // class T; // (O)
    // class T::Nested; // (X) 컴파일 오류. 중첩 클래스는 전방 선언을 할 수 없습니다.
    // class T {
    // public:
    //     class Nested {};
    // };
 
}

TEST(TestClassicCpp, Include) {

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

