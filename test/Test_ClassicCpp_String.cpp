#include "gtest/gtest.h" 

#include <algorithm>

TEST(TestClassicCpp, String) {
    {
        std::string s1("OK"); // 문자열 상수는 새로운 메모리 공간에 복제되어 저장됩니다.
        std::string s2 = "OK"; // s2("OK") 와 동일합니다.
        std::string s3 = s1; // s3(s)로 복사 생성자를 호출합니다. 구현에 따라 지연 복사되는 버전도 있습니다. 
        std::string s4; // 빈 string 개체를 생성합니다. 
        std::string s5 = ""; // 빈 문자열을 생성합니다.
        std::string s6(s1, 0, 2); // s1[0]부터 2개 문자를 복사하여 생성합니다.
        // std::string s6(s1, 100, 2); // (X) 예외 발생. s1[99]부터 2개 복사. out_of_range
        std::string s7(s1, 0, std::string::npos); // s[0] 부터 끝까지 복사하여 생성합니다.
        std::string s8(3, 'a'); // 'a'문자를 3개 채웁니다.
        EXPECT_TRUE(s8 == "aaa");
        const char* ptr = "abcdefg";
        std::string s9(ptr, ptr + 3); // ptr + 3 은 d 위치임
        EXPECT_TRUE(s9 == "abc"); // d 위치 직전까지 복사하여 생성합니다.

        s1.c_str(); // (O) c 스타일의 문자열을 리턴합니다.(끝에 널문자가 있습니다.)
        s1.data(); // (△) 비권장. 내부 데이터를 리턴합니다. 끝에 널문자가 없을 수도 있습니다.
        EXPECT_TRUE(s1 == "OK"); // (O) 단순 포인터 비교가 아니라 실제 문자들을 비교합니다.

        s1.length() == 2; // s2.size() 와 같습니다.
        s1[0] = 'a'; // [] 로 문자에 접근할 수 있습니다.
    }
}
