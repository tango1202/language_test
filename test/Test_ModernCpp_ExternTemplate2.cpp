#include "gtest/gtest.h" 

#include "Test_ModernCpp_ExternTemplate.h"

// 템플릿 선언만 합니다. 이전에 정의된 템플릿을 사용합니다.
extern template int ExternTemplate::Add<int>(int, int); // C++11

TEST(TestModern, ExternTemplate2) {
    using namespace ExternTemplate;

    EXPECT_TRUE(Add(10, 20) == 30);
}