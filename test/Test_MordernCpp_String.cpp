#include "gtest/gtest.h" 

TEST(TestMordern, String) {
    {
        // C++11
        std::u16string str1_11{u"한글"};
        std::u32string str2_11{U"한글"};

        // C++20
        // std::u8string str3_20{u8"한글"};

        EXPECT_TRUE(str1_11.size() == 2);
        EXPECT_TRUE(str1_11[1] == u'글'); 

        EXPECT_TRUE(str2_11.size() == 2);
        EXPECT_TRUE(str2_11[1] == U'글');
    
        // C++20
        // EXPECT_TRUE(str3_20.size() == 2);
        // EXPECT_TRUE(str3_20[1] == U'글');
    }
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
}





