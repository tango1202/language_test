#include "gtest/gtest.h" 
#include <charconv>

TEST(TestMordern, String) {
    {
        // C++11
        std::u16string str1_11{u"abc가나다"};
        std::u32string str2_11{U"abc가나다"};

        // C++20
#if 202002L <= __cplusplus // C++20~         
        std::u8string str3_20{u8"abc가나다"};
#endif

        EXPECT_TRUE(str1_11.size() == 6);
        EXPECT_TRUE(str1_11[3] == u'가'); 

        EXPECT_TRUE(str2_11.size() == 6);
        EXPECT_TRUE(str2_11[3] == U'가');

        // C++20
#if 202002L <= __cplusplus // C++20~            
        EXPECT_TRUE(str3_20.size() == 12); // abc : 3byte, 가 : 3byte, 나 : 3byte, 다 : 3byte
        EXPECT_TRUE(str3_20[3] == 0xEA); // 한글 '가'는 UTF-8에서 3byte. 0xEA, 0xB0, 0x80
        EXPECT_TRUE(str3_20[4] == 0xB0);
        EXPECT_TRUE(str3_20[5] == 0x80);
#endif        
    }
}

TEST(TestMordern, Convert) {

#if 201703L <= __cplusplus // C++17~
    // C++17 숫자, 문자열 변환
    {
        char buf[10];
        {      
            // 11을 10진수 문자열로 변환    
            auto [ptr, ec]{std::to_chars(buf, buf + sizeof(buf), 11, 10)}; // 구조화된 바인딩. std::to_chars_result result{std::to_chars()} 와 동일
            if (ec == std::errc{}) {
                EXPECT_TRUE(std::string(buf, ptr - buf) == "11");
            }
        }
        {
            // 11을 16진수 문자열로 변환
            auto [ptr, ec]{std::to_chars(buf, buf + sizeof(buf), 11, 16)};
            EXPECT_TRUE(std::string(buf, ptr - buf) == "b");
        }
    }
    {
        char str[]{"11year"}; // 숫자와 일반 문자로 구성됩니다.
        int result{0};
        auto [ptr, ec]{std::from_chars(str, str + sizeof(str), result)};
        if (ec == std::errc{}) {
            EXPECT_TRUE(result == 11); // 숫자 부분만 잘 변환합니다.
        }
        EXPECT_TRUE(ptr == &str[2]); // year는 숫자가 아니므로 첫 문자 주소를 리턴합니다.
    }
#endif    
}



