#include "gtest/gtest.h" 

TEST(TestClassicCpp, Literals) {
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
        const char* str1 = "abc"; // 문자열 상수
        char* temp = const_cast<char*>(str1);
        // (X) 예외 발생. 문자열 상수는 rodata에 있기 때문에 수정할 수 없습니다.
        // *temp = 'd';
       
        char str2[] = "abc"; // {'a', `b`, 'c', '\0'};
        // (O) 배열은 문자열 상수의 복제본이어서 항목을 수정할 수 있습니다.
        str2[0] = 'd';
        EXPECT_TRUE(str2[0] == 'd');
    }
    // ----
    // 이스케이프 문자
    // ----
    {
        // 0x25B3(△)
        std::cout<<"\u25B3"<<std::endl;

        // 0x0103A0(𐎠)
        std::cout<<"\U000103A0"<<std::endl;
    }
}