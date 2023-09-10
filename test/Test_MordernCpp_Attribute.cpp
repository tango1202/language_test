#include "gtest/gtest.h" 

#include <cmath>
namespace {

    // 함수가 항상 예외를 throw하거나 종료합니다. 
    // 컴파일러는 호출후 제어 연결을 할 필요가 없으므로, 이에 따른 최적화가 가능합니다.
    [[noreturn]] void f() {throw "error";}

    // 실제로는 리턴하므로 컴파일러는 경고를 발생합니다.
    // [[noreturn]] int g() {return 0;} // 경고. function declared 'noreturn' has a 'return' statement
}
TEST(TestMordern, Attribute) {
    {
        try {
            f();
        }
        catch (...) {}

        // g();
    }
}





