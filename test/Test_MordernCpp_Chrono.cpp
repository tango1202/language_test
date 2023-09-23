#include "gtest/gtest.h" 

namespace {
    template<typename Func, typename... Params>
    std::chrono::microseconds CheckMicrosecond(Func func, Params... params) {
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    

        func(params...);

        std::chrono::system_clock::time_point end{std::chrono::system_clock::now()};
        std::chrono::microseconds val{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        return val;
    }
    void MyFunc() {}
}

TEST(TestMordern, Chrono) {
    {
        std::chrono::microseconds duration{CheckMicrosecond(MyFunc)};

        std::cout<<"MyFunc() : "<<duration.count()<<std::endl; 
    }

}