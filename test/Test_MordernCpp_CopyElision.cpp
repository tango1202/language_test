#include "gtest/gtest.h" 

namespace Test_1 {
    class A {
    public:
        int m_Val;
    public:
        A() = default;
        A(const A& other) = delete;
        A(A&& other) = default; // 이동 생성자 사용
        A& operator =(const A& other) = delete; 
        A& operator =(A&& other) = delete;         
    };
    void Func1(A a) {}

    A Func2() {return A{};}
}
namespace Test_2 {
    class A {
    public:
        int m_Val;
    public:
        A() = default;
        A(const A& other) = delete;
        A(A&& other) = delete; // 이동 생성자 사용 안함
        A& operator =(const A& other) = delete; 
        A& operator =(A&& other) = delete;         
    };
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

        // A a{A{}}; // (X) 컴파일 오류. 이동 생성자가 없음   
    }
    // C++17~ 임시 개체 이동 생성 불필요
    {
        using namespace Test_2;

        A a{A{}}; // (O) 문법적으로 이동 생성자를 사용하지 않습니다.         
    }
}