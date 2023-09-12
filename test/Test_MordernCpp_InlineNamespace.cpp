#include "gtest/gtest.h" 

namespace MyLib {
    namespace V1 {
        int f() {return 1;} // #1
    }
    inline namespace V2 {
        int f() {return 2;} // #2
    }
}
TEST(TestMordern, InlineNamespace) {
    EXPECT_TRUE(MyLib::V1::f() == 1);
    EXPECT_TRUE(MyLib::V2::f() == 2);
    EXPECT_TRUE(MyLib::f() == 2); // MyLib에 없으면 inline 사용
}