#include "gtest/gtest.h" 

namespace MyLib {
    namespace V1 {
        int f() {return 1;} // #1
    }
    inline namespace V2_11 {
        int f() {return 2;} // #2
    }
}

#if 201703L <= __cplusplus // C++17~
// (C++17~) 단순화된 중첩 네임스페이스
namespace MyLib_17::Parser {
    void Tokenizer() {}
}
namespace MyLib_17::File {
    void Load() {};
    void Save() {};
}
#endif
#if 202002L <= __cplusplus // C++20~ 
namespace A_20::B_17::inline C_11 {
    void f() {}
}

// namespace A_20 {
//     namespace B_17 {
//         inline namespace C_11 {
//         }
//     }
// }
#endif

TEST(TestModern, Namespace) {
    {
        EXPECT_TRUE(MyLib::V1::f() == 1);
        EXPECT_TRUE(MyLib::V2_11::f() == 2);
        EXPECT_TRUE(MyLib::f() == 2); // MyLib에 없으면 inline 사용
    }
#if 201703L <= __cplusplus // C++17~    
    // (C++17~) 단순화된 중첩 네임스페이스
    {
        MyLib_17::Parser::Tokenizer();
        MyLib_17::File::Load();
    }
#endif 
#if 202002L <= __cplusplus // C++20~ 
    {
        A_20::B_17::f();
    }  
#endif
}