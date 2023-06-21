#include "gtest/gtest.h" 


namespace { 

}
TEST(TestClassicCpp, Conversions) {
    {
        // char, signed char, unsigned char, short 는 가능하면 int로 변환, 안되면 unsigned int
        int a = (char)'a';
        int b = (signed char)'a';
        int c = (unsigned char)'a';
        int d = (short)1;
    }
    {
        // wchar_t는 int, unsigned int, long, unsigned long 으로 변환
        int a = (wchar_t)L'a';
        unsigned int b = (wchar_t)L'a';

        long c = (wchar_t)L'a';
        unsigned long d = (wchar_t)L'a';
    }
    {
        // 열거형은 int, unsigned int, long, unsigned long 으로 변환
        enum Color {Black};
        int a = (Color)Black;
        unsigned int b = (Color)Black;
        long c = (Color)Black;
        unsigned d = (Color)Black;
    }
    {
        // bool 은 int(true : 1, false : 0) 로 변환
        int a = (bool)true;
    }
    {
        // float 은 double 로 변환
        double a = (float)1.0F;
    }
    {
        // none-const는 const로 변환. 반대는 컴파일 오류
        int a = 10;
        const int* b = &a;
        // int* c = b; // (X) 컴파일 오류. const는 none-const로 변환 안됨
    }
    {
        // 모든 포인터는 void*로 변환. NULL은 모든 포인터로 변환
        int a = 10;
        void* p = &a; // 모든 포인터는 void*로 변환됨
        p = NULL; // NULL 은 모든 포인터로 변환됨
    }
    {
        // 자식 개체는 부모 개체로 변환됨(Up Casting)
        class Base {};
        class Derived: public Base {};

        Derived d;
        Base& b = d;
    }
    {
        // 괄호를 이용한 형변환
        // (X) 비권장. 잘못 사용하여 데이터 절삭으로 고생할 수 있으니, 
        // 나중에 검색이라도 쉽게 되도록 `static_cast`를 사용하세요.
        float f = 10.F;
        int a = (int)f;
        int b = int(f);

        EXPECT_TRUE(a == 10 && b == 10);
    }
    {
        int a = 10;
        const int& r = a;

        // r이 상수성 계약으로 참조하는데 멋대로 바꿨습니다.
        // (X) 비권장. 코딩 계약상 금지입니다.
        const_cast<int&>(r) = 20; // 포인터와 참조자만 가능

        EXPECT_TRUE(a == 20); 
    }
    {
        // (X) 비권장. 실수를 정수로 명시적 변환. 그냥 내림, 반올림, 올림 함수를 쓰세요.
        int a = static_cast<int>(3.14F);
        EXPECT_TRUE(a == 3); 

        // 정수를 열거형 상수로 변환
        enum Color {Black = 0};
        Color b = static_cast<Color>(0);

        // void* 를 다른 포인터로 변환
        int c = 10;
        void* v = &c;
        int* p = static_cast<int*>(v);

    }
    {
        // 부모 개체를 자식 개체로 변환(Down Casting)
        class Base {};
        class Derived: public Base {}; 
        class Other {};

        Derived d;
        Base& b = d;
        // (O) static_cast는 상속관계가 있어야 함(런타임 검사를 수행하지 않음)
        Derived& downCasting = static_cast<Derived&>(b);
        
        // (X) 컴파일 오류. b는 Other와 아무런 상속관계가 없음
        // Other& other = static_cast<Other&>(b);
    }
    {
        class Base {};
        class Other {};
        Base b;
        // (X) 비권장. 상속관계가 없더라도 변환해 줍니다.
        Other& other = reinterpret_cast<Other&>(b);
        
        // (X) 컴파일 오류. 개체의 주소를 그냥 char에 담아버리지는 않습니다.
        // char a = reinterpret_cast<char>(&b);

        int i = 10;
        const int* p1 = &i;
        // (X) 컴파일 오류. 상수성은 지켜 줍니다.
        // int* p2 = reinterpret_cast<int*>(p1);
    }
    {
        class T {
        public:
            // int로 형변환 되면 10을 리턴합니다.
            operator int() const { return 10; }
            
            // char로 형변환 되면 1을 리턴합니다.
            operator char() const { return 1; }
        };

        T t;
        int i = t;
        char c = t;

        EXPECT_TRUE(i == 10);
        EXPECT_TRUE(c == 1);
    }
    {
        class T {
        public:
            // int로 변환합니다.
            int ToInt() const { return 10; }
            
            // char로 변환합니다.
            char ToChar() const { return 1; }
        };

        T t;
        int i = t.ToInt();
        char c = t.ToChar();

        EXPECT_TRUE(i == 10);
        EXPECT_TRUE(c == 1);
    }
    {
        class T {
        public:
            operator bool() { return true; }
        };

        T t;
        bool status = false;
        // (X) 비권장. 컴파일 오류가 나지 않는 비극
        // t가 bool로 변환되어 true가 되고, 
        // 암시적으로 true가 int로 변환되어 1이 되고 조건식이 참이 됨
        // 의도한 코드인지를 모르겠으나,
        //  이렇게 암시적인 상황을 활용해서 복잡하게 의도하는 건 코드 분석이나 코딩 계약에 좋지 않음
        if (0 < t) { 
            status = true;
        }
        EXPECT_TRUE(status == true);
    }
    {
        class Base {
        public:
            virtual ~Base() {} // dynamic_cast를 사용하려면 가상함수가 있어야 합니다.
        };
        class Derived: public Base {}; 
        class Other {};

        Derived d;
        Base* b = &d;
        // (O) static_cast는 상속관계가 있어야 함(런타임 검사를 수행하지 않음)
        Derived* downCasting = dynamic_cast<Derived*>(b);      
        EXPECT_TRUE(downCasting != NULL);

        Other* other = dynamic_cast<Other*>(b);
        EXPECT_TRUE(other == NULL);



    }
}
