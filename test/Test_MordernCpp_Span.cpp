#include "gtest/gtest.h" 

#if 202002L <= __cplusplus // C++20~  
#include <span>
#endif

// 배열을 포인터로 전달받습니다.
int Sum1(const int* p, int size) {
    int result{0};

    for (int i{0}; i < size; ++i) {
        result += p[i];
    }    
    return result;
}

// 배열 참조자로 전달받습니다.
// const int (&arr)[]를 사용하면 sizeof(arr)시 크기를 알 수 없으므로 size를 전달받습니다.
int Sum2(const int (&arr)[], int size) { 
    int result{0};
    for (int i{0}; i < size; ++i) {
        result += arr[i];
    }    
    return result;    
}
// 템플릿으로 배열 참조자를 전달받습니다. 이때 배열 요소 갯수인 N을 추론할 수 있습니다.
template<typename T, size_t N>
int Sum3(const T (&arr)[N]) {
    int result{0};
    for (int i{0}; i < N; ++i) {
        result += arr[i];
    }    
    return result;    
}

#if 202002L <= __cplusplus // C++20~  
// 배열을 span으로 전달받습니다.
int Sum4(std::span<int> span) {

    int result{0};
    auto itr{span.begin()};
    auto endItr{span.end()};

    for (; itr != endItr; ++itr) {
        result += *itr;
    }
    return result;
}
#endif

TEST(TestMordern, Span) {

    {
        int arr[]{1, 2, 3};
        EXPECT_TRUE(Sum1(arr, sizeof(arr) / sizeof(arr[0])) == 1 + 2 + 3);
        EXPECT_TRUE(Sum2(arr, sizeof(arr) / sizeof(arr[0])) == 1 + 2 + 3);
        EXPECT_TRUE(Sum3(arr) == 1 + 2 + 3);
#if 202002L <= __cplusplus // C++20~ 
        EXPECT_TRUE(Sum4(arr) == 1 + 2 + 3);
#endif
    }
#if 202002L <= __cplusplus // C++20~     
    // span 초기화
    {
        int arr1[]{1, 2, 3};
        std::span<int> a{arr1}; // C스타일 배열
        
        std::array<int, 3> arr2{1, 2, 3};
        std::span<int> b{arr2}; // C++ 배열

        std::vector<int> v1{1, 2, 3};
        std::span<int> c{v1}; // vector

        std::vector<int> v2{1, 2, 3, 4, 5};
        std::span<int> d{v2.data(), 3}; // 특정 메모리와 갯수

        EXPECT_TRUE(
            std::ranges::equal(a, b) && // 요소들이 같으면 true입니다.
            std::ranges::equal(b, c) &&
            std::ranges::equal(c, d) 
        );

        std::string str{"abc"};
        std::span<char> e{str};
        EXPECT_TRUE(e[0] == 'a' && e[1] == 'b' && e[2] == 'c'); 

    }
#endif
}