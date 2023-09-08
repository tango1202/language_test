#include "gtest/gtest.h" 

namespace {
    int f(int) {return 1;} // #1
    int f(int*) {return 2;} // #2
}
TEST(TestCppPattern, Nullptr) {
    {
        int* ptr1 = 0;
        int* ptr2 = NULL;
        int* ptr3 = nullptr;

        EXPECT_TRUE(f(ptr1) == 2); // int* 이므로 f(int*) 호출
        EXPECT_TRUE(f(ptr2) == 2); // int* 이므로 f(int*) 호출
        EXPECT_TRUE(f(ptr3) == 2); // int* 이므로 f(int*) 호출
    }
    {
        auto ptr1 = 0; // int
        auto ptr2 = NULL; // long long
        auto ptr3 = nullptr; // nullptr_t

        EXPECT_TRUE(f(ptr1) == 1); // f(int) 호출  
        EXPECT_TRUE(f(ptr2) == 1); // f(int) 호출
        EXPECT_TRUE(f(ptr3) == 2); // f(int*) 호출
    }
}