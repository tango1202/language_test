#include "gtest/gtest.h" 

#if 202002L <= __cplusplus // C++20~
#include <format>
#include <chrono>
#endif

#if 202002L <= __cplusplus // C++20~

namespace MyFormatterTest {
    struct MyClass {
        int m_X;
        int m_Y;
    public:
        MyClass(int x, int y) : m_X{x}, m_Y{y} {}
        int GetX() const {return m_X;}
        int GetY() const {return m_Y;}
    };
}

// MyClass의 값을 서식화 합니다. d : 10진수(디폴트), x : 16진수 
template <> 
struct std::formatter<MyFormatterTest::MyClass> { // #1
private:
    std::string m_FormatSpec; // 사용자가 지정한 서식 문자열에서 {0:abcd} 중 a만 저장합니다.
public:    
    // ctx : 형식 문자열의 {인수 인덱스: 뒷부분이 전달됩니다. } 전까지 파싱하고, }위치를 리턴합니다.
    // format("{0:d}, {1:x}", a, b) 로 호출시 
    //  * 처음에는 `d}, {1:x}`가 전달되고, 
    //  * 다음번은 `x}` 가 전달됩니다.
    // 만약 오류시 std::format_error를 발생시켜야 합니다. 
   //  template<typename ParseContext>
    constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx) { // #2
        auto result{std::find(ctx.begin(), ctx.end(), '}')};
        if (result != ctx.end()) {
            m_FormatSpec = std::string(ctx.begin(), result); // } 전까지 저장합니다.
        }
        else {
            throw std::format_error("invalid format string"); // #4. 예외를 발생시킵니다.
        }
        return result; // #5. } 위치를 리턴합니다.
    }
    
    // 형식 문자열에서 사용자가 입력한 내용을 각 멤버 변수에 반영하여 서식화 합니다.
    // template <typename FormatContext>
    std::format_context::iterator format(const MyFormatterTest::MyClass& obj, std::format_context& ctx) const { // #6
        if (m_FormatSpec[0] == 'x') {
            return std::format_to(ctx.out(), "MyClass : x = {:#x} y = {:#x}", obj.GetX(), obj.GetY());
        }
        else {
            return std::format_to(ctx.out(), "MyClass : x = {:d} y = {:d}", obj.GetX(), obj.GetY());
        }
    }
};

#endif

TEST(TestModern, Format) {
#if 202002L <= __cplusplus // C++20~
    {
        int a{1};
        int b{2};

        // {0}, {1}, {2}에 인수를 순서대로 출력합니다. {2}는 16진수로 출력합니다.
        // a : 1, b : 2, add : 0x3
        std::cout << std::format("a : {0}, b : {1}, add : {2:#x}", a, b, a + b) << std::endl;      
    }
    // 서식화 함수
    {
        int a{10};
        std::string result1{std::format("a : {0}", a)};
        EXPECT_TRUE(result1 == "a : 10");
        
        std::string result2;
        std::format_to(
            std::back_inserter(result2), // 출력할 이터레이터
            "a : {0}", // 서식 문자열
            a // 출력할 인수
        );
        EXPECT_TRUE(result2 == "a : 10");
        
        std::string result3;
        std::format_to_n(
            std::back_inserter(result3), // 출력할 이터레이터
            3, // 문자수 
            "a : {0}", // 서식 문자열
            a // 출력할 인수
        );
        EXPECT_TRUE(result3 == "a :"); // 문자 3개만 출력합니다.

        auto length{std::formatted_size("a : {0}", a)}; // 서식화 했을때의 길이를 구합니다.
        std::string result4;
        std::format_to_n(
            std::back_inserter(result4), // 출력할 이터레이터
            length, // 문자수 
            "a : {0}", // 서식 문자열
            a // 출력할 인수
        );
        EXPECT_TRUE(result4 == "a : 10");
    }
    // 서식 문자열
    {
        EXPECT_TRUE(std::format("data:{},{},{}", 0, 10, 20) == "data:0,10,20");
        EXPECT_TRUE(std::format("data:{2},{1},{0}", 0, 10, 20) == "data:20,10,0");
        EXPECT_TRUE(std::format("data:{0},{0},{1}", 0, 10, 20) == "data:0,0,10"); // 특정 인덱스를 여러번 사용할 수 있습니다.
    }
    // 표준 서식 지정자
    {
        // 채움과 정렬
        EXPECT_TRUE(std::format("{0:*<5}", 3) == "3****"); // 5자리 너비에서 빈공간은 *로 채웁니다.
        EXPECT_TRUE(std::format("{0:*>5}", 3) == "****3");
        EXPECT_TRUE(std::format("{0:*^5}", 3) == "**3**");

        // 인수 인덱스 생략
        EXPECT_TRUE(std::format("{:*<5}, {:*>5}, {:*^5}", 3, 4, 5) == "3****, ****4, **5**"); // 인수 인덱스를 생략 할 수 있습니다.

        // 부호
        EXPECT_TRUE(std::format("{0:*>+5}", 3) == "***+3"); // 양수인 경우 +를 표시합니다.
        EXPECT_TRUE(std::format("{0:+5}", 3) == "   +3"); // 빈공간은 디폴트로 공백 문자로 채웁니다. 숫자는 기본적으로 오른쪽 정렬입니다.
        EXPECT_TRUE(std::format("{0:<+5}", 3) == "+3   "); // 왼쪽 정렬로 표시합니다.

        // #
        EXPECT_TRUE(std::format("{0}, {0:#b}, {0:#o}, {0:#x}", 2) == "2, 0b10, 02, 0x2"); // #은 정수에서 진법을 출력합니다.
        EXPECT_TRUE(std::format("{0:g}", 3.) == "3"); // g는 소수점 이하가 없으면, 정수로 표시됩니다. 
        EXPECT_TRUE(std::format("{0:#g}", 3.) == "3.00000"); // g에서 항상 소수점 이하를 출력합니다. 기본 정밀도는 소수점을 포함하여 6입니다.

        // 0
        EXPECT_TRUE(std::format("{0:0>+5}", 3) == "000+3"); // 채움과 정렬은 부호 앞에 표시됩니다.
        EXPECT_TRUE(std::format("{0:+05}", 3) == "+0003"); // 대체 문자 0은 부호 뒤에 표시됩니다.
    
        // 너비와 정밀도
        EXPECT_TRUE(std::format("{0:*>10f}", 3.14) == "**3.140000"); // 너비만 지정
        EXPECT_TRUE(std::format("{0:.4f}", 3.141592) == "3.1416"); // 정밀도만 지정
        EXPECT_TRUE(std::format("{0:*>8.4f}", 3.141592) == "**3.1416"); // 너비와 정밀도 지정. 반올림하여 표시됨.
      
        // 너비와 정밀도의 f, g 차이
        EXPECT_TRUE(std::format("{0:*>8.4f}", 3.14) == "**3.1400"); // 소수점 이하가 14밖에 없어서 나머지는 0으로 채웁니다.
        EXPECT_TRUE(std::format("{0:*>8.4g}", 3.14) == "****3.14"); // 소수점 이하가 14밖에 없어서 정밀도 4를 모두 채우지는 못합니다.
        EXPECT_TRUE(std::format("{0:*>#8.4g}", 3.14) == "***3.140"); // #을 이용하여 0으로 채울 수 있습니다. 이때 정밀도는 .까지 포함합니다.      

        // 너비와 정밀도를 외부에서 전달받을 수 있습니다.
        EXPECT_TRUE(std::format("{:*>{}.{}f} {:s}", 3.14, 8, 4, "test") == "**3.1400 test"); // {}로 너비와 정밀도를 전달할 수 있습니다. 다만, 이때 인수 인덱스 지정은 안됩니다.

        // 너비는 최소 길이 입니다. 주어진 너비보다 수치가 크면 무시됩니다.
        EXPECT_TRUE(std::format("{0:6.4f}", 123.456789) == "123.4568"); // 최소 크기 6을 지정했고, 8자리로 출력됩니다.

        // 문자열
        EXPECT_TRUE(std::format("{0}, {0:s}", "hello") == "hello, hello"); 

        // 정수
        EXPECT_TRUE(std::format("{0}, {0:b}, {0:o}, {0:x}", 2) == "2, 10, 2, 2");
        EXPECT_TRUE(std::format("{0}, {0:#b}, {0:#o}, {0:#x}", 2) == "2, 0b10, 02, 0x2"); // #을 붙여 진법을 표시합니다.
        EXPECT_TRUE(std::format("{0}, {0:#0b}, {0:#0o}, {0:#0x}", 2) == "2, 0b10, 02, 0x2"); // #0을 붙여 진법과 빈자리는 0으로 채웁니다. 하지만 빈자리가 없어 0은 표시되지 않습니다.
        EXPECT_TRUE(std::format("{0}, {0:#05b}, {0:#05o}, {0:#05x}", 2) == "2, 0b010, 00002, 0x002"); // #0을 붙이고, 5를 붙여 5자리에 진법과 0을 표시합니다.

        // bool
        EXPECT_TRUE(std::format("{0}, {0:s}", true) == "true, true");        

        // 실수
        
        // 실수 기본값은 .8f와 같습니다.
        EXPECT_TRUE(std::format("{0}, {0:.8f}", 123456789.123456789) == "123456789.12345679, 123456789.12345679");

        // 16진수 표기, 지수, 고정폭, 자동을 지원합니다.
        EXPECT_TRUE(std::format("{0}, {0:a}, {0:e}, {0:f}, {0:g}", 3.141592) == "3.141592, 1.921fafc8b007ap+1, 3.141592e+00, 3.141592, 3.14159");

        // f와 g의 차이. 
        // f는 항상 소수점으로 출력하고, g는 소수점 이하의 내용이 있는 경우만 출력합니다., 고정폭으로 표시하기 힘들면 지수로 출력합니다.
        EXPECT_TRUE(std::format("{0:f}", 3.) == "3.000000"); // 소수점 이하를 0으로 표시합니다. 기본 정밀도는 6입니다.
        EXPECT_TRUE(std::format("{0:g}", 3.) == "3"); // g는 소수점 이하가 없으면, 정수로 표시됩니다. 
        EXPECT_TRUE(std::format("{0:f}, {0:g}", 123456789.123456789) == "123456789.123457, 1.23457e+08"); // 고정폭으로 표시하기 힘들면 지수로 출력합니다.
        // f의 정밀도는 .을 포함하지 않은 갯수이고, g의 정밀도는 .을 포함한 갯수입니다.
        EXPECT_TRUE(std::format("{0:f}, {0:g}", 3.123456789) == "3.123457, 3.12346"); // f와 g의 기본 정밀도는 6입니다. 하지만, g는 소수점을 포함한 정밀도 입니다. 
        EXPECT_TRUE(std::format("{0:.3f}, {0:.3g}", 3.123456789) == "3.123, 3.12");

        // 포인터
        EXPECT_TRUE(std::format("{0:p}", nullptr) == "0x0"); // 주소를 16진수로 표시합니다.
    }
    // chrono 서식 지정자
    {
        using namespace std::chrono; // 표준 사용자 정의 리터럴인 23y, 5d, 14h, 5min, 3s를 사용하기 위해서 추가합니다.
        
        std::chrono::year_month_day date{23y, std::chrono::December, 5d};
        EXPECT_TRUE(std::format("{:%Y/%m/%d (%A)}", date) == "0023/12/05 (Tuesday)");
        EXPECT_TRUE(std::format("{:%Y-%m month, day = %d}", date) == "0023-12 month, day = 05"); // / 대신 다양한 문자 표현이 가능합니다.
        EXPECT_TRUE(std::format("{:%D}", date) == "12/05/23"); // %m/%d/%y 와 동일합니다.
        EXPECT_TRUE(std::format("{:%F}", date) == "0023-12-05"); // %Y-%m-%d 와 동일합니다.
        EXPECT_TRUE(std::format("{:%x}", date) == "12/05/23"); // 로케일의 날짜 표시 형식입니다.
        
        std::chrono::hh_mm_ss time{14h + 5min + 3s};
        EXPECT_TRUE(std::format("{:%H:%M:%S (%p)}", time) == "14:05:03 (PM)"); // 24시간제로 표시합니다.
        EXPECT_TRUE(std::format("{:%I:%M:%S (%p)}", time) == "02:05:03 (PM)"); // 12시간제로 표시합니다.
        EXPECT_TRUE(std::format("{:%R}", time) == "14:05"); // %H:%M 과 동일합니다.
        EXPECT_TRUE(std::format("{:%T}", time) == "14:05:03"); // %H:%M:%S와 동일합니다.
        EXPECT_TRUE(std::format("{:%r}", time) == "02:05:03 PM"); // 로케일의 12시간제로 표시합니다.
        EXPECT_TRUE(std::format("{:%X}", time) == "14:05:03"); // 로케일의 시간 형식으로 표시합니다.
    }

#endif
}
TEST(TestModern, Formatter) {
#if 202002L <= __cplusplus // C++20~

    // formatter
    {
        using namespace MyFormatterTest;
        
        EXPECT_TRUE(
            std::format("{0}, {1:d}, {2:x}", 
            MyClass{1, 2}, // 기본은 10진수로 출력합니다.
            MyClass{3, 4}, // d는 10진수로 출력합니다.
            MyClass{5, 6}) == // x는 0x 접두어를 붙여 16진수로 출력합니다.
            "MyClass : x = 1 y = 2, MyClass : x = 3 y = 4, MyClass : x = 0x5 y = 0x6"
        );
    }
#endif
}