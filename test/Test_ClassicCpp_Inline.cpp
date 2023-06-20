#include "gtest/gtest.h" 
#include "Test_Inline.h"

TEST(TestClassicCpp, Inline) {
    EXPECT_TRUE(TestInline::Plus(10, 20) == 30);
}