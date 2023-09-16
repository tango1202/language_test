#include "gtest/gtest.h" 

TEST(TestMordern, ScopedEnum) {

    {
        enum Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week{Sunday}; // (△) 비권장.  

    }
    {
        // 혹은
        class Week {
        public:
            // 클래스내에 정의. 사용시 클래스명을 기재해야 함
            enum Val {
                Sunday, Monday, Tuesday, Wednesday, 
                Thursday, Friday, Saturday
            };
        };

        Week::Val val{Week::Sunday}; // 범위 확인 연산자와 클래스명 사용       
    }
    {
        // 범위 있는 열거형
        enum class Week {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
        Week week{Week::Sunday}; // 범위명을 지정하여 이름 충돌 회피       
    }
}
