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

        auto chars_11 = "test"; // const char*
        auto str_11 = "test"_forced_string; // std::string
        EXPECT_TRUE(str_11.size() == 4);
        EXPECT_TRUE("hello"_forced_string.size() == 5); // 임시 개체도 가능
    }
}







