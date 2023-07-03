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
    {  
        EXPECT_TRUE(MAKE_STRING_LITERAL(Hello World) == "Hello World"); // 문자열로 변경됨
        EXPECT_TRUE(g_Func() == 10); // g_Func 호출
    }
    // ----
    // undef
    // ----
    {
        #define PI 3.14
        EXPECT_TRUE(PI == 3.14); // (O)

        #undef PI // 제거됩니다.
        // EXPECT_TRUE(PI == 3.14); // (X) 컴파일 오류  
    }
    {
        #define MY_DEBUG // MY_DEBUG 정의 유무만 알면 되므로 꼭 대체 목록을 작성할 필요 없음

        int status = 0;
        #if defined(MY_DEBUG)
            status = 1;
        #else 
            status = 2;
        #endif

        EXPECT_TRUE(status == 1); // MY_DEBUG가 정의되어 1
    }
    // ----
    // __LINE__, __FILE__, #line
    // ----
    { 
        // Line Number:118 Filename:C:\Data\language_test\test\Test_ClassicCpp_Preprocessor.cpp
        std::cout<<"Line Number:"<<__LINE__<<" Filename:"<<__FILE__<<std::endl; 

#line 1234 "test.cpp" 
        std::cout<<"Line Number:"<<__LINE__<<" Filename:"<<__FILE__<<std::endl;  // Line Number:1234 Filename:test.cpp      
    }
    {
// #define OS_WIN
// #ifndef OS_WIN
//     #warning "Only Windows."
//     #error "Only Windows."
// #else
// #endif
    }
    // ----
    // pragma pack(push, 1)
    // ----
    {
        class T {
            char m_Char; // 1byte, 메모리 접근 편의를 위해 32bit(4byte) 단위로 할당(패딩). 3byte 빈공간이 생김 
            int m_Int; // 4byte
        };

        EXPECT_TRUE(sizeof(T) == 8); // char가 패딩됨
    } 
#pragma pack(push, 1) // 데이터 버스 크기를 1 바이트 단위로 설정      
    {
        class T {
            char m_Char; // 1byte 
            int m_Int; // 4byte
        };

        EXPECT_TRUE(sizeof(T) == 5); // 1 + 4 = 5byte
    }
#pragma pack(pop) // 데이터 버스 크기 설정 원복     
}


