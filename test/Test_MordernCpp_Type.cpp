#include "gtest/gtest.h" 
#include <charconv>

namespace {
    int f(int) {return 1;} // #1
    int f(int*) {return 2;} // #2
}
TEST(TestMordern, Nullptr) {
    {
        int* ptr1 = 0;
        int* ptr2 = NULL;
        int* ptr3_11 = nullptr; // C++11

        EXPECT_TRUE(f(ptr1) == 2); // int* 이므로 f(int*) 호출
        EXPECT_TRUE(f(ptr2) == 2); // int* 이므로 f(int*) 호출
        EXPECT_TRUE(f(ptr3_11) == 2); // int* 이므로 f(int*) 호출
    }
    {
        // auto ptr1 = 0; // (△) 비권장. int
        // auto ptr2 = NULL; // (△) 비권장. long long
        auto ptr3_11 = nullptr; // nullptr_t

        // EXPECT_TRUE(f(ptr1) == 1); // f(int) 호출  
        // EXPECT_TRUE(f(ptr2) == 1); // f(int) 호출
        EXPECT_TRUE(f(ptr3_11) == 2); // f(int*) 호출
    }
}

TEST(TestMordern, Type) {
    // Raw String 리터럴
    {
        const char* str = "abc\r\ndef";
        std::cout << str << std::endl;    

        // 이스케이프 문자와 개행을 소스코드에 기재된 그대로 출력합니다.
        const char* str_11 = R"(abc\r\n
def)";     
        std::cout << str_11 << std::endl;
    }
    {
        // 임의 구분자 aaa 사용
        const char* str_11 = R"aaa(abc"()"
def)aaa";     
        std::cout << str_11 << std::endl;
    }    

    // char8_t
    {
#if 202002L <= __cplusplus // C++20~   
        
        // UTF-8 에서 영문 1글자는 1byte 입니다.
        char8_t en_20 = u8'a';

        // UTF-8에서 한글 1글자는 3byte입니다. 
        // 따라서 문자 1개를 1byte로는 저장할 수 없습니다.
        // char8_t kr_20 = u8'가'; // (X) 컴파일 오류

        char8_t arr_20[] = u8"가"; // 한글 1글자는 UTF-8로 3byte입니다.
        EXPECT_TRUE(sizeof(arr_20) == 4); // 널문자 포함하여 4byte입니다.
#endif        
    }

}

TEST(TestMordern, BineryLiteral) {
#if 201402L <= __cplusplus // C++14~        
    // C++14 이진 리터럴
    {
        int val1_14 = 0b11;
        int val2_14 = 0b1111;
        int val3_14 = 0B11111111;

        EXPECT_TRUE(val1_14 == 0x03);
        EXPECT_TRUE(val2_14 == 0x0F);
        EXPECT_TRUE(val3_14 == 0xFF);
    }
#endif    
}
TEST(TestMordern, DigitSeperator) {
#if 201402L <= __cplusplus // C++14~        
    // C++14 숫자 구분자
    {
        int val = 1000000;
        int val_14 = 1'000'000;

        EXPECT_TRUE(val == val_14);
    }
#endif   
}
TEST(TestMordern, HexFloatingPointLiteral) {
#if 201703L <= __cplusplus // C++17~    
    // (C++17~) 16진수 부동 소수점 리터럴
    {
        // A(10진수 10), 9(1/16 * 9 = 0.5625), (p11은 2의 11승 == 2048)
        // 즉, 10.5625 * 2 ^ 11 =  21632
        float floatVal_17 = 0xA.9p11; // 21632 
    }
#endif
}

