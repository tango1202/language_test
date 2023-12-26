#include "gtest/gtest.h" 

#if 202002L <= __cplusplus // C++20~
#include <source_location>
namespace SourceLocation_1 {
    void Log(std::string message, const std::source_location& location = std::source_location::current()) {

        // Message.
        // source_location : F:\Data\language_test\test\Test_MordernCpp_Utility.cpp, virtual void TestMordern_Utility_Test::TestBody()(41, 12)
        std::cout 
            << message << std::endl
            << "source_location : " 
            << location.file_name() << ", " << location.function_name()  // 파일명, 함수명
            << "(" << location.line() << ", " << location.column() << ")"// 줄번호, 칼럼번호
        << std::endl;
    }
}
namespace Swap_1 {
    constexpr std::pair<int, int> ConstSwap(int&& a, int&& b) {
        int resultA{std::move(a)};
        int resultB{std::move(b)};
        
        std::swap(resultA, resultB); // 컴파일 타임에 두 수를 바꿔치기 합니다.
        return std::make_pair(resultA, resultB);
    }
}
#endif

TEST(TestMordern, Utility) {
    // 정수 비교
    {
        int x{-1}; // 0xFFFF FFFF(4294967295) 
        unsigned int y{1};
        // EXPECT_TRUE(x < y); // (X) 런타임 오류. -1은 unsigned int로 형변환되어 4294967295입니다.
    }
#if 202002L <= __cplusplus // C++20~
    {
        int x{-1}; // 0xFFFF FFFF(4294967295) 
        unsigned int y{1};
        EXPECT_TRUE(std::cmp_less(x, y)); // (O) 음수와 양수를 정상적으로 비교합니다.       
    }
    {
        static_assert(std::in_range<unsigned int>(-1) == false); // unsigned int 범위 바깥입니다.
        static_assert(std::in_range<unsigned int>(1) == true); // unsigned int 범위 입니다.      
    }
 
#endif  

#if 202002L <= __cplusplus // C++20~
    // source_location
    {
        using namespace SourceLocation_1;
        Log("Message.");    
    }  
#endif  

#if 202002L <= __cplusplus // C++20~
    // 삼중 비교 
    {
        class T {
        private:
            int m_Val;

        public:
            explicit T(int val) : m_Val(val) {}

            bool operator ==(const T& other) const {return m_Val == other.m_Val;}
            bool operator <(const T& other) const {return m_Val < other.m_Val;}

            // 삼중 비교 연산자가 없습니다.
            // std::strong_ordering operator<=>(const T& other) const {return m_Val <=> other.m_Val;}
        };

        // EXPECT_TRUE(T{1} <=> T{2} == 0); // (X) 컴파일 오류. 삼중 비교 연산자가 없습니다.
        // EXPECT_TRUE(std::compare_three_way{}(T{1}, T{2}) < 0); // (X) 컴파일 오류. 삼중 비교 연산자가 없습니다.
        // EXPECT_TRUE(std::strong_order(T{1}, T{2}) < 0); // (X) 컴파일 오류. 삼중 비교 연산자가 없습니다.
        EXPECT_TRUE(std::compare_strong_order_fallback(T{1}, T{2}) < 0); // 삼중 비교 연산자가 없더라도, 삼중 비교를 할 수 있습니다.
    }
#endif
#if 202002L <= __cplusplus // C++20~
    // 유틸리티의 점진적 constexpr 개선
    {
        using namespace Swap_1;

        constexpr std::pair<int, int> result{ConstSwap(0, 1)};
        static_assert(result.first == 1 && result.second == 0);        
    }
#endif    
#if 202002L <= __cplusplus // C++20~
    {
#if __cpp_lib_string_view
        std::cout << "Support string_view" << std::endl; // string_view를 지원합니다.
#else 
        std::cout << "No string_view" << std::endl;
#endif         
    }
#endif

}
