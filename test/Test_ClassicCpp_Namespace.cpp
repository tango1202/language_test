#include "gtest/gtest.h" 

namespace A {
    int f() {return 10;}
}
namespace B {
    int f() {return 20;}
}

// Parser 에 필요한 항목들을 논리적으로 묶음
namespace Parser {
   void Tokenizer() {}
   void Load() {}
}

namespace { 
    void g();
}

namespace C {
    int f() {return 30;} // 정의
    int g(); // 선언
}

int C::g() { // A::명시해서 정의할 수 있음
    return f(); // 같은 namespace이면 C::f() 와 같이 명시하지 않아도 됨
}

namespace D {
    void d() {}

    void Func() {
        d(); // 같은 네임스페이스 내의 항목은 네임스페이스 명 생략
    }
}
namespace E {
    void e() {
        D::d(); // 다른 네임스페이스의 것은 명시적으로 네임스페이스명 지정 
    }
    void f() {
        using D::d; // using 선언으로 d를 가져옴
        d(); // d는 네임스페이스명 없이 사용
        D::Func(); // Func은 네임스페이스명 지정
    }
}

namespace MyTestLibrary {
    void f() {}
}
namespace MTL = MyTestLibrary; // 별칭 정의

//namespace MTL { 
// void g() {} // (X) 컴파일 오류. 별칭으로 정의한 namespace에 새로운 정의는 추가할 수 없다.
//}

namespace F {
    using namespace D; // D의 것은 다 가져옴

    void f() {
        d(); // D의 것은 그냥 사용
        Func(); // D의 것은 그냥 사용
    }
}

namespace MyModule { // 여러개의 namespace를 합성할 수 있음
   using namespace A;
   using namespace B;
}

namespace MyModule {
    class Date {};
    void MyFunc() {}
} 
namespace { 
    void f(MyModule::Date d) {
        // (X) 컴파일 오류. 인자중 MyModule 네임스페이스가 있어서 탐색 가능하다고 하는데 GCC 8.1에서 컴파일 오류가 발생합니다.
        // MyFunc(); 
    }
}
TEST(TestClassicCpp, Namespace) {
    EXPECT_TRUE(A::f() == 10); // namespace A의 f() 호출
    EXPECT_TRUE(B::f() == 20); // namespace B의 f() 호출

    EXPECT_TRUE(C::g() == 30); // namespace C의 f()를 호출
}