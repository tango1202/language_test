#include "gtest/gtest.h" 

#include <array>

namespace {
#if 201402L <= __cplusplus // C++14~        
    template<size_t N, std::size_t... params>
    auto ArrayToTuple_14(const std::array<int, N>& arr, std::integer_sequence<size_t, params...>) {
        return std::make_tuple(arr[params]...); // arr[params#1], arr[params#2], arr[params#3] 등으로 전개됨 
    }
#endif    
}

TEST(TestMordern, IntergerSequence) {
    
    // C++14 IntergerSequence
    {

        std::array<int, 5> arr{10, 20, 30, 40, 50};
        std::tuple<int, int> data{std::make_tuple(arr[0], arr[1])};
        EXPECT_TRUE(
            std::get<0>(data) == 10 &&
            std::get<1>(data) == 20 
        ); 
    } 

#if 201402L <= __cplusplus // C++14~    
    {
        std::array<int, 5> arr{10, 20, 30, 40, 50};    

        // arr 에서 0, 1 인덱스만 tuple로 변환
        std::tuple<int, int> data1{ArrayToTuple_14(arr, std::integer_sequence<size_t, 0, 1>{})};
        EXPECT_TRUE(
            std::get<0>(data1) == 10 &&
            std::get<1>(data1) == 20 
        ); 

        // arr 에서 2, 3, 4 인덱스만 tuple로 변환
        std::tuple<int, int, int> data2{ArrayToTuple_14(arr, std::integer_sequence<size_t, 2, 3, 4>{})};
        EXPECT_TRUE(
            std::get<0>(data2) == 30 &&
            std::get<1>(data2) == 40 &&
            std::get<2>(data2) == 50 
        ); 
    }
#endif    
}