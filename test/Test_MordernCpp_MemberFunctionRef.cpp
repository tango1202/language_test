#include "gtest/gtest.h" 

TEST(TestMordern, MemberFunctionRef) {
    class T {
    public:
        int Func() & {return 1;} // #1. 좌측값에서 호출
        int Func() && {return 2;} // #2. 우측값에서 호출
    };

    T t;
    EXPECT_TRUE(t.Func() == 1);
    EXPECT_TRUE(std::move(t).Func() == 2);
    EXPECT_TRUE(T{}.Func() == 2);    
}