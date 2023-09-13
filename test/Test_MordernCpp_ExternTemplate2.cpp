#include "gtest/gtest.h" 

#include "Test_MordernCpp_ExternTemplate.h"

// 이전에 정의된 템플릿을 사용합니다.
extern template int ExternTemplate::Add<int>(int, int);

TEST(TestMordern, ExternTemplate2) {
    using namespace ExternTemplate;

    EXPECT_TRUE(Add(10, 20) == 30);
}