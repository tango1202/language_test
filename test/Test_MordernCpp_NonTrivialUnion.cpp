#include "gtest/gtest.h" 

namespace {

    union T {
        std::string m_Str; // 생성자와 소멸자가 있는 non-trivial
        std::vector<int> m_Vec; // 생성자와 소멸자가 있는 non-trivial
        ~T() {}}


    };
}

TEST(TestCppPattern, NonTrivialUnion) {

    T obj{"Hello"};
    // T obj = {"Hello"};
}


// std::variant 
// https://en.cppreference.com/w/cpp/utility/variant
// std::string16
// std::w32string



