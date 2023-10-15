#include "gtest/gtest.h" 

#include <execution>
#include <thread>

namespace {
    template<typename Func, typename... Params>
    std::chrono::microseconds Measure(Func func, Params... params) {
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    
 
        func(params...);

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
                std::this_thread::sleep_for(std::chrono::milliseconds{1});
            }
        );

        EXPECT_TRUE(total == 100);       
    }

    template<typename T>
    void SumWithPolicy(T policy) { // 실행 정책에 따라 병렬화 합니다.
        std::vector<int> v(100, 1); // 100 개를 생성하고 1로 초기화 합니다.

        int total{0};
        std::mutex mutex;

        std::for_each(
           std::execution::seq, 
            v.begin(), v.end(), 
            [&](int item) {
                {
                    // std::lock_guard<std::mutex> lock(mutex); // total에 접근하기 전에 lock 합니다.
                     total += item;
                    std::cout << std::this_thread::get_id() << std::endl;
                }
                // std::this_thread::sleep_for(std::chrono::milliseconds{1});
            }
        );

        EXPECT_TRUE(total == 100);       
    }

    void Sort(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
        std::sort(begin, end);
    }

    template<typename T>
    void SortWithPolicy(T policy, std::vector<int>::iterator begin, std::vector<int>::iterator end) {
        std::sort(policy, begin, end);
    }
}

TEST(TestMordern, Algorithm) {

    // C++17 accumulate와 reduce 
    {
        std::vector<int> v{1, 2, 3, 4};
        int init{0};

        EXPECT_TRUE(std::accumulate(v.begin(), v.end(), init) == init + v[0] + v[1] + v[2] + v[3]); // 왼쪽에서 오른쪽으로 순서대로 계산합니다.
        EXPECT_TRUE(std::reduce(v.begin(), v.end(), init) == init + v[3] + v[2] + v[0] + v[1]); // (C++17~) 순서는 뒤죽박죽이지만 계산 결과는 같습니다.
    }

    // C++17 : 실행 정책
    {
        // 일반 알고리즘
        std::chrono::microseconds duration1{Measure(
            Sum
        )};
        std::cout << "Sum Duration : " << duration1.count() << std::endl; 
  
        // sequenced_policy
        std::chrono::microseconds duration2{Measure(
            SumWithPolicy<std::execution::sequenced_policy>,
            std::execution::seq
        )};
        std::cout << "SumWithPolicy(seq) Duration : " << duration2.count() << std::endl; 

        // // parallel_policy
        // std::chrono::microseconds duration3{Measure(
        //     SumWithPolicy<std::execution::parallel_policy>,
        //     std::execution::par
        // )};
        // std::cout << "SumWithPolicy(par) Duration : " << duration3.count() << std::endl; 

        // // parallel_unsequenced_policy
        // std::chrono::microseconds duration4{Measure(
        //     SumWithPolicy<std::execution::parallel_unsequenced_policy>,
        //     std::execution::par_unseq
        // )};
        // std::cout << "SumWithPolicy(par_unseq) Duration : " << duration4.count() << std::endl; 

    }
    {
        std::vector<int> v(1'000'000, 0); // 1,000,000 개를 생성하고 0으로 초기화 합니다.

        // 일반 sort
        std::chrono::microseconds duration1{Measure(
            Sort, 
            v.begin(), v.end() 
        )};
        std::cout << "Sort() Duration : " << duration1.count() << std::endl; 

        // sequenced_policy
        std::chrono::microseconds duration2{Measure(
            SortWithPolicy<std::execution::sequenced_policy>,
            std::execution::seq,
            v.begin(), v.end() 
        )};
        std::cout << "SortWithPolicy(seq) Duration : " << duration2.count() << std::endl; 

        // parallel_policy
        std::chrono::microseconds duration3{Measure(
            SortWithPolicy<std::execution::parallel_policy>,
            std::execution::par,
            v.begin(), v.end() 
        )};
        std::cout << "SortWithPolicy(par) Duration : " << duration3.count() << std::endl;   

        // parallel_unsequenced_policy
        std::chrono::microseconds duration4{Measure(
            SortWithPolicy<std::execution::parallel_unsequenced_policy>,
            std::execution::par_unseq,
            v.begin(), v.end() 
        )};
        std::cout << "SortWithPolicy(par_unseq) Duration : " << duration4.count() << std::endl;       
    }
}