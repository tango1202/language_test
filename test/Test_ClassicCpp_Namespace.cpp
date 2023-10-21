#include "gtest/gtest.h" 

// ----
// 이름 충돌 테스트
// ----
namespace A {
    int f() {return 10;}
}
namespace B {
    int f() {return 20;}
}
// ----
// Parser 에 필요한 항목들을 논리적으로 묶음
// ----
namespace Parser {
   void Tokenizer() {}
   void Load() {}
}
// ----
// 네임스페이스 항목의 함수 선언과 정의 분리
// ----
namespace C {
    int f() {return 30;} // 정의
    int g(); // 선언
    int h(); // 선언
}
namespace C {
    int g() { // 네임스페이스에서 정의
        return f(); // 같은 네임스페이스이면 C::f() 와 같이 명시하지 않아도 됨
    }
}
int C::h() { // C::로 명시해서 정의할 수 있음
    return f(); 
}
// ----
// 서로 다른 네임스페이스 항목 사용
// ----
namespace D {
    void d1() {}
    void d2() {
        d1(); // 같은 네임스페이스 내의 항목은 네임스페이스 명 생략
    }
}
namespace E {
    void e() {
        D::d1(); // 다른 네임스페이스의 것은 명시적으로 네임스페이스 명 지정 
    }
    void f() {
        using D::d1; // using 선언으로 d를 가져옴
        d1(); // d1는 using 선언으로 가져왔으니 네임스페이스 명 없이 사용
        D::d2(); // d2는 가져오지 않았으니 네임스페이스 명을 지정하여 사용
    }
}
namespace F {
    using namespace D; //using 지시문으로 D의 것은 다 가져옴

    void f() {
        d1(); // D의 것은 그냥 사용
        d2(); // D의 것은 그냥 사용
    }
}
// ----
// 전역 공간 `using`지시문 금지
// ----
namespace MyModule1 {
    void Test() {}
}
namespace MyModule2 {
    void Test() {} 
}
using namespace MyModule1; // 전역 공간에 MyModule1의 항목들을 모두 가져옵니다.
using namespace MyModule2; // 전역 공간에 MyModule2의 항목들을 모두 가져옵니다.

namespace MyModule3 {
    void f() {
        //Test(); // (X) 컴파일 오류. MyModule1::Test() 인지, MyModule2::Test() 인지 모릅니다.
    } 
}
// ----
// 중첩 네임스페이스
// ----
namespace MyLib {
    namespace File {
        void Load() {};
        void Save() {};
    }

    namespace Parser {
        void Tokenizer() {}
    }
}
// ----
// 별칭과 합성
// ----
namespace MyTestLibrary {
    int f() {return 40;}
}
namespace MTL = MyTestLibrary; // 별칭 정의

//namespace MTL { 
// void g() {} // (X) 컴파일 오류. 별칭으로 정의한 네임스페이스에 새로운 정의는 추가할 수 없다.
//}

namespace G {
    int f() {return 50;}
}
namespace H {
    int g() {return 60;}
}
namespace MyModule { // 여러개의 네임스페이스를 합성할 수 있음
   using namespace G;
   using namespace H;
}
TEST(TestClassicCpp, Namespace) {
    {
        EXPECT_TRUE(A::f() == 10); // 네임스페이스 A의 f() 호출
        EXPECT_TRUE(B::f() == 20); // 네임스페이스 B의 f() 호출

        EXPECT_TRUE(C::g() == 30); 
        EXPECT_TRUE(C::h() == 30);

        EXPECT_TRUE(MTL::f() == 40);

        EXPECT_TRUE(MyModule::f() == 50);
        EXPECT_TRUE(MyModule::g() == 60);
    }
    {
        MyLib::Parser::Tokenizer();
        MyLib::File::Load();    
    }
}
