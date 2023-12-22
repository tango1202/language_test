#include "gtest/gtest.h" 

#include <numeric>
#include <bitset>

TEST(TestMordern, Bit) {

#if 202002L <= __cplusplus // C++20~
    {
        // 2의 거듭 제곱인지 검사합니다. 이때 부호없는 정수를 사용해야 합니다.
        static_assert(std::has_single_bit(0u) == false);
        static_assert(std::has_single_bit(1u) == true); // 2^0
        static_assert(std::has_single_bit(2u) == true); // 2^1
        static_assert(std::has_single_bit(3u) == false); 
        static_assert(std::has_single_bit(4u) == true); // 2^2 

        static_assert(std::bit_ceil(3u) == 4); // 주어진 수를 올림한 2의 거듭 제곱입니다.
        static_assert(std::bit_floor(3u) == 2); // 주어진 수를 내림한 2의 거듭 제곱입니다.
        static_assert(std::bit_width(0b00000011u) == 2); // 주어진 수를 저장할 수 있는 비트 개수입니다.

        static_assert(std::rotl(static_cast<std::uint8_t>(0b10000110u), 2) == 0b00011010); // 비트를 왼쪽으로 회전합니다.
        static_assert(std::rotr(static_cast<std::uint8_t>(0b10000110u), 2) == 0b10100001); // 비트를 오른쪽으로 회전합니다.

        static_assert(std::countl_zero(static_cast<std::uint8_t>(0b00110000u)) == 2); // 왼쪽 비트에서 연속된 0의 갯수입니다.
        static_assert(std::countl_one(static_cast<std::uint8_t>(0b11110000u)) == 4); // 왼쪽 비트에서 연속된 1의 갯수입니다.
        static_assert(std::countr_zero(static_cast<std::uint8_t>(0b00110000u)) == 4); // 오른쪽 비트에서 연속된 0의 갯수입니다.
        static_assert(std::countr_one(static_cast<std::uint8_t>(0b00110011u)) == 2); // 오른쪽 비트에서 연속된 1의 갯수입니다.
    
        static_assert(std::popcount(0b00110011u) == 4); // 1의 갯수 입니다.
    }
    {
        union Test {    
            std::uint32_t byte4; // 4byte 정수
            std::uint8_t byte[4]; // 1byte
        };

        Test test;
        test.byte4 = 0x48abcdef; // 4바이트로 저장시

        EXPECT_TRUE(test.byte4 == 0x48abcdef);
        if constexpr(std::endian::native == std::endian::little) {
            EXPECT_TRUE(test.byte[0] == 0xef); // 낮은 주소에 4바이트중 하위 바이트를 저장합니다.
            EXPECT_TRUE(test.byte[1] == 0xcd);
            EXPECT_TRUE(test.byte[2] == 0xab);
            EXPECT_TRUE(test.byte[3] == 0x48);            
        }
        else {
            EXPECT_TRUE(test.byte[0] == 0x48); // 낮은 주소에 상위 바이트를 저장합니다.
            EXPECT_TRUE(test.byte[1] == 0xab);
            EXPECT_TRUE(test.byte[2] == 0xcd);
            EXPECT_TRUE(test.byte[3] == 0xef);    
        }
    }
    {
        union Test {    
            std::uint16_t byte2[2]; // 2byte 정수
            std::uint8_t byte[4]; // 1byte
        };
        Test test;
        test.byte2[0] = 0x48ab; // 2바이트로 저장시
        test.byte2[1] = 0xcdef;

        EXPECT_TRUE(test.byte2[0] == 0x48ab);
        EXPECT_TRUE(test.byte2[1] == 0xcdef);
        if constexpr(std::endian::native == std::endian::little) {

            EXPECT_TRUE(test.byte[0] == 0xab); // 낮은 주소에 2바이트중 하위 바이트를 저장합니다.
            EXPECT_TRUE(test.byte[1] == 0x48);
            EXPECT_TRUE(test.byte[2] == 0xef); // 낮은 주소에 2바이트중 하위 바이트를 저장합니다.
            EXPECT_TRUE(test.byte[3] == 0xcd);            
        }
        else {
            EXPECT_TRUE(test.byte[0] == 0x48); // 낮은 주소에 상위 바이트를 저장합니다.
            EXPECT_TRUE(test.byte[1] == 0xab);
            EXPECT_TRUE(test.byte[2] == 0xcd);
            EXPECT_TRUE(test.byte[3] == 0xef);    
        }        
    }

#endif


}