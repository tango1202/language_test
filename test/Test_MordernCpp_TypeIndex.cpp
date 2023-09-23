#include "gtest/gtest.h" 

#include <typeindex>

TEST(TestMordern, TypeIndex) {
    class A {};
    class B {};

    // const std::type_info& 여서 컨테이너에서 사용할 수 없습니다.
    // const std::type_info& a{typeid(A)};
    // const std::type_info& b{typeid(B)};

    std::type_index a{typeid(A)};
    std::type_index b{typeid(B)};

    std::map<std::type_index, int> m;

    m.insert(std::make_pair(a, 0)); // A 타입의 추가 정보를 저장할 수 있습니다.
    m.insert(std::make_pair(b, 1)); // B 타입의 추가 정보를 저장할 수 있습니다.

    EXPECT_TRUE(m[typeid(A)] == 0 && m[typeid(B)] == 1);
}

