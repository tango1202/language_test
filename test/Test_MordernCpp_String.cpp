#include "gtest/gtest.h" 
#include <charconv>

namespace StringView_1 {
    std::size_t Func(const std::string& str) {return str.length();}    
}
namespace StringView_2 {
    std::size_t Func(const char* str) {
        std::size_t i{0};
        while(str[i] != '\0') {
            ++i;
        } 
        return i;
    }  
}
namespace StringView_3 {
    std::size_t Func(std::string_view sv) {
        return sv.length();
    }    
}

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

TEST(TestMordern, StringView) {
    // C++17 StringView
    {
        using namespace StringView_1;

        std::string str1{"Hello"};
        const char* str2{"World"};

        EXPECT_TRUE(Func(str1) == 5);
        EXPECT_TRUE(Func(str2) == 5); // (△) 비권장. 암시적으로 string 개체가 생성됩니다.
    }
    {
        using namespace StringView_2; 
        std::string str1{"Hello"};
        const char* str2{"World"};

        EXPECT_TRUE(Func(str1.c_str()) == 5); // (△) 비권장. 널종료 문자열을 만듭니다.
        EXPECT_TRUE(Func(str2) == 5); // (△) 비권장. 문자열의 길이는 매번 다시 '\0'까지 카운팅해야 합니다.          
    }
    {
        using namespace StringView_3;

        std::string str1{"Hello"};
        const char* str2{"World"};

        EXPECT_TRUE(Func(str1) == 5); // (O) 불필요하게 string 개체를 생성하지 않습니다.
        EXPECT_TRUE(Func(str2) == 5); // (O) 불필요하게 string 개체를 생성하지 않습니다.
    }
}

TEST(TestMordern, Convert) {
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
}



