#include "gtest/gtest.h"
#include "../cpp/src/Plus.h"

TEST(TestPlus, Test1) {
    EXPECT_TRUE(Plus::Run(1, 2) == 3);    
}

TEST(TestPlus, Test2) {
    EXPECT_TRUE(Plus::Run(1, -1) == 0);    
}