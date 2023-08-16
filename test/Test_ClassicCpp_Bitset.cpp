#include "gtest/gtest.h" 
#include <bitset>

TEST(TestClassicCpp, Bitset) {
 
    std::bitset<10> b1; // 10개의 비트 플래그로 구성됨. all 0 
    std::bitset<16> b2 = 0xaaaa; // 0x1010 1010 1010 1010 
    std::bitset<10> b3(std::string("1010101010")); // 0x1010 1010 10 
    std::bitset<100> b4;


    b1.set(1); // 1번 비트 true 
    b1.reset(2); // 2번 비트 false

    b4.set(99); // true

    EXPECT_TRUE(b1.test(1) == true);
    EXPECT_TRUE(b1.test(2) == false);
    EXPECT_TRUE(b4.test(99) == true);
    EXPECT_TRUE(b4.test(100) == false);
    
}

