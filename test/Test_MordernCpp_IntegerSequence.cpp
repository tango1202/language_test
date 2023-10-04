#include "gtest/gtest.h" 

namespace {
    template<size_t N, std::size_t... params>
    auto ArrayToTuple(const std::array<int, N>& arr, std::integer_sequence<size_t, params...>) {
        return std::make_tuple(arr[params]...); // arr[param1], arr[param2], arr[param3] 등으로 전개됨 
    }
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
    {
        std::array<int, 5> arr{10, 20, 30, 40, 50};    

        // arr 에서 0, 1 인덱스만 tuple로 변환
        std::tuple<int, int> data1{ArrayToTuple(arr, std::integer_sequence<size_t, 0, 1>{})};
        EXPECT_TRUE(
            std::get<0>(data1) == 10 &&
            std::get<1>(data1) == 20 
        ); 

        // arr 에서 2, 3, 4 인덱스만 tuple로 변환
        std::tuple<int, int, int> data2{ArrayToTuple(arr, std::integer_sequence<size_t, 2, 3, 4>{})};
        EXPECT_TRUE(
            std::get<0>(data2) == 30 &&
            std::get<1>(data2) == 40 &&
            std::get<2>(data2) == 50 
        ); 
    }

}