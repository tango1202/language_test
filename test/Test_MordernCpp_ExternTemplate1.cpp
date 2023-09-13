#include "gtest/gtest.h" 

#include "Test_MordernCpp_ExternTemplate.h"

TEST(TestMordern, ExternTemplate1) {
    using namespace ExternTemplate;

    EXPECT_TRUE(Add(1, 2) == 3); // Add<int>()가 정의 되어 포함됩니다.
}