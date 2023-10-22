#include "gtest/gtest.h" 

#include <cmath>
namespace {

    // mm 단위로 리턴
    int operator ""_km(long double val) {return std::round(val * 1000. * 1000.);}
    int operator ""_m(long double val) {return std::round(val * 1000.);}
    int operator ""_cm(long double val) {return std::round(val * 10.);}
    int operator ""_mm(long double val) {return std::round(val);}   
    int operator ""_mm(unsigned long long val) {return val;}  // 정수형 리터럴도 오버로딩

    // const char* 형의 문자열을 std::string으로 변환
    std::string operator ""_forced_string(const char* str, size_t) {return std::string(str);}
}
TEST(TestMordern, UserLiteral) {
    {
        EXPECT_TRUE(1.0_km == 1000.0_m);
        EXPECT_TRUE(1.0_m == 100.0_cm);
        EXPECT_TRUE(1.0_cm == 10.0_mm);
        EXPECT_TRUE(1.0_mm == 1_mm);

        auto str{"test"}; // const char*
        auto str_11{"test"_forced_string}; // std::string
        EXPECT_TRUE(str_11.size() == 4);
        EXPECT_TRUE("hello"_forced_string.size() == 5); // 임시 개체도 가능
    }
}
TEST(TestMordern, BineryLiteral) {
#if 201402L <= __cplusplus // C++14~        
    // C++14 이진 리터럴
    {
        int val1_14{0b11};
        int val2_14{0b1111};
        int val3_14{0B11111111};

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
        int val{1000000};
        int val_14{1'000'000};

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






