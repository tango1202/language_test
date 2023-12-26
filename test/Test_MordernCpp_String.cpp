#include "gtest/gtest.h" 
#include <charconv>

TEST(TestMordern, String) {
    // 개요
    {
        std::u16string u16str{u"abc가나다"};
        EXPECT_TRUE(u16str == u"abc가나다");
        EXPECT_TRUE(u16str.length() == 6);
        EXPECT_TRUE(u16str[3] == u'가'); // 각 요소는 문자 1개 입니다.
        
        std::u32string u32str{U"abc가나다"};  
        EXPECT_TRUE(u32str == U"abc가나다");
        EXPECT_TRUE(u32str.length() == 6);
        EXPECT_TRUE(u32str[3] == U'가'); // 각 요소는 문자 1개 입니다.        
    }
    {
        std::wstring wstr{L"abc가나다"}; // windows에서는 와이드 문자열이 UTF-16으로 저장됩니다.
        std::u16string u16str{reinterpret_cast<const char16_t*>(wstr.c_str())}; // 강제적으로 형변환하면 UTF-16과 동일한 형태로 저장되어 있습니다.
        EXPECT_TRUE(u16str == u"abc가나다");
    }
    {
#if 202002L <= __cplusplus // C++20~    
        std::u8string u8str{u8"abc가나다"}; 
        EXPECT_TRUE(u8str == u8"abc가나다"); 
        EXPECT_TRUE(u8str.length() == 12); // UTF8에서 영문자는 1byte, 한글은 3byte입니다. 한글 1글자를 3개로 처리합니다.
                                           // abc : 3byte, 가 : 3byte, 나 : 3byte, 다 : 3byte
        EXPECT_TRUE(u8str[3] == 0xEA); // 한글 '가'는 UTF-8에서 3byte. 0xEA, 0xB0, 0x80
        EXPECT_TRUE(u8str[4] == 0xB0);
        EXPECT_TRUE(u8str[5] == 0x80);
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
        EXPECT_TRUE(ptr == &str[2]); // ptr은 숫자 다음 위치입니다. 즉, 'y' 위치입니다.
    }
#endif   


}



