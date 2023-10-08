#include "gtest/gtest.h" 

namespace {
    class T {
    public:
        // 암시적 형변환이 됩니다.
        operator bool() {return true;}
    };

    class T_11 {
    public:
        // 명시적으로만 형변환 됩니다.
        explicit operator bool() {return true;} 
    };
}
TEST(TestMordern, ExplicitConversions) {
    T t;
    int val1{t}; // (△) 비권장. bool() 을 이용하여 형변환 하고 암시적으로 int로 변환합니다.

    T_11 t_11;
    // int val2{t_11}; // (X) 컴파일 오류
    bool val3{static_cast<bool>(t_11)}; // 명시적으로 변환해야 사용할 수 있습니다.
}