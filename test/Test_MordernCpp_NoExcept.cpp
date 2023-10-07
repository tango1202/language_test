#include "gtest/gtest.h" 

namespace NoExcept_1 {
    // 기존 
    // 나열된 예외 이외에는 unexpected() 핸들러로 분기. 
    // 나열된 예외가 없으므로 예외 발생시 무조건 unexpected() 핸들러 호출
    void f() throw() {
    }

    // C++11. 예외 발생 안함. 
    // 컴파일러는 이 사양을 믿고 스택 풀기등을 안해도 되므로 최적화 수행 
    // 혹여나 예외가 발생하면 terminate()를 호출
    void g() noexcept {
    }
}
namespace NoExcept_2 {
    // void f() noexcept {
    //     throw 'a'; // (X) 컴파일 경고. terminate 된다고 경고하고, 런타임에서 terminate()를 호출하여 종료합니다.
    // }
}
namespace NoExcept_3 {
    void f() {
        throw 'a'; 
    }
    void g() noexcept {
        f(); // f() 함수에서 예외를 발생시킵니다. 컴파일 경고가 없습니다.
    }
}
namespace NoExcept_4 {
    void f() {
        throw 'a'; 
    }
    void g() noexcept {
    }
}

namespace NoExcept_5 {
    // C++14에서 noexcept는 함수 유형의 일부가 아닙니다. 
    typedef void (*MyFunc)(void); // 함수 포인터 typedef
    // typedef void (*MyFunc_17)(void) noexcept; // (X) 컴파일 오류. noexcept는 함수 유형의 일부가 아닙니다. 

    void FuncTrue() noexcept(true) {}
    void FuncFalse() noexcept(false) {}
}
namespace NoExcept_6 {
    // C++17
    typedef void (*MyFunc_17)(void) noexcept; 

    void FuncTrue() noexcept(true) {}
    void FuncFalse() noexcept(false) {}
}

TEST(TestMordern, NoExcept) {
    {
        using namespace NoExcept_1;
        f();
        g();
    }
    {
        using namespace NoExcept_3;
        // g(); // noexcept 함수 내에서 사용하는 함수 f()가 예외를 발생시켜 terminate()를 호출합니다.
    }
    {
        using namespace NoExcept_4;     
        EXPECT_TRUE(noexcept(f()) == false);   
        EXPECT_TRUE(noexcept(g()) == true);   
    }
    // (C++17~) 함수 유형에 포함된 noexcept
    {
        using namespace NoExcept_5;

        // ~C++17 이전에는 noexcept 여부와 상관없이 대입할 수 있습니다.
        MyFunc f1{FuncTrue}; 
        MyFunc f2{FuncFalse};    
    }
    {
        using namespace NoExcept_6;

        // C++17~ 이후에는 noexcept를 고려하여 대입할 수 있습니다.
        MyFunc_17 f3{FuncTrue}; 
        // MyFunc_17 f4{FuncFalse}; // (X) 컴파일 오류. noexcept가 다릅니다.  
    }    
}