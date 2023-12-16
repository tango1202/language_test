#include "gtest/gtest.h" 

namespace {
    // ----
    // 널검사가 필요없는 참조자
    // ----
    // 포인터를 인자로 사용하면, 널검사가 필요합니다.
    void f(int* p) {
        if (p == NULL) {
            // (△) 비권장. p가 NULL이면 뭘 해야 할까요?
        }
        *p = 20;
    }
    // 참조자를 인자로 사용하면, 널검사가 필요없습니다.
    void f(int& r) {
        r = 20;
    }
    // ----
    // Dangling 참조자
    // ----
    // const int& GetX() { 
    //     int x = 10; // (X) 컴파일 경고. 고맙게도 컴파일 경고는 나와줍니다.
    //     return x; 
    // } 

    // ----
    // 포인터 사용법 - 함수 포인터
    // ---- 
    void TestFunc(int) {}

    // ----
    // 포인터 사용법 - 다형성 포인터
    // ---- 
    class Base {
    public:
        int m_Value;
        virtual void f() {std::cout << "Base" << std::endl;}    
    };
    class Derived : public Base {
    public:
        virtual void f() {std::cout << "Derived" << std::endl;}    
    };    
}

TEST(TestLegacyCpp, PointerReference) {
    // ----
    // 포인터 개요
    // ----
    {
        int x = 20;
        int* p = &x; // p는 x의 주소를 가집니다.
        *p = 30; // p가 가리키는 곳(x)의 실제값이 30이 됩니다.
        EXPECT_TRUE(*p == 30 && x == 30); 
    }
    // ----
    // 참조자 개요
    // ----    
    {
        int x = 20;
        int& r = x; // r은 x의 별칭입니다.
        r = 30; // r이 참조하는 곳(x)의 실제값이 30이 됩니다.
        EXPECT_TRUE(r == 30 && x == 30);
    }
    // ----
    // 안전한 참조자
    // ----       
    {
        // int& r; // (X) 컴파일 오류. 생성만 할 수 없습니다. 초기값을 넣어주어야 합니다. 쓰레기값을 가질 일이 없습니다.

        int x = 10;
        int y = 20;

        int& r = x;
        r = y; // (O) r 이 y를 참조하는 것이 아니라, y의 값인 20을 대입 받습니다. r이 참조하는 x가 20으로 수정됩니다.
        y = 30;
        EXPECT_TRUE(x == 20 && r == 20 && y == 30); // r은 x를 참조하기 때문에 y를 30으로 수정한 것과 상관없이 x와 같은 20 입니다    }
    }
    // ----
    // Dangling 참조자
    // ----
    {
        // (X) 예외 발생. 이미 소멸된 지역 변수를 참조함
        // int& result = GetX(); 

        // (△) 비권장. 사용할 수는 있으나 GetX()에서 지역 변수 참조 리턴 warning 발생
        // const int& result = GetX(); 

        int* p = NULL;
        // int& r = *p; // (X) 오동작. *p로 NULL 의 개체를 구하는 건 오동작 할 수 있습니다. 
        // r = 10; // (X) 예외 발생. p가 NULL이라 10을 NULL(0)주소 위치에 넣다가 예외 발생 합니다. 이렇게 사용하시면 안됩니다.
    }
    // ----
    // 포인터 사용법 - 개체 포인터
    // ----   
    {
        int obj = 10;
        int* p1 = &obj; // *p1 수정 가능. p1 수정 가능
        *p1 = 20;

        const int* p2 = &obj; // *p2 수정 불가. p2 수정 가능
        // *p2 = 20; // (X) 컴파일 오류
        p2 = p1;

        int* const p3 = &obj; // *p3 수정 가능. p3 수정 불가
        *p3 = 20;
        // p3 = p1; // (X) 컴파일 오류

        const int* const p4 = &obj; // *p4 수정 불가. p4 수정 불가
        // *p4 = 20; // (X) 컴파일 오류
        // p4 = p1; // (X) 컴파일 오류
    }
    // ----
    // 포인터 사용법 - 배열 포인터
    // ----  
    {
        int a[2] = {1, 2};

        // 배열 자체를 가리키는 포인터
        int(* p5)[2] = &a; // int[2]인 배열을 가리키는 포인터 
        EXPECT_TRUE((*p5)[0] == 1 && (*p5)[1] == 2); // (*p5) 로 배열 개체 접근
    
        // 배열의 첫번째 요소를 가리키는 포인터
        int* p6 = a; // 혹은 int* p6 = &a[0]; 
        EXPECT_TRUE(*(p6 + 0) == 1 && *(p6 + 1) == 2);
        EXPECT_TRUE(p6[0] == 1 && p6[1] == 2);
    }
    // ----
    // 포인터 사용법 - 함수 포인터
    // ---- 
    {
        void (*p7)(int) = &TestFunc;
        void (*p8)(int) = TestFunc; // &TestFunc 와 동일

        p7(10); // (*p7)(10); 도 가능. TestFunc 함수 호출 
        p8(10); // (*p8)(10); 도 가능. TestFunc 함수 호출        
    }
    // ----
    // 포인터 사용법 - 다형성 포인터
    // ---- 
    {
        Derived d;
        Base* p9 = &d; // 부모 클래스 포인터로 자식 클래스 제어
        p9->f(); // Derived 출력
    }
    // ----
    // 포인터 사용법 - 멤버 변수 포인터 - 라이브러리 개발시 사용할 수도 있음
    // ----
    {
        int Base::* p10 = &Base::m_Value; // Base 클래스 멤버 변수 m_Value를 가리킴
        Base b;
        b.*p10 = 10;
        EXPECT_TRUE(b.m_Value == 10);
    }
    // ----
    // 포인터 사용법 - 멤버 함수 포인터 - 라이브러리 개발시 사용할 수도 있음
    // ----
    {
        void (Base::* p11)() = &Base::f; // Base 클래스 멤버 함수 f를 가리킴
        Base b;
        (b.*p11)(); // 멤버 함수 포인터는 괄호 필요. Base 출력
    } 
    // 다차원 포인터
    {
        int val = 10;
        int* p = &val; // val 의 포인터
        int** pp = &p; // p의 포인터. 즉 val의 포인터의 포인터

        EXPECT_TRUE(*p == 10);

        EXPECT_TRUE(*pp == p); // pp는 p를 가리킵니다.
        EXPECT_TRUE(**pp == 10); // *(*pp) == *(p) 이므로 **pp는 p가 가리키는 val 입니다.
    }
    // ----
    // 참조자 사용법 - 개체 참조자
    // ----
    {
        int obj = 10;
        int& r1 = obj; // r1으로 obj 수정 가능
        r1 = 20;

        const int& r2 = obj; // const 형이어서 r2로 obj 수정 불가
        // r2 = 20; // (X) 컴파일 오류
        
        // int& r3 = 20; // (X) 컴파일 오류. T&로 상수 참조 불가
        const int& r4 = 20; // (O) const T&로 상수 참조 가능

    }
    // ----
    // 참조자 사용법 - 배열 참조자
    // ----
    {
        int a[2];
        int(&r5)[2] = a; // 괄호로 감쌈
        r5[0] = 10;
        EXPECT_TRUE(a[0] == 10);
    }
    // ----
    // 참조자 사용법 - 함수 참조자 
    // ----
    {
        void (&r6)(int) = TestFunc;
        r6(10); // TestFunc 함수 호출 
    }
    // ----
    // 참조자 사용법 - 다형성 참조자
    // ----
    {
        Derived d;
        Base& r7 = d; // 부모 클래스 참조자로 자식 클래스 제어
        r7.f(); // Derived 출력
    } 
}
