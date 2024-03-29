#include "gtest/gtest.h" 

#if 201703L <= __cplusplus // C++17~
    #include <execution>
#endif
#include <thread>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace {
    template<typename Func, typename... Params>
    std::chrono::microseconds Measure(Func func, Params&&... params) {
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    
 
        func(std::forward<Params>(params)...);

        std::chrono::system_clock::time_point end{std::chrono::system_clock::now()};
        std::chrono::microseconds val{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        return val;
    }


    void Sum() {
        std::vector<int> v(100, 1); // 100 개를 생성하고 1로 초기화 합니다.

        int total{0};

        std::for_each(
            v.begin(), v.end(), 
            [&](int item) {
                total += item; // 각 요소의 값을 누적합니다.
                //!! 테스트 속도 때문에 임시 주석
                //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
            }
        );

        EXPECT_TRUE(total == 100);       
    }
#if 201703L <= __cplusplus // C++17~
    void SumPararell() { 
        std::vector<int> v(100, 1); // 100 개를 생성하고 1로 초기화 합니다.

        int total{0};
        std::mutex mutex;

        std::for_each(
           std::execution::par, 
            v.begin(), v.end(), 
            [&](int item) {
                {
                    std::lock_guard<std::mutex> lock(mutex); // total에 접근할 때만 lock 합니다.
                    total += item;
                }
                //!! 테스트 속도 때문에 임시 주석
                //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
            }
        );

        EXPECT_TRUE(total == 100);       
    }
#endif
}

TEST(TestModern, Algorithm) {

    // C++17 accumulate와 reduce 
    {
        std::vector<int> v{1, 2, 3, 4};
        int init{0};

        EXPECT_TRUE(std::accumulate(v.begin(), v.end(), init) == init + v[0] + v[1] + v[2] + v[3]); // 왼쪽에서 오른쪽으로 순서대로 계산합니다.
#if 201703L <= __cplusplus // C++17~
        EXPECT_TRUE(std::reduce(v.begin(), v.end(), init) == init + v[3] + v[2] + v[0] + v[1]); // (C++17~) 순서는 뒤죽박죽이지만 계산 결과는 같습니다.
#endif
    }
   
    // C++17 병렬 알고리즘과 실행 정책
    {
        // 일반 알고리즘
        std::chrono::microseconds duration1{Measure(
            Sum
        )};
        std::cout << "Sum Duration : " << duration1.count() << std::endl; 
  
#if 201703L <= __cplusplus // C++17~
        // parallel_policy
        std::chrono::microseconds duration3{Measure(
            SumPararell
        )};
        std::cout << "SumPararell Duration : " << duration3.count() << std::endl; 
#endif        
    }
#if 202002L <= __cplusplus // C++20~
    // constexpr 알고리즘
    {
        constexpr int arr[]{1, 2, 3};
        constexpr int sum{std::accumulate(arr, arr + 3, 0)}; // 시퀀스의 값을 누적합니다.
        static_assert(sum == 1 + 2 + 3); // 컴파일 타임 상수로 구합니다.
    }
#endif
#if 202002L <= __cplusplus // C++20~
    {
        static_assert(std::midpoint(1.0, 2.0) == 1.5); // 실수
        
        static_assert(std::midpoint(1, 3) == 2); 
        static_assert(std::midpoint(1, 2) == 1); // 나누어 떨어지지 않으면 내림을 합니다.
        static_assert(std::midpoint(1, 4) == 2); // 나누어 떨어지지 않으면 내림을 합니다.  
    }
    {
        // (a + t(b - a))
        static_assert(std::lerp(1, 3, 0.5) == 2); // (1 + 0.5(3 - 1)) 
        static_assert(std::lerp(1, 3, 1) == 3); 
        static_assert(std::lerp(1, 3, 2) == 5);
        static_assert(std::lerp(1, 3, 3) == 7);
        static_assert(std::lerp(1, 3, 4) == 9);
    }
#endif
}