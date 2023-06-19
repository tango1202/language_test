#include "gtest/gtest.h" 

namespace {
    // enum Week {
    //     Sunday, // 기본적으로 0부터 시작하여 1씩 증가
    //     Monday, 
    //     Tuesday, 
    //     Wednesday = 10, // 값 지정한 이후로 1씩 증가
    //     Thursday, 
    //     Friday, 
    //     Saturday
    // };

    class Week {
    public:
        // 클래스내에 정의. 사용시 클래스명을 기재해야 함
        enum Val {
            Sunday, // 기본적으로 0부터 시작하여 1씩 증가
            Monday, 
            Tuesday,
            Wednesday = 10, 
            Thursday, 
            Friday, 
            Saturday
        };
    }; 
}

TEST(TestClassicCpp, Enum) {
    {
        EXPECT_TRUE(
            Week::Sunday == 0 && 
            Week::Monday == 1 && 
            Week::Tuesday == 2 &&
            Week::Wednesday == 10 && // 값 지정한 이후로 1씩 증가
            Week::Thursday == 11 &&
            Week::Friday == 12 && 
            Week::Saturday == 13
        );
    }
    {
        int val = Week::Saturday; // 열거형 상수의 int 형변환 가능
        EXPECT_TRUE(val == 13); 
    }
    {
        Week::Val val = Week::Sunday; // 클래스명 사용
        EXPECT_TRUE(val == Week::Sunday);
    }

}