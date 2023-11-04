#include "gtest/gtest.h" 

namespace {
    template<typename Func, typename... Params>
    std::chrono::microseconds Measure(Func func, Params&&... params) {
        // func 실행전 time_point 측정
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    

        // func 실행
        func(std::forward<Params>(params)...);

        // func 실행 후 time_point 측정
        std::chrono::system_clock::time_point end{std::chrono::system_clock::now()};

        // 두 time_point 간의 차
        std::chrono::microseconds val{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        return val;
    }
    void MyFunc() {}
}

TEST(TestMordern, Chrono) {
    {
        std::chrono::microseconds duration{Measure(MyFunc)};

        std::cout << "MyFunc() : " << duration.count() << std::endl; 
    }
}