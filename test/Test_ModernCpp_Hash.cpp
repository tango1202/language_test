#include "gtest/gtest.h" 

TEST(TestModern, Hash) {
    int a{1};
    int b{1};
    int c{2};

    size_t hashA{std::hash<int>{}(a)};
    size_t hashB{std::hash<int>{}(b)};
    size_t hashC{std::hash<int>{}(c)};

    EXPECT_TRUE(hashA == hashB); // 같은 값은 해시값도 같습니다.
    EXPECT_TRUE(hashA != hashC); // 다른 값은 해시값도 다릅니다.

    std::string str1{"Hello"};
    std::string str2{"Hello"};
    std::string str3{"Hi"};

    size_t hashStr1{std::hash<std::string>{}(str1)};
    size_t hashStr2{std::hash<std::string>{}(str2)};
    size_t hashStr3{std::hash<std::string>{}(str3)};

    EXPECT_TRUE(hashStr1 == hashStr2); // 같은 값은 해시값도 같습니다.
    EXPECT_TRUE(hashStr1 != hashStr3); // 다른 값은 해시값도 다릅니다.
}