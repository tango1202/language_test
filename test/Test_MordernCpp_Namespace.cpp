#include "gtest/gtest.h" 

namespace MyLib {
    namespace V1 {
        int f() {return 1;} // #1
    }
    inline namespace V2 {
        int f() {return 2;} // #2
    }
}

// (C++17~) 단순화된 중첩 네임스페이스
namespace MyLib_17::Parser {
    void Tokenizer() {}
}
namespace MyLib_17::File {
    void Load() {};
    void Save() {};
}

TEST(TestMordern, Namespace) {
    {
        EXPECT_TRUE(MyLib::V1::f() == 1);
        EXPECT_TRUE(MyLib::V2::f() == 2);
        EXPECT_TRUE(MyLib::f() == 2); // MyLib에 없으면 inline 사용
    }
    // (C++17~) 단순화된 중첩 네임스페이스
    {
        MyLib_17::Parser::Tokenizer();
        MyLib_17::File::Load();
    }
}