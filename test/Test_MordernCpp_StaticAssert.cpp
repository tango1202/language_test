#include "gtest/gtest.h" 

#include <cmath>
namespace {

    // T는 포인터 타입만 전달되어야 합니다.
    template<typename T>
    void Func_11(T t) { 
        static_assert(std::is_pointer<T>::value, "only pointer"); // is_pointer는 C++11에 추가된 type_traits
    }
#if 201703L <= __cplusplus // C++17~    
    template<typename T>
    void Func_17(T t) { 
        static_assert(std::is_pointer<T>::value); 
    }
#endif    
}
TEST(TestMordern, StaticAssert) {
    
    {
        int a{20};
        Func_11(&a);
        // Func_11(a); // (X) 컴파일 오류. error: static assertion failed: only pointer
    }
#if 201703L <= __cplusplus // C++17~    
    // C++17 static_assert의 메시지 생략
    {
        int a{20};
        // Func_17(a); // (X) 컴파일 오류. error: static assertion failed
    }
#endif    
}





