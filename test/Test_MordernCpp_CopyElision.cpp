#include "gtest/gtest.h" 

namespace Test_1 {

    class A {};
    void Func1(A param) {}
    A Func2() {return A{};}
}
namespace Test_2 {
    class A_11 {
    public:
        int m_Val;
    public:
        A_11() = default;
        A_11(const A_11& other) = delete;
        A_11(A_11&& other) noexcept = delete; // 이동 생성자 사용 안함
        A_11& operator =(const A_11& other) = delete; 
        A_11& operator =(A_11&& other) noexcept = delete;         
    };
}
namespace Test_3 {
    class A_11 {
    public:
        int m_Val;
    public:
        A_11() = default;
        A_11(const A_11& other) = delete;
        A_11(A_11&& other) noexcept = default; // 억지로 이동 생성자를 사용함
        A_11& operator =(const A_11& other) = delete; 
        A_11& operator =(A_11&& other) noexcept = delete;         
    };
#if 201703L <= __cplusplus // C++17~
    class A_17 {
    public:
        int m_Val;
    public:
        A_17() = default;
        A_17(const A_17& other) = delete;
        A_17(A_17&& other) noexcept = delete; // 이동 생성자 사용 안함
        A_17& operator =(const A_17& other) = delete; 
        A_17& operator =(A_17&& other) noexcept = delete;           
    };   
#endif    
}

TEST(TestMordern, CopyElision) {
   
    // ~C++17 컴파일러 최적화에 의존한 임시 개체 최소화
    {
        using namespace Test_1;
        A a{A{}}; // A{}를 기본 생성자로 생성하고 a에 이동 생성 합니다. 하지만 컴파일러에 따라 a를 기본 생성자로 생성합니다.

        Func1(A{}); // A{}를 기본 생성자로 생성하고, param에 이동 생성 합니다. 하지만 컴파일러에 따라 param을 기본 생성자로 생성합니다.

        A result = Func2(); // A{} 기본 생성자로 생성하고, result에 이동 생성합니다. 하지만 컴파일러에 따라 result를 기본 생성자로 생성합니다.
    }
    // ~C++17 임시 개체 이동 생성 필요
    {
        using namespace Test_2;

        // A_11 a{A_11{}}; // (X) 컴파일 오류. 이동 생성자가 없음   
    }
    {
        using namespace Test_3;
        A_11 a{A_11{}}; // 컴파일러 최적화로 이동 생성자를 사용하지 않지만, 컴파일을 위해 문법적으로는 이동 생성자가 필요합니다.    
    }
#if 201703L <= __cplusplus // C++17~    
    // C++17~ 임시 개체 이동 생성 불필요
    {
        using namespace Test_3;

        A_17 a{A_17{}}; // (O) 문법적으로 복사 생성자와 이동 생성자를 사용하지 않습니다.         
    }
#endif
}