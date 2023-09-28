#include "gtest/gtest.h" 

TEST(TestMordern, String) {
    {
        // C++11
        std::u16string str1{u"한글"};
        std::u32string str2{U"한글"};

        // C++20
        // std::u8string str3{u8"한글"};

        EXPECT_TRUE(str1.size() == 2);
        EXPECT_TRUE(str1[1] == u'글'); 

        EXPECT_TRUE(str2.size() == 2);
        EXPECT_TRUE(str2[1] == U'글');
    
        // C++20
        // EXPECT_TRUE(str3.size() == 2);
        // EXPECT_TRUE(str3[1] == U'글');
    }
    {
        std::cout<<"abc\r\ndef"<<std::endl;       

        // 이스케이프 문자와 개행을 소스코드에 기재된 그대로 출력합니다.
        std::cout<<R"(abc\r\n
def)"
        <<std::endl;

        // 임의 구분자 aaa 사용
        std::cout<<R"aaa(abc"()"
def)aaa"
        <<std::endl;
    }
}





