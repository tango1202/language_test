#include "gtest/gtest.h" 

namespace {
    // 포인터를 인자로 사용하면, 널검사가 필요합니다.
    void f(int*p) {
        if (p == nullptr) {
            // p가 nullptr이면 뭘 해야 할까요?
        }
        *p = 20;
    }
    // 참조자를 인자로 사용하면, 널검사가 필요없습니다.
    void f(int& r) {
        r = 20;
    }

    // const int& GetX() { // 혹은 const int& GetX()도 마찬가지 입니다. 
    //     int x = 10; // 고맙게도 컴파일 경고는 나와줍니다.
    //     return x; 
    // } 

    void TestFunc(int) {}

    class Base {
    public:
        int m_Value;
        virtual void f() { std::cout<<"Base"<<std::endl; }    
    };
    class Derived : public Base {
    public:
        virtual void f() { std::cout<<"Derived"<<std::endl; }    
    };    
}

TEST(TestClassicCpp, PointerReference) {
    {
        int* p = nullptr;
        // int& r = *p; // (X) *p로 nullptr 의 개체를 구하는 건 오동작 할 수 있습니다. 
        // r = 10; // (X) 예외가 발생합니다. 이렇게 사용하시면 안됩니다.
    }
    {
        int x = 20;
        int* p = &x; // p는 x의 주소를 가집니다.
        *p = 30; // p가 가리키는 곳이 30이 됩니다.
        EXPECT_TRUE(*p == 30 && x == 30); 
    }
    {
        int x = 20;
        int& r = x; // r은 x의 별칭입니다.
        r = 30; // x는 30이 됩니다.
        EXPECT_TRUE(r == 30 && x == 30);
    }
    {
        int x = 10;
        f(&x);
        EXPECT_TRUE(x == 20);
    }
    {
        int x = 10;
        f(x);
        EXPECT_TRUE(x == 20);
    }
    {
        // int result = GetX(); // 예외 발생. 이미 소멸된 지역변수를 참조함
        // EXPECT_TRUE(result == 10);
    }
     // 개체 포인터
    {
        int obj = 10;
        int* p1 = &obj; // *p1 수정 가능. p1 수정 가능
        *p1 = 20;

        const int* p2 = &obj; // *p2 수정 불가. p2 수정 가능
        // *p2 = 20; // (X)
        p2 = p1;

        int* const p3 = &obj; // *p3 수정 가능. p3 수정 불가
        *p3 = 20;
        // p3 = p1; // (X)

        const int* const p4 = &obj; // // *p4 수정 불가. p4 수정 불가
        // *p4 = 20; // (X)
        // p4 = p1; // (X)
    }
    // 배열 포인터
    {
        int a[2] = {1, 2};

        // 배열 자체를 가리키는 포인터
        int(* p5)[2] = &a; // int[2]를 배열 가리키는 포인터 
        EXPECT_TRUE((*p5)[0] == 1 && (*p5)[1] == 2); // (*p5) 로 배열 개체 접근
    
        // 배열의 첫번째 요소를 가리키는 포인터
        int* p6 = a; // 혹은 int* p6 = &a[0]; 
        EXPECT_TRUE(*(p6 + 0) == 1 && *(p6 + 1) == 2);
        EXPECT_TRUE(p6[0] == 1 && p6[1] == 2);
    }
    // 함수 포인터
    {
        void (*p7)(int) = &TestFunc;
        void (*p8)(int) = TestFunc; // &TestFunc 랑 동일

        p7(10); // (*p6)(10); 도 가능. TestFunc 함수 호출 
        p8(10); // (*p7)(10); 도 가능. TestFunc 함수 호출        
    }
    // 다형성 포인터
    {
        Derived d;
        Base* p9 = &d; // 부모 클래스 포인터로 자식 클래스 제어
        p9->f(); // Derived 출력
    }
    // 멤버 변수 포인터 - 라이브러리 개발시 사용할 수도 있음
    {
        int Base::* p10 = &Base::m_Value; // Base 클래스 멤버 변수 m을 가리킴
        Base b;
        b.*p10 = 10;
        EXPECT_TRUE(b.m_Value == 10);
    }
    // 멤버 함수 포인터 - 라이브러리 개발시 사용할 수도 있음
    {
        void (Base::* p11)() = &Base::f; // Base 클래스 멤버 함수 f를 가리킴
        Base b;
        (b.*p11)(); // 멤버 함수 포인터는 괄호 필요. Base 출력
    }        
    // 개체 참조자
    {
        int obj = 10;
        int& r1 = obj; // r1 수정 가능
        r1 = 20;

        const int& r2 = obj; // const 형이어서 r2 수정 불가
        // r2 = 20; // (X)

        int& r3 = obj;
        // r3 = other; // (X) 참조자는 값 변경 불가
        // int& r4 = 20; // (X) T& 상수 참조 불가
        const int& r5 = 20; // (O) const T&로 상수 참조 가능
    }
    // 배열 참조자
    {
        int a[2];
        int(& r6)[2] = a; // 괄호로 감쌈
        r6[0] = 10;
        EXPECT_TRUE(a[0] == 10);
    }
    // 함수 참조자 
    {
        void (&r7)(int) = TestFunc;
        r7(10); // TestFunc 함수 호출 
    }
    // 다형성 참조자
    {
        Derived d;
        Base& r8 = d; // 부모 클래스 참조자로 자식 클래스 제어
        r8.f(); // Derived 출력
    } 
}