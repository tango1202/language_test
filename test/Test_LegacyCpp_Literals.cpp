#include "gtest/gtest.h" 

TEST(TestLegacyCpp, Literals) {
    // ----
    // bool 과 숫자
    // ----
    {
        // bool 형
        bool a = true; // 참
        bool b = false; // 거짓

        // 정수형
        int c = 10; // 10진수.
        int d = 010; // 8진수.
        int e = 0x1a; // 16진수. x 또는 X. a~f 또는 A~F
        unsigned int f = 10U; // unsigned. u 또는 U
        unsigned long g = 10U;
        long h = 10L; // long. l 또는 L
        unsigned long i = 10UL; // unsigned long. u 또는 U, l 또는 L

        // 실수형
        double j = 3.14; // 고정 소수점
        double k = 3.14e10; // 부동 소수점 e 또는 E
        float l = 3.14F; // f 또는 F
        float m = 3.14e10F; 
        long double n = 3.14L; // l 또는 L
        long double o = 3.14e10L;
    }
    // ----
    // 문자 상수
    // ----
    {
        char a = 'A';
        wchar_t b = L'A'; // 와이드 문자 2byte 또는 4byte
    }
    // ----
    // 문자열 상수
    // ----
    {
        const char* str1 = "abc"; // 문자열 상수
        const wchar_t* str2 = L"abc"; // 와이드 문자열 상수

        char str3[] = "abc"; // {'a', `b`, 'c', '\0'};
        EXPECT_TRUE(str3[0] == 'a');
        EXPECT_TRUE(str3[1] == 'b');
        EXPECT_TRUE(str3[2] == 'c');
        EXPECT_TRUE(str3[3] == '\0'); // 널문자가 추가됨

        wchar_t str4[] = L"abc"; // {L'a', L`b`, L'c', L'\0'};
        EXPECT_TRUE(str4[0] == L'a');
        EXPECT_TRUE(str4[1] == L'b');
        EXPECT_TRUE(str4[2] == L'c');
        EXPECT_TRUE(str4[3] == L'\0'); // 널문자가 추가됨
    }
    {
        const char* str1 = "Hello World"; 
        const char* str2 = "Hello " "World"; // (O) 두 문자열 상수 이어 붙이기
        const char* str3 = "Hello " // (O) 개행된 문자열 상수 붙이기
                           "World"; 
        EXPECT_TRUE(str1 == str2 && str2 == str3); // (O) 동일한 문자열 상수는 컴파일러가 1개만 생성해 줍니다.   
    }
    // ----
    // 이스케이프 문자
    // ----
    {
        std::cout <<"mytest : HelloWorld" << std::endl; 
        std::cout <<"H\relloWorld" << std::endl;    
        std::cout <<"H\nelloWorld" << std::endl;           
        // 0x25B3(△)
        std::cout << "\u25B3" << std::endl;

        // // 0x0103A0(𐎠)
        std::cout << "\U000103A0" << std::endl;
    }
}