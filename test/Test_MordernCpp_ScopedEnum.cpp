#include "gtest/gtest.h" 

TEST(TestCppPattern, ScopedEnum) {

    {
        enum Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week = Sunday; // (△) 비권장.      
    }
    {
        // 범위 있는 열거형
        enum class Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week = Week::Sunday; // 범위명을 지정하여 이름 충돌 회피       
    }
}
