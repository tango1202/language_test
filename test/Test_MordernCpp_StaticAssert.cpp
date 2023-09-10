#include "gtest/gtest.h" 

#include <cmath>
namespace {

    // T는 포인터 타입만 전달되어야 합니다.
    template<typename T>
    void Func(T t) { 
        static_assert(std::is_pointer<T>::value, "only pointer"); // is_pointer는 C++11에 추가된 type_traits
    }
}
TEST(TestMordern, StaticAssert) {
    int a = 20;
    Func(&a);
    // Func(a); // (X) 컴파일 오류. error: static assertion failed: only pointer
}





