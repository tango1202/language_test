#include "gtest/gtest.h" 

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

TEST(TestLegacyCpp, String) {
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
        EXPECT_TRUE(sizeof(str) / sizeof(char) == 4); // 배열 갯수는 널문자를 포함하여 4 입니다.
        EXPECT_TRUE(strlen(str) == 3); // 문자열의 길이는 널문자를 제외한 3입니다.
    }
    // 문자열 상수 수정
    {
        const char* str1 = "abc"; // 문자열 상수
        char* temp = const_cast<char*>(str1);
        // (X) 예외 발생. 문자열 상수는 rodata에 있기 때문에 수정할 수 없습니다.
        // *temp = 'd';
       
        char str2[] = "abc"; // {'a', `b`, 'c', '\0'};
        // (O) 배열은 문자열 상수의 복제본이어서 항목을 수정할 수 있습니다.
        str2[0] = 'd';
        EXPECT_TRUE(str2[0] == 'd');
    }
    // 소스 코드와 인코딩
    {
        // UTF-8
        const char* str = "가"; // UTF-8 가[0xEA 0xB0 0x80] 가 저장된 곳을 가리키는 포인터 입니다.

        EXPECT_TRUE(strlen(str) == 3); // UTF-8에서 한글 1글자는 3byte 입니다.
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 0) == 0xEA);
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 1) == 0xB0);
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 2) == 0x80);
    }
    {
        // euc-kr
        // const char* str = "가"; // 완성형 가[0xB0 0xA1] 가 저장된 곳을 가리키는 포인터 입니다.

        // EXPECT_TRUE(strlen(str) == 2); // euc-kr에서 한글 1글자는 2byte 입니다.
        // EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 0) == 0xB0);
        // EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 1) == 0xA1);
    
    }
    // 바이트 문자열
    {
        const char* str = "abc"; // 0x61 0x62 0x63 가 저장된 영역을 가리키는 포인터 입니다.

        EXPECT_TRUE(strlen(str) == 3); // UTF-8에서 영문 3글자는 3byte입니다.
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 0) == 0x61); // 0x61. 아스키 코드 a
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 1) == 0x62); // 0x62. 아스키 코드 b
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 2) == 0x63); // 0x63. 아스키 코드 c
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 3) == 0x00); // 널문자        
    }
    // 멀티 바이트 문자열
    {
        const char* str = "abc가나다"; // abc[0x61 0x62 0x63] 가[0xEA 0xB0 0x80] 나[0xEB 0x82 0x98] 다[0xEB 0x8B 0xA4] 가 저장된 영역을 가리키는 포인터 입니다.
        EXPECT_TRUE(strlen(str) == 12); // UTF-8에서 한글 1글자는 12byte입니다. a(1) + b(1) + c(1) + 가(3) + 나(3) + 다(3) 1 + 1 + 1 + 3 + 3 + 3 = 12 

        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 0) == 0x61); // 0x61. 아스키 코드 a
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 1) == 0x62); // 0x62. 아스키 코드 b
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 2) == 0x63); // 0x63. 아스키 코드 c
  
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 3) == 0xEA); // 가
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 4) == 0xB0); 
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 5) == 0x80); 

        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 6) == 0xEB); // 나
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 7) == 0x82); 
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 8) == 0x98); 

        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 9) == 0xEB); // 다
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 10) == 0x8B); 
        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 11) == 0xA4); 

        EXPECT_TRUE(*reinterpret_cast<const unsigned char*>(str + 12) == 0x00); // 널문자  

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
        const wchar_t* wstr = L"abc가나다"; // Windows는 UTF-16. abc[0x61 0x62 0x63] 가[0xAC00] 나[0xB098] 다[0xB2E4] 가 저장된 영역을 가리키는 포인터 입니다.
        EXPECT_TRUE(wcslen(wstr) == 6); // Windows에서는 2byte 단위로 저장합니다.

        EXPECT_TRUE(wstr[0] == 0x0061); // 0x0061. UTF-16 인코딩. 아스키 코드 a
        EXPECT_TRUE(wstr[1] == 0x0062); // 0x0062. UTF-16 인코딩. 아스키 코드 b
        EXPECT_TRUE(wstr[2] == 0x0063); // 0x0063. UTF-16 인코딩. 아스키 코드 c
        EXPECT_TRUE(wstr[3] == 0xAC00); // 0xAC00. UTF-16 인코딩. 유니코드 가
        EXPECT_TRUE(wstr[4] == 0xB098); // 0xB098. UTF-16 인코딩. 유니코드 나
        EXPECT_TRUE(wstr[5] == 0xB2E4); // 0xB2E4. UTF-16 인코딩. 유니코드 다
        EXPECT_TRUE(wstr[6] == 0x0000); // 널문자  
    }
    // 개요
    {
        std::string str = "abc";
        EXPECT_TRUE(str.length() == 3);

        std::wstring wstr = L"abc가나다"; // 와이드 문자를 사용합니다.
        EXPECT_TRUE(wstr.length() == 6);
        EXPECT_TRUE(wstr[3] == L'가');        
    }
    {
        std::string s("OK");
        EXPECT_TRUE(*(&s[0] + 0) == 'O');
        EXPECT_TRUE(*(&s[0] + 1) == 'K'); // 연속된 메모리를 사용합니다.
    }
    
    {
        std::string s("abc");
        
        EXPECT_TRUE(s[0] == 'a' && s[1] == 'b'&& s[2] == 'c'); // []로 접근할 수 있습니다.  

        std::string::iterator itr = s.begin();
        std::string::iterator endItr = s.end();
        for (;itr != endItr; ++itr) { // 이터레이터로 탐색할 수 있습니다.
            *itr = *itr + 1; // 다음 문자로 바꿉니다.
        }
        EXPECT_TRUE(s[0] == 'b' && s[1] == 'c'&& s[2] == 'd'); 
    }
    {
        std::string s1("abc");
        std::string s2 = s1; // s2 는 s1으로부터 복사 생성합니다.

        s1 = "def"; 
        EXPECT_TRUE(s1 == "def" && s2 == "abc"); // 서로 다른 메모리 공간을 사용하여 데이터를 공유하지 않습니다.

        s2 = "def";
        EXPECT_TRUE(s1.data() != s2.data() && s1 == s2); // 서로 다른 메모리 공간을 사용하더라도 값이 같으면 상등하다고 비교합니다.
    }
    // string 초기화
    {
        std::string s; // 빈 string 개체를 생성합니다. 
    }
    
    {
        std::string s1("OK"); // 문자열 상수는 새로운 메모리 공간에 복제되어 저장됩니다.
        std::string s2 = s1; // s2(s1)로 복사 생성자를 호출합니다. 구현에 따라 지연 복사되는 버전도 있습니다.        
    }
    {
        std::string s(3, 'a'); // 'a'문자를 3개 채웁니다.
        EXPECT_TRUE(s == "aaa");        
    }
    {
        std::string s1("OK"); // 문자열 상수는 새로운 메모리 공간에 복제되어 저장됩니다.
        std::string s2 = "OK"; // s2("OK") 와 동일합니다.
        std::string s3 = ""; // 빈 문자열을 생성합니다.        
    }
    {
        std::string s1 = "abc";

        std::string s2(s1, 1, 2); // s1[1]부터 2개 문자를 복사하여 생성합니다.
        EXPECT_TRUE(s2 == "bc");

        // std::string s3(s1, 100, 2); // (X) 예외 발생. s1[100]부터 2개 복사. out_of_range

        std::string s4(s1, 0, std::string::npos); // s1[0] 부터 끝까지 복사하여 생성합니다.
        EXPECT_TRUE(s4 == s1);

        std::string s5(s1.begin(), s1.end()); // s1.begin()부터 s1.end() 직전 까지 복사하여 생성합니다. 즉, s1[0]부터 끝까지 복사하여 생성합니다.
        EXPECT_TRUE(s5 == s1);

        const char* ptr = "abc";
        std::string s6("abc", 2); // ptr에서 2개 복사하여 생성합니다.
        EXPECT_TRUE(s6 == "ab");        
    }
    // 문자열 할당, 합성, 비교(assign(), append(), compare())
    {
        std::string s = "abc";

        s = "abcd";
        EXPECT_TRUE(s == "abcd"); // = 로 문자열을 할당합니다.

        s.assign("abced");
        EXPECT_TRUE(s == "abced"); // assign()으로 문자열을 할당합니다.
    }
    {
        std::string s1 = "Hello";
        std::string s2 = "World";
    
        EXPECT_TRUE(s1 + s2 == "HelloWorld"); // +는 두 문자열을 합성한 임시 개체를 생성합니다.

        s1 += s2; // += 은 s1에 s2를 더합니다.
        EXPECT_TRUE(s1 == "HelloWorld");

        s1.append("!!!"); // append()는 문자열을 더합니다.
        EXPECT_TRUE(s1 == "HelloWorld!!!");        
    }
    {
        std::string s = "abc";

        EXPECT_TRUE(s == "abc");
        EXPECT_TRUE(s != "def");
        EXPECT_TRUE(s < "ccc");
        EXPECT_TRUE(s.compare("ccc") < 0); // s < "ccc"
        EXPECT_TRUE(s.compare("abc") == 0); // s == "abc"
        EXPECT_TRUE(s.compare("aaa") > 0); // s > "aaa"
    }
    // 문자열 길이(size(), length(), empty())
    {
        std::string s = "abc";
        EXPECT_TRUE(s.size() == 3); // 널문자를 제외한 문자의 갯수 입니다.
        EXPECT_TRUE(s.length() == 3); // 널문자를 제외한 문자의 갯수 입니다.
        EXPECT_TRUE(s.empty() == false); // 문자열이 비었는지 검사합니다.       
    }
    // 문자열 탐색(find(), find_first_of(), find_last_of())
    {
        std::string s = "abced abcde";
        EXPECT_TRUE(s.find("ab") == 0); // "ab"와 일치하는 첫번째 인덱스를 리턴합니다.
        EXPECT_TRUE(s.find("ab", 1) == 6); // 1 인덱스 이후로 "ab와 일치하는 첫번째 인덱스를 리턴합니다.
    }
    {
        std::string s = "This is test. Hello World";
        EXPECT_TRUE(s.find_first_of("ieo") == 2); // 'i', 'e', 'o'중에 일치하는 문자가 있는 첫번째 인덱스를 찾습니다. 
        EXPECT_TRUE(s.find_last_of("ieo") == 21); // 'i', 'e', 'o'중에 일치하는 문자가 있는 마지막 인덱스를 찾습니다.   
    }

    // 부분 문자열 처리(replace(), substr(), copy()) 
    {
        std::string s = "Hello. My name is Tango.";

        std::string::size_type pos = s.find("Tango");
        std::string::size_type count = std::string("Tango").size();
        
        s.replace(pos, count, "Sam"); // Tango를 Sam으로 바꿉니다.
        EXPECT_TRUE(s == "Hello. My name is Sam."); 

        EXPECT_TRUE(s.substr(pos, 3) == "Sam"); // 부분 문자열을 복사하여 리턴합니다.

        char arr[3]; // 문자 배열
        s.copy(arr, 3, pos); // 크기가 3인 arr 배열에 pos 위치에서부터 배열 크기만큼 부분 문자열을 복사 합니다.
        EXPECT_TRUE(arr[0] == 'S' && arr[1] == 'a' && arr[2] == 'm');

    }

    // C스타일 문자열과의 호환
    {
        std::string str = "abc"; // 널종료 문자열로 생성할 수 있습니다.

        EXPECT_TRUE(strlen(str.data()) == 3); // (△) 비권장. 내부 데이터 메모리 블록을 리턴합니다. STL 구현에 따라 끝에 널문자가 있을 수도, 없을 수도 있습니다.
        EXPECT_TRUE(strlen(str.c_str()) == 3); // 널종료 문자열을 리턴하므로 안전합니다.
    }

    // 짧은 문자열 최적화
    {
        std::string s;
        EXPECT_TRUE(s.capacity() == 15); // GCC에서는 15개의 문자 입니다.
    }
}
