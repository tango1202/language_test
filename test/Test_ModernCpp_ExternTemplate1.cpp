#include "gtest/gtest.h" 

#include "Test_ModernCpp_ExternTemplate.h"

TEST(TestModern, ExternTemplate1) {
    using namespace ExternTemplate;

    EXPECT_TRUE(Add(1, 2) == 3); // Add<int>()가 정의 되어 포함됩니다.
}