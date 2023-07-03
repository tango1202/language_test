#include "gtest/gtest.h" 

#include "Test_IncludeGuard1.h"
#include "Test_IncludeGuard2.h"

TEST(TestCppPattern, IncludeGuard) {
    EXPECT_TRUE(TestIncludeGuard::g_Value == 1);
}

