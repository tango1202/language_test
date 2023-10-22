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

TEST(TestMordern, Namespace) {
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
}