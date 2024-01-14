#include "gtest/gtest.h" 

TEST(TestModern, Optional) {
#if 201703L <= __cplusplus // C++17~
    // C++ Optional
    {
        std::optional<std::string> option{"Kim"};

        // 값을 변경합니다.
        option = "Lee";

        // 값에 접근합니다.
        EXPECT_TRUE(option.value() == "Lee");

        // 빈값으로 초기화 합니다.
        option.reset();

        // 값이 있는지 검사합니다.
        EXPECT_TRUE(option.has_value() == false);

        // = {}으로 빈 값을 대입할 수 있습니다.
        option = "Lee";
        option = {}; 
        EXPECT_TRUE(option.has_value() == false);

        // 값이 없으면 예외를 발생합니다.
        try {
            option.value();        
        }
        catch (std::bad_optional_access&) {
            std::cout << "bad_optional_access" << std::endl;
        }

        // 값이 없으면 사용할 기본값을 전달합니다.
        EXPECT_TRUE(option.value_or("empty") == "empty");    
    }
#endif
}