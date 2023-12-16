#include "gtest/gtest.h" 
#include <cmath>

namespace {
    std::pair<int, char> f() {
        return std::pair<int, char>(10, 'a');
    }
        std::pair<int, char> g() {
        return std::make_pair(10, 'a');
    }
}

TEST(TestLegacyCpp, Pair) {

    {
        std::pair<int, char> result = f();
        EXPECT_TRUE(result.first == 10 && result.second == 'a');
    }
    {
        std::pair<int, char> result = g();
        EXPECT_TRUE(result.first == 10 && result.second == 'a');        
    }
}