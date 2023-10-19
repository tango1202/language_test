#include "gtest/gtest.h" 
#include <charconv>

namespace {
    size_t MyStrlen(const char * str) {
        size_t length = 0;
        while (*str != '\0') { // 널문자를 만날때까지 카운팅합니다.
            ++str;
            ++length;
        }
        return length;
    }    
}
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
        std::u16string str1_11{u"abc가나다"};
        std::u32string str2_11{U"abc가나다"};

        // C++20
        // std::u8string str3_20{u8"abc가나다"};

        EXPECT_TRUE(str1_11.size() == 6);
        EXPECT_TRUE(str1_11[3] == u'가'); 

        EXPECT_TRUE(str2_11.size() == 6);
        EXPECT_TRUE(str2_11[3] == U'가');

        // C++20
        // EXPECT_TRUE(str3_20.size() == 6);
        // EXPECT_TRUE(str3_20[3] == U'가');
    }
    // 널종료 문자열
    {
        EXPECT_TRUE(MyStrlen("abc") == 3);
    }
    {
        char str[] = "abc"; // (O) {'a', `b`, 'c', '\0'};
        EXPECT_TRUE(str[0] == 'a');
        EXPECT_TRUE(str[1] == 'b');
        EXPECT_TRUE(str[2] == 'c');
        EXPECT_TRUE(str[3] == '\0'); // 널문자가 추가됨
        EXPECT_TRUE(sizeof(str) == 4); // 널문자까지 포함하여 4byte 크기 입니다.
        EXPECT_TRUE(strlen(str)); // 문자열의 길이는 널문자를 제외한 3입니다.
    }
    // C++ 문자열
    {
        std::string str = "abc"; // 널종료 문자열로 생성할 수 있습니다.

        EXPECT_TRUE(strlen(str.data()) == 3); // (△) 비권장. 내부 데이터 메모리 블록을 리턴합니다. STL 구현에 따라 끝에 널문자가 있을 수도, 없을 수도 있습니다.
        EXPECT_TRUE(strlen(str.c_str()) == 3); // 널종료 문자열을 리턴하므로 안전합니다.
    }
    // 소스 코드와 인코딩
    {
        // UTF-8
        char* str = "가"; // UTF-8 가[0xEA 0xB0 0x80] 가 저장된 곳을 가리키는 포인터 입니다.

        EXPECT_TRUE(strlen(str) == 3); // UTF-8에서 한글 1글자는 3byte 입니다.
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 0) == 0xEA);
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 1) == 0xB0);
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 2) == 0x80);
    }
    {
        // euc-kr
        // char* str = "가"; // 완성형 가[0xB0 0xA1] 가 저장된 곳을 가리키는 포인터 입니다.

        // EXPECT_TRUE(strlen(str) == 2); // euc-kr에서 한글 1글자는 2byte 입니다.
        // EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 0) == 0xB0);
        // EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 1) == 0xA1);
    
    }
    // 바이트 문자열
    {
        char* str = "abc"; // 0x61 0x62 0x63 가 저장된 영역을 가리키는 포인터 입니다.

        EXPECT_TRUE(strlen(str) == 3); // UTF-8에서 영문 3글자는 3byte입니다.
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 0) == 0x61); // 0x61. 아스키 코드 a
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 1) == 0x62); // 0x62. 아스키 코드 b
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 2) == 0x63); // 0x63. 아스키 코드 c
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 3) == 0x00); // 널문자        
    }
    // 멀티 바이트 문자열
    {
        char* str = "abc가나다"; // abc[0x61 0x62 0x63] 가[0xEA 0xB0 0x80] 나[0xEB 0x82 0x98] 다[0xEB 0x8B 0xA4] 가 저장된 영역을 가리키는 포인터 입니다.
        EXPECT_TRUE(strlen(str) == 12); // UTF-8에서 한글 1글자는 12byte입니다. a(1) + b(1) + c(1) + 가(3) + 나(3) + 다(3) 1 + 1 + 1 + 3 + 3 + 3 = 12 

        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 0) == 0x61); // 0x61. 아스키 코드 a
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 1) == 0x62); // 0x62. 아스키 코드 b
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 2) == 0x63); // 0x63. 아스키 코드 c
  
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 3) == 0xEA); // 가
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 4) == 0xB0); 
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 5) == 0x80); 

        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 6) == 0xEB); // 나
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 7) == 0x82); 
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 8) == 0x98); 

        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 9) == 0xEB); // 다
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 10) == 0x8B); 
        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 11) == 0xA4); 

        EXPECT_TRUE(*reinterpret_cast<unsigned char*>(str + 12) == 0x00); // 널문자  

        std::setlocale(LC_ALL, "en_US.utf8");
        EXPECT_TRUE(mblen(str + 0, MB_CUR_MAX) == 1); // a 문자는 1byte 크기임
        EXPECT_TRUE(mblen(str + 1, MB_CUR_MAX) == 1); // b 문자는 1byte 크기임
        EXPECT_TRUE(mblen(str + 2, MB_CUR_MAX) == 1); // c 문자는 1byte 크기임
        EXPECT_TRUE(mblen(str + 3, MB_CUR_MAX) == 3); // 가 문자는 3byte 크기임
        EXPECT_TRUE(mblen(str + 6, MB_CUR_MAX) == 3); // 나 문자는 3byte 크기임
        EXPECT_TRUE(mblen(str + 9, MB_CUR_MAX) == 3); // 다 문자는 3byte 크기임

        wchar_t wstr[7];
        std::mbstowcs(wstr, str, 7); // UTF-8로 저장되어 있는 데이터를 디코딩하여 문자 1개씩 유니코드로 변경하여 저장합니다.

        EXPECT_TRUE(wstr[0] == 0x0061); // 0x0061. 아스키 코드 a
        EXPECT_TRUE(wstr[1] == 0x0062); // 0x0062. 아스키 코드 b
        EXPECT_TRUE(wstr[2] == 0x0063); // 0x0063. 아스키 코드 c
        EXPECT_TRUE(wstr[3] == 0xAC00); // 0xAC00. 유니코드 가
        EXPECT_TRUE(wstr[4] == 0xB098); // 0xB098. 유니코드 나
        EXPECT_TRUE(wstr[5] == 0xB2E4); // 0xB2E4. 유니코드 다
        EXPECT_TRUE(wstr[6] == 0x0000); // 널문자
    }
    // 와이드 문자열
    {
        wchar_t* wstr = L"abc가나다"; // abc[0x61 0x62 0x63] 가[0xAC00] 나[0xB098] 다[0xB2E4] 가 저장된 영역을 가리키는 포인터 입니다.
        EXPECT_TRUE(wcslen(wstr) == 6); 

        EXPECT_TRUE(wstr[0] == 0x0061); // 0x0061. 아스키 코드 a
        EXPECT_TRUE(wstr[1] == 0x0062); // 0x0062. 아스키 코드 b
        EXPECT_TRUE(wstr[2] == 0x0063); // 0x0063. 아스키 코드 c
        EXPECT_TRUE(wstr[3] == 0xAC00); // 0xAC00. 유니코드 가
        EXPECT_TRUE(wstr[4] == 0xB098); // 0xB098. 유니코드 나
        EXPECT_TRUE(wstr[5] == 0xB2E4); // 0xB2E4. 유니코드 다
        EXPECT_TRUE(wstr[6] == 0x0000); // 널문자        
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



