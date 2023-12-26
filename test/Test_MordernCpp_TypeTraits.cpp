#include "gtest/gtest.h" 

namespace Enable_If_1 {
    template<typename T> 
    T Add(T a, T b) {
        return a + b;
    }
}
namespace Enable_If_2 {
    template<
        typename T, 
        typename U = typename std::enable_if< // U 는 사용하지 않으므로 생략 가능합니다. typename = typename std::enable_if<
            std::is_integral<T>::value // 조건. 정수형 타입이면 true 입니다.
        >::type // 조건이 true인 경우에만 enable_if<>::type이 정의됩니다.
    > 
    T Add(T a, T b) {
        return a + b;
    }
}
namespace Constant_1 {
#if 202002L <= __cplusplus // C++20~      
    constexpr int Factorial(int val) {
        
        int result{1}; // 초기화된 지역 변수 정의

        if (val < 1) {
            return 1; // 2개 이상의 리턴문
        }

        for (int i{val}; 0 < i; --i) { // 제어문
            result *= i;
        }
        bool isConstant = std::is_constant_evaluated();

        if (!std::is_constant_evaluated()) {
            std::cout << "Factorial Result : " << result << std::endl; // 컴파일 타임 함수가 아니라면, 메시지를 출력합니다.
        }
        return result;
    } 
#endif    
}

TEST(TestMordern, TypeTraits) {
    {
        using namespace Enable_If_1;
        EXPECT_TRUE(Add(10, 20) == 30); // 정수 합
        EXPECT_TRUE(Add(std::string("Hello"), std::string("World")) == std::string("HelloWorld"));         
    }
    {
        using namespace Enable_If_2;
        EXPECT_TRUE(Add(10, 20) == 30); // 정수 합
        // EXPECT_TRUE(Add(std::string("Hello"), std::string("World")) == std::string("HelloWorld")); // (X) 컴파일 오류. 정수 타입이 아니어서 enable_if<>::type이 정의되지 않고, SFINIE에 의해 오버로딩된 함수 후보 목록에서 제외됩니다. 따라서 함수가 없습니다. 
    }  
#if 202002L <= __cplusplus // C++20~
    {
        using namespace Constant_1;
        static_assert(Factorial(5) == 1 * 2 * 3 * 4 * 5); // 컴파일 타임 함수입니다.

        int val{5};
        int result{Factorial(val)}; // 런타임 함수입니다. 메시지를 출력합니다.
        EXPECT_TRUE(result == 1 * 2 * 3 * 4 * 5);
    } 
#endif
#if 202002L <= __cplusplus // C++20~
    {
        static_assert(std::is_bounded_array<int[3]>::value == true); // 요소의 갯수가 알려진 타입입니다.
        static_assert(std::is_bounded_array<int[]>::value == false); // 요소의 갯수가 알려지지 않았습니다.          
        static_assert(std::is_bounded_array<int>::value == false); // 배열이 아니면 false 입니다.  

        static_assert(std::is_unbounded_array<int[3]>::value == false); // 요소의 갯수가 알려진 타입입니다.
        static_assert(std::is_unbounded_array<int[]>::value == true); // 요소의 갯수가 알려지지 않았습니다.          
        static_assert(std::is_unbounded_array<int>::value == false); // 배열이 아니면 false 입니다.          
    }
#endif
}