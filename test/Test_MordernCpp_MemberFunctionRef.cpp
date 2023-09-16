#include "gtest/gtest.h" 

TEST(TestMordern, MemberFunctionRef) {
    class T {
    public:
        int Func() & {return 1;} // #1. 좌측값에서 호출
        int Func() && {return 2;} // #2. 우측값에서 호출
    };

    T t;
    EXPECT_TRUE(t.Func() == 1); // 좌측값이므로 #1 호출
    EXPECT_TRUE(std::move(t).Func() == 2); // move는 우측값이므로 #2 호출
    EXPECT_TRUE(T().Func() == 2); // T() 는 임시 개체(우측값)이므로 #2 호출   
}