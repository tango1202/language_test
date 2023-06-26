#include "gtest/gtest.h" 

namespace {
    class T {
    public:
        T operator +(const T &b) const { return *this; }   
        T& operator +=(const T &b) { return *this; }
    };

}
TEST(TestClassicCpp, Operators) {

    // ----
    // 이항 산술 연산
    // ----
    {
        T a;
        T b;

        // (△) 비권장. 임시 개체가 생성됨
        T c = a + b; // a + b 인 임시 개체를 생성. c의 복사 생성자 호출하여 임시 개체 대입

        // (O) 권장. 임시 개체가 생성되지 않음
        T d = a; // d의 복사 생성자를 호출하여 a값 대입
        d += b; // d에 b값 증가       

    }

    // ----
    // 증감 연산자
    // ----
    {
        int n = 10;
        int a = ++n;
        EXPECT_TRUE(a == 11); // 증가시킨 후 값
        EXPECT_TRUE(n == 11);

        int b = n++;
        EXPECT_TRUE(b == 11); // (△) 비권장. 의도한 것인지 조금 헷갈립니다. 증가시킨 전 값. 
        EXPECT_TRUE(n == 12);
    }

    // ----
    // 비교 연산 표현
    // ----
    {
        int x = 10;
        int y = 10;
        EXPECT_TRUE(!(x < y || y < x)); // x == y

        x = 10;
        y = 20;   
        EXPECT_TRUE((x < y || y < x)); // x != y

        x = 10;
        y = 20;
        EXPECT_TRUE((x < y)); // x < y

        x = 20;
        y = 10;
        EXPECT_TRUE((y < x)); // x > y      

        x = 10;
        y = 20;
        EXPECT_TRUE(!(y < x)); // x <= y

        x = 10;
        y = 10;
        EXPECT_TRUE(!(y < x)); // x <= y   

        x = 20;
        y = 10;
        EXPECT_TRUE(!(x < y)); // x >= y

        x = 10;
        y = 10;
        EXPECT_TRUE(!(x < y)); // x >= y   
    }
}