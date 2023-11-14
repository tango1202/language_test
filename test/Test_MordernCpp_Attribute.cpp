#include "gtest/gtest.h" 

#include <cmath>
namespace Attribute_1 {

    // 함수가 항상 예외를 throw하거나 종료합니다. 
    // 컴파일러는 호출후 제어 연결을 할 필요가 없으므로, 이에 따른 최적화가 가능합니다.
    [[noreturn]] void f_11() {throw "error";}

    // 실제로는 리턴하므로 컴파일러는 경고를 발생합니다.
    // [[noreturn]] int g_11() {return 0;} // (X) 컴파일 경고. function declared 'noreturn' has a 'return' statement
    // [[noreturn]] void g_11() {} // (X) 컴파일 경고. 'noreturn' function does return
}
namespace Attribute_2 {
#if 201402L <= __cplusplus // C++14~       
    namespace [[deprecated]] MyLib {} // 네임스페이스
    class [[deprecated]] MyClass { // 클래스, 구조체, 공용체
        [[deprecated]] int m_Val; // 멤버 변수
        [[deprecated]] void f() {} // 멤버 함수
    }; 
    template<typename T>
    class [[deprecated]] A {}; // 템플릿
    using MyType [[deprecated]] = int; // using
    [[deprecated]] typedef int YourType; // typedef
    [[deprecated]] int val; // 변수  
#if 201703L <= __cplusplus // C++17~
    [[deprecated]] auto [a, b] = std::make_pair(1, 3.14); // 구조화된 바인딩 
#endif
    [[deprecated]] void f() {} // 함수 
    enum [[deprecated]] MyEnum {MyVal [[deprecated]]}; // 열거형, 열거자
    enum class [[deprecated]] YourEnum {YourVal [[deprecated]]}; // 범위 있는 열거형, 열거자
#endif    
}
namespace Attribute_3 {
 #if 201703L <= __cplusplus // C++17~
    enum class Error_11 {Ok, Fail};

    // 리턴값을 무시하면 안됩니다.
    [[nodiscard]] Error_11 GetLastError_17() {return Error_11::Ok;} 

    // Error_17 타입이 리턴되면 무시하면 안됩니다.
    enum class [[nodiscard]] Error_17 {Ok, Fail};

    Error_17 GetForcedError_17() {return Error_17::Ok;}
#endif
}
namespace Attribute_4 {
 #if 201703L <= __cplusplus // C++17~
    // namespace [[maybe_unused]] MyLib {} // (X) 컴파일 경고. 네임스페이스는 지원하지 않습니다.
    class [[maybe_unused]] MyClass { // 클래스, 구조체, 공용체
        [[deprecated]] int m_Val; // 멤버 변수
        [[deprecated]] void f() {} // 멤버 함수
    }; 
    template<typename T>
    class [[maybe_unused]] A {}; // 템플릿
    using MyType [[maybe_unused]] = int; // using
    [[maybe_unused]] typedef int YourType; // typedef
    [[maybe_unused]] int val; // 변수    
    [[maybe_unused]] auto [a, b] = std::make_pair(1, 3.14); // 구조화된 바인딩     
    [[maybe_unused]] void f() {} // 함수 
    void g([[maybe_unused]] int a, [[maybe_unused]] int b) {} // 함수 인자 
    enum [[maybe_unused]] MyEnum {MyVal [[maybe_unused]]}; // 열거형, 열거자
    enum class [[maybe_unused]] YourEnum {YourVal [[maybe_unused]]}; // 범위 있는 열거형, 열거자
#endif    
}
TEST(TestMordern, Attribute) {
    {
        using namespace Attribute_1;
        try {
            f_11();
        }
        catch (...) {}

        // g_11();
    }
    // fallthrough
    {
        using namespace Attribute_2;
#if 201703L <= __cplusplus // C++17~        
        int val{0};
        switch (val) {
        case 0: 
            val = 0; 
            break;
        case 1: 
            val = 1; 
            [[fallthrough]]; // 의도한 fall through이니 컴파일러에게 경고하지 말라고 알려줍니다.
        case 2: 
            val = 2; 
            break;
        }
#endif        
    }
    // nodiscard
    {
        using namespace Attribute_3;
       
#if 201703L <= __cplusplus // C++17~     
        {
            // GetLastError_17(); // (X) 컴파일 경고. 리턴값을 무시하면 안됩니다.
            Error_11 error = GetLastError_17(); 
        }
        {
            // GetForcedError_17(); // (X) 컴파일 경고. Error_17 타입이 리턴되면 무시하면 안됩니다.
            Error_17 error = GetForcedError_17();
        }
#endif
#if 202002L <= __cplusplus // C++20~
        // 생성자 지원
        {
            class T_20 {
            public:
                [[nodiscard]] T_20(int, char) {}

                static void f(T_20) {}
            };

            // T{10, 'a'}; // (X) 컴파일 경고. 생성되어 만들어진 개체를 무시하면 안됩니다.
            T_20 a{10, 'a'}; // a 변수에서 사용
            T_20 b = T_20{10, 'a'}; // b 변수에서 사용
            a = T_20{10, 'a'}; // a 변수에서 사용
            T_20::f(T_20{10, 'a'}); // f 함수에 인자로 전달해서 사용
        }  
#endif
    }
    // likely, unlikely
    {
#if 202002L <= __cplusplus // C++20~
        int val{0};
        if (val < 0) [[likely]] {} // if가 참일때가 자주 실행되니 최적화를 해주세요.
        else {}

        switch(val) {
        case 0: break;
        [[likely]] case 1: break; // case 1인 경우가 자주 실행되니 최적화를 해주세요.
        [[unlikely]]case 2: break; // case 2인 경우는 드물게 실행되니 최적화를 해주세요.    
        }
#endif        
    }

    {
        class Empty {}; // 빈 클래스는 강제로 1byte
        EXPECT_TRUE(sizeof(Empty) == 1);

        class Composite {
            int m_X;
            Empty m_Empty; // 1byte 이지만 3byte 패딩됨
        };
        EXPECT_TRUE(sizeof(Composite) == sizeof(int) + sizeof(int));
#if 202002L <= __cplusplus // C++20~
        class Composite_20 {
            int m_X;
            [[no_unique_address]] Empty m_Empty; // 0byte
        };
        EXPECT_TRUE(sizeof(Composite_20) == sizeof(int));  
#endif                
    }

}





