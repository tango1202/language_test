#include "gtest/gtest.h" 
#include <cstdarg>

namespace {
    #define MY_SUM(count, ...) T::Sum(count, __VA_ARGS__)
    #define MY_SUM2(count, ...) T::Sum(count, ##__VA_ARGS__)
}

TEST(TestMordern, VariadicMacro) {
    class T {
    public:
        static int Sum(int count, ...) {
            int result = 0;
            std::va_list paramList; // 가변 인자
            va_start(paramList, count); // 가변 인자 처리 시작
            for (int i = 0; i < count; ++i) {
                result += va_arg(paramList, int); // 가변 인자 추출
            }
            va_end(paramList); // 가변인자 처리 끝
            return result;       
        }
    };

    // 가변 인자에 인수가 있는 경우
    EXPECT_TRUE(T::Sum(3, 1, 2, 3) == 1 + 2 + 3);
    EXPECT_TRUE(MY_SUM(3, 1, 2, 3) == 1 + 2 + 3);

    // 가변 인자에 인수가 없는 경우
    EXPECT_TRUE(T::Sum(0) == 0);
    // EXPECT_TRUE(MY_SUM(0) == 0); // (X) 컴파일 오류. T::Sum(count, ) 로 치환됩니다.
    EXPECT_TRUE(MY_SUM2(0) == 0); // (O) 

}