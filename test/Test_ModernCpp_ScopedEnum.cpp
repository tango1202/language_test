#include "gtest/gtest.h" 

namespace {
    enum MyInt_11 : int {};
    void Func(int val) {}
    void Func_11(MyInt_11 val) {}    
}

TEST(TestModern, ScopedEnum) {

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
        enum class Week_11 {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week_11 week{Week_11::Sunday}; // 열거형 이름을 같이 사용하여 이름 충돌 회피       
    }
    // 열거형의 암시적 형변환
    {

        enum Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week = Sunday;
        int val = week; // int형으로 형변환 됩니다.

        enum class Week_11 {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday    
        };

        Week_11 week_11 = Week_11::Sunday;
        // int val_11 = week_11; // (X) 컴파일 오류. 형변환되지 않습니다.
    }
    {

        enum Test {a_, b_, c_};
        EXPECT_TRUE(sizeof(Test) == sizeof(int));

        enum MyEnum1_11 : int {a, b, c}; // int 형을 기반 타입으로 사용합니다.
        enum class MyEnum2_11 : char {i, j, k}; // char 형을 기반 타입으로 사용합니다.
    }
    {
        // 특정 값을 대입합니다. 반드시 MyInt_11 타입만 대입받을 수 있습니다.
        MyInt_11 val1{MyInt_11(10)};
        MyInt_11 val2 = MyInt_11(10); // MyInt_11 val1{MyInt_11(10)}; 와 동일
        MyInt_11 val3{val1}; 

        // MyInt_11 val4 = 10; // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        // MyInt_11 val5(10); // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        // MyInt_11 val6{10}; // (X) 컴파일 오류. 중괄호 직접 초기화는 지원하지 않습니다.
        // MyInt_11 val7 = {10}; // (X) 컴파일 오류. 암시적으로 중괄호 직접 초기화를 사용하므로 지원하지 않습니다.
    }
    // 암시적 형변환 차단
    {
        Func(10);
        Func('c'); // (△) 비권장. 암시적 형변환 됩니다.

        // Func_11(10); // (X) 컴파일 오류. 암시적 형변환을 차단합니다.
        // Func_11('c'); // (X) 컴파일 오류. 암시적 형변환을 차단합니다.
        Func_11(MyInt_11(10)); // MyInt_11 타입만 가능합니다.       
    }
#if 201703L <= __cplusplus // C++17~
    // C++17 열거형의 중괄호 직접 초기화 허용
    {
        MyInt_11 val1(MyInt_11(10));
        MyInt_11 val2 = MyInt_11(10); // MyInt val1(MyInt(10)); 와 동일
        MyInt_11 val3{val1}; 

        // MyInt_11 val4 = 10; // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        // MyInt_11 val5(10); // (X) 컴파일 오류. 정수는 대입 받을 수 없습니다.
        MyInt_11 val6_17{10}; // (O) C++17~ 중괄호 직접 초기화를 지원합니다.
        // MyInt_11 val7_17 = {10}; // (X) 컴파일 오류. 중괄호 직접 초기화는 허용하지만, {10}은 int로 추론되어 사용할 수 없습니다.    
    }
#endif
    // using enum
    {
        enum class Week_11 {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week_11 week{Week_11::Sunday}; 
        bool isFreeDay{false};
        switch(week) {
        case Week_11::Sunday: isFreeDay = true; break;
        case Week_11::Monday: break;
        case Week_11::Tuesday: break;
        case Week_11::Wednesday: break;
        case Week_11::Thursday: break;
        case Week_11::Friday: break;
        case Week_11::Saturday: break;
        }        
    }
    {
#if 202002L <= __cplusplus // C++20~ 
        enum class Week_11 {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week_11 week{Week_11::Sunday}; // 범위명을 지정하여 이름 충돌 회피
        bool isFreeDay{false};
        switch(week) {
            
        using enum Week_11; // (C++20~) 유효 범위 내에서 Week_11의 열거자를 사용할 수 있습니다.
        case Sunday: isFreeDay = true; break;
        case Monday: break;
        case Tuesday: break;
        case Wednesday: break;
        case Thursday: break;
        case Friday: break;
        case Saturday: break;
        }
#endif
    }

}
