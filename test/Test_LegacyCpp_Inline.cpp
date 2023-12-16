#include "gtest/gtest.h" 
#include "Test_Inline.h"

namespace {
    // ----
    // 개요
    // ----
    inline int Plus(int a, int b) {
        return a + b;
    }

    void f() {
        int x = 10;
        int y = 20;

        int z = Plus(x, y);
        // Plus 함수가 인라인화 되면 다음과 같이 변경됩니다.
        // int z = x + y;
    }
    // ----
    // 클래스 멤버 함수의 인라인화
    // ----
    // h에서
    class T {
        int f1() {return 0;} // 컴파일러 판단에 따라 inline화 됨
        int f2(); // 선언만 되었기에 inline화 안됨
    };

    // cpp에서
    int T::f2() {return 0;} // inline화 안됨
}

TEST(TestLegacyCpp, Inline) {
    // ----
    // 여러 cpp에서 사용하는 인라인 함수 정의
    // ----
    EXPECT_TRUE(TestInline::Plus(10, 20) == 30); // TestInline::Plus(10, 20)은 10 + 20 으로 인라인화 됨
}