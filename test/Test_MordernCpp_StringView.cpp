#include "gtest/gtest.h" 

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
#if 201703L <= __cplusplus // C++17~
namespace StringView_3 {
    std::size_t Func(std::string_view sv) {
        return sv.length();
    }    
}
#endif

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
#if 201703L <= __cplusplus // C++17~    
    {
        using namespace StringView_3;

        std::string str1{"Hello"};
        const char* str2{"World"};

        EXPECT_TRUE(Func(str1) == 5); // (O) 불필요하게 string 개체를 생성하지 않습니다.
        EXPECT_TRUE(Func(str2) == 5); // (O) 불필요하게 string 개체를 생성하지 않습니다.
    }
#endif    
}

