#include "gtest/gtest.h" 
#include <cstdarg>

namespace VariadicMacro_1 {
    #define MY_SUM_11(count, ...) T::Sum(count, __VA_ARGS__)
    #define MY_SUM2_11(count, ...) T::Sum(count, ##__VA_ARGS__)
}

TEST(TestMordern, Align) {
    // 4byte 단위로 정렬합니다.
    class alignas(alignof(int)) A_11 {
        char m_A;
        int m_B;
    };

    // 2, 4, 8, 16... 단위로 정렬하며, 내부 멤버중 제일 큰값보다 커야 합니다.
    class alignas(8) B_11 {
        char m_A[13];
        int m_B; 
    };

    // 2는 내부 멤버중 제일 큰 값인 int 보다 적은값이므로 무시되고 alignof(int) 크기로 정렬됩니다.
    class alignas(2) C_11 {
        char m_A[13];
        int m_B; 
    };

    // 4byte 단위로 멤버 변수가 할당 되므로 char(1) + int(4) = 5 이므로 4 * 2 개 영역에 할당됨
    EXPECT_TRUE(alignof(A_11) == 4 && sizeof(A_11) == 4 * 2);

    // 8byte 단위로 멤버 변수가 할당 되므로 13 + int(4) = 17 이므로 8 * 3 개 영역에 할당됨
    EXPECT_TRUE(alignof(B_11) == 8 && sizeof(B_11) == 8 * 3); 

    // 4byte 단위로 멤버 변수가 할당 되므로 13 + int(4) = 17 이므로 4 * 5 개 영역에 할당됨
    EXPECT_TRUE(alignof(C_11) == 4 && sizeof(C_11) == 4 * 5); 
}
TEST(TestMordern, VariadicMacro) {
    using namespace VariadicMacro_1;
    class T {
    public:
        static int Sum(int count, ...) {
            int result{0};
            std::va_list paramList; // 가변 인자
            va_start(paramList, count); // 가변 인자 처리 시작
            for (int i{0}; i < count; ++i) {
                result += va_arg(paramList, int); // 가변 인자 추출
            }
            va_end(paramList); // 가변 인자 처리 끝
            return result;       
        }
    };

    // 가변 인자에 인수가 있는 경우
    EXPECT_TRUE(T::Sum(3, 1, 2, 3) == 1 + 2 + 3);
    EXPECT_TRUE(MY_SUM_11(3, 1, 2, 3) == 1 + 2 + 3);

    // 가변 인자에 인수가 없는 경우
    EXPECT_TRUE(T::Sum(0) == 0);
    // EXPECT_TRUE(MY_SUM(0) == 0); // (X) 컴파일 오류. T::Sum(count, ) 로 치환됩니다.
    EXPECT_TRUE(MY_SUM2_11(0) == 0); // (O) 

}

TEST(TestMordern, Sizeof) {

    class T {
    public:
        int m_X;
    };

    // (X) C++03 에서는 컴파일 오류
    // (O) C++11 부터 허용
    EXPECT_TRUE(sizeof(T::m_X) == sizeof(int));
}

#if 202002L <= __cplusplus // C++20~
namespace VA_OPT_1 {
    int Sum(int init, int a, int b, int c) {return init + a + b + c;}
    int Sum(int init) {return init;}
    #define MY_FUNC_20(...) Sum(10 __VA_OPT__(,) __VA_ARGS__) // 가변 인수가 있다면 ,를 넣습니다.
}
TEST(TestMordern, VA_OPT) {
    using namespace VA_OPT_1;
    EXPECT_TRUE(MY_FUNC_20(1, 2, 3) == 10 + 1 + 2 + 3); // f(10, 1, 2, 3) 가변 인수가 있다면 , 를 넣습니다.
    EXPECT_TRUE(MY_FUNC_20() == 10); // f(10) 가변 인수가 없다면 ,를 넣지 않습니다.
}
#endif 

#if 202002L <= __cplusplus // C++20~

TEST(TestMordern, HasCppAttributes) {
    // __has_cpp_attribute() 매크로 함수
    {
#if __has_cpp_attribute(deprecated)
        std::cout << "Support deprecated" << std::endl; // deprecated를 지원합니다.
#else 
        std::cout << "No Support deprecated" << std::endl;
#endif
    }
    // 언어 지원 테스트
    {
#if __cpp_char8_t
        std::cout << "Support char8_t" << std::endl; // char8_t를 지원합니다.
#else 
        std::cout << "No Support char8_t" << std::endl;
#endif        
    }
}
#endif