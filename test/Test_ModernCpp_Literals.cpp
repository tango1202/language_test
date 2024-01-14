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
    std::string operator ""_forced_string(const char* str, size_t size) {
        std::cout << "String Size : " << size << std::endl; // 전달된 문자열의 길이입니다.(널문자는 포함되지 않습니다.)
        return std::string(str);
    }
}
namespace UserLiteral_1 {    
    int operator ""_a(const char* str, size_t size) {
        return 1;
    }
    int operator ""_a(const char* str) {
        return 2;
    }
}
namespace UserLiteral_2 {    
    int operator ""_a(const char* str) { // (△) 비권장. 우선 순위가 낮습니다. 다른 타입과 동일한 식별자를 사용하지 마세요.
        return 2;
    }
    int operator ""_a(long double val) {  
        return 3;
    }
}
TEST(TestModern, UserLiteral) {
    {
        EXPECT_TRUE(1.5_km == 1500.0_m);
        EXPECT_TRUE(1.5_m == 150.0_cm);
        EXPECT_TRUE(1.5_cm == 15.0_mm);
        EXPECT_TRUE(1.5_mm == 2._mm); // 반올림 하므로 2._mm와 동일합니다.

        // EXPECT_TRUE(1_km == 1000.0_m); // (X) 컴파일 오류. long double만 오버로딩 되었습니다.
        EXPECT_TRUE(1_mm == 1); // mm는 정수형도 오버로딩 되었습니다.    

        auto chars_11 = "test"; // const char*
        // EXPECT_TRUE(chars_11.size() == 4); // (X) 컴파일 오류. const char*이므로 size() 멤버 함수가 없습니다

        auto str_11 = "test"_forced_string; // std::string
        EXPECT_TRUE(str_11.size() == 4); // (O)

        EXPECT_TRUE("hello"_forced_string.size() == 5); // 임시 개체도 가능합니다.
    }
    {
        using namespace UserLiteral_1;
        EXPECT_TRUE("3.14"_a == 1); // int operator ""_a(const char* str, size_t size) 버전이 호출됩니다.
        EXPECT_TRUE(3.14_a == 2); // int operator ""_a(const char* str)  버전이 호출됩니다.
    }
    {
        using namespace UserLiteral_2;
        EXPECT_TRUE(3.14_a == 3); // int operator ""_a(long double val) 버전이 호출됩니다.
    }    
}







