#include "gtest/gtest.h" 
#include <typeinfo>

namespace {

    // ----
    // define 상수
    // ----
    #define SUNDAY 0 // (△) 비권장. enum Val {Sunday, Monday, Tuesday}; 가 낫다
    #define MONDAY 1
    #define TUESDAY 2

    #define PI 3.14 // (△) 비권장. const double g_Pi = 3.14;가 낫다.
    #define WELCOM "Hello World" // (△) 비권장. const char* g_Welcome = "Hello World";가 낫다.
    #define LOGICAL int // (△) 비권장. typedef int Logical; 이 낫다.

    class Week {
    public:
         enum Val { // (O) 매크로로 치환된 정수가 아닌 Week::Val 타입으로 코딩계약을 할 수 있습니다.
            Sunday, 
            Monday, 
            Tuesday,
        };
    };
    const double g_Pi = 3.14; // (O) 타입에 기반한 코딩 계약이 가능합니다.
    const char* g_Welcome = "Hello World"; // (O) 타입에 기반한 코딩 계약이 가능합니다.
    typedef int Logical; // (O) 타입에 기반한 코딩 계약이 가능합니다.

    inline int Square(int x) { return x * x;} // (O) 인자 타입에 따른 코딩 계약, 디버깅 용이

    // 개행을 사용하여 f 함수 매크로문 정의
    #define LONGFUNC int f(int x) { \
        return x + x;               \
    }

    // f 함수를 정의함
    LONGFUNC;   

    // #과 ##
    #define MAKE_STRING_LITERAL(x) #x
   
    #define MAKE_FUNCTION(prefix, functionName, a) int prefix##functionName() {return a;}
    MAKE_FUNCTION(g_, Func, 10); // g_Func 이름의 함수를 정의함
}

TEST(TestClassicCpp, Preprocessor) {
    // ----
    // define 상수
    // ----
    {
        int week = MONDAY;
        EXPECT_TRUE(week == MONDAY);  
        EXPECT_TRUE(PI == 3.14);   
        EXPECT_TRUE(WELCOM == "Hello World"); 
        EXPECT_TRUE(typeid(LOGICAL) == typeid(int)); 
    }
    {
        Week::Val week = Week::Monday;
        EXPECT_TRUE(week == Week::Monday);
        EXPECT_TRUE(g_Pi == 3.14);   
        EXPECT_TRUE(g_Welcome == "Hello World"); 
        EXPECT_TRUE(typeid(Logical) == typeid(int)); 
    }
    // 긴 define
    {
        int val = f(3);
        EXPECT_TRUE(val == 6);
    }

    // ----
    // define 함수
    // ----
    {
        #define SQUARE(x) x * x // (△) 비권장. 인자와 코딩 계약이 안됩니다.

        EXPECT_TRUE(SQUARE(2) == 4); // 2 * 2
        EXPECT_TRUE(SQUARE(1 + 1) == 3); // 1 + 1 * 1 + 1
    }
    {
        #undef SQUARE
        #define SQUARE(x) (x) * (x) // (△) 비권장. 인자와 코딩 계약이 안됩니다.
        EXPECT_TRUE(SQUARE(1 + 1) == 4); // (1 + 1) * (1 + 1)
    }
    // ----
    // define 함수 # 과 ##
    // ----   
    EXPECT_TRUE(MAKE_STRING_LITERAL(Hello World) == "Hello World"); // 매크로 함수 결과 문자열로 변경됨
    EXPECT_TRUE(g_Func() == 10); // 매크로 함수 결과 g_Func 이라는 이름으로 합성됨
}

