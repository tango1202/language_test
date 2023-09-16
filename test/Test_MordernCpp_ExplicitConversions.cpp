#include "gtest/gtest.h" 

namespace {
    class T1 {
    public:
        // 암시적 형변환이 됩니다.
        operator bool() {return true;}
    };

    class T2 {
    public:
        // 명시적으로만 형변환 됩니다.
        explicit operator bool() {return true;} 
    };
}
TEST(TestMordern, ExplicitConversions) {
    T1 t1;
    int val1{t1}; // (△) 비권장. bool() 을 이용하여 형변환 하고 암시적으로 int로 변환합니다.

    T2 t2;
    // int val2{t2}; // (X) 컴파일 오류
    bool val3{static_cast<bool>(t2)}; // 명시적으로 변환해야 사용할 수 있습니다.
}