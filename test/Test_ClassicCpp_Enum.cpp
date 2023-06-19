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
        Week::Val val = Week::Sunday; // 클래스명 사용
        EXPECT_TRUE(val == Week::Sunday);
    }
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
        Week::Val val1 = Week::Saturday; // (O) 형변환 하지 마세요.
        int val2 = Week::Saturday; // (X) 비권장. 열거형 상수의 int 형변환 가능
        EXPECT_TRUE(val1 ==  Week::Saturday);
        EXPECT_TRUE(val2 == 13);

        val1 = static_cast<Week::Val>(val2 - 1); // (X) 비권장. int는 static_cast로 열겨형으로 형변환 합니다.
        EXPECT_TRUE(val1 ==  Week::Friday);
    }


}
