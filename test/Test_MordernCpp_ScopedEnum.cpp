#include "gtest/gtest.h" 

namespace {
    enum MyInt : int {};
    void Func(int val) {}
    void Func_11(MyInt val) {}    
}

TEST(TestMordern, ScopedEnum) {

    {
        enum Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week{Sunday}; // (△) 비권장.  

    }
    {
        // 혹은
        class Week {
        public:
            // 클래스내에 정의. 사용시 클래스명을 기재해야 함
            enum Val {
                Sunday, Monday, Tuesday, Wednesday, 
                Thursday, Friday, Saturday
            };
        };

        Week::Val val{Week::Sunday}; // 범위 확인 연산자와 클래스명 사용       
    }
    {
        // 범위 있는 열거형
        enum class Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week{Week::Sunday}; // 범위명을 지정하여 이름 충돌 회피       
    }
    {
        enum MyEnum1 : int {a, b, c}; // int 형을 기반 타입으로 사용합니다.
        enum class MyEnum2 : char {i, j, k}; // char 형을 기반 타입으로 사용합니다.
    }
    {
        // 특정 값을 대입합니다. 반드시 MyInt 타입만 대입받을 수 있습니다.
        MyInt val1(MyInt(10));
        MyInt val2 = MyInt(10); // MyInt val1(MyInt(10)); 와 동일
        MyInt val3{val1}; 

        // MyInt val4 = 10; // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        // MyInt val5(10); // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        // MyInt val6{10}; // (X) 컴파일 오류. 중괄호 직접 초기화는 지원하지 않습니다.
        // MyInt val7 = {10}; // (X) 컴파일 오류. 중괄호 복사 초기화는 지원하지 않습니다.        
    }
    // 암시적 형변환 차단
    {
        Func(10);
        Func('c'); // (△) 비권장. 암시적 형변환 됩니다.

        // Func_11(10); // (△) 컴파일 오류
        // Func_11('c'); // (△) 컴파일 오류
        Func_11(MyInt(10)); // MyInt 타입만 가능합니다.       
    }

    // C++17 열거형의 중괄호 직접 초기화 허용
    {
        MyInt val1(MyInt(10));
        MyInt val2 = MyInt(10); // MyInt val1(MyInt(10)); 와 동일
        MyInt val3{val1}; 

        // MyInt val4 = 10; // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        // MyInt val5(10); // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        MyInt val6{10}; // (O) C++17~ 중괄호 직접 초기화를 지원합니다.
        // MyInt val7 = {10}; // (X) 컴파일 오류. 중괄호 복사 초기화는 지원하지 않습니다.    
    }

}
