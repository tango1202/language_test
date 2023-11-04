#include "gtest/gtest.h" 

namespace Enable_If_1 {
    template<typename T> 
    T Sum(T a, T b) {
        return a + b;
    }
}
namespace Enable_If_2 {
    template<
        typename T, 
        typename U = typename std::enable_if< // U 는 사용하지 않으므로 생략 가능합니다. typename = typename std::enable_if<
            std::is_integral<T>::value // 조건. 정수형 타입이면 true 입니다.
        >::type // 조건이 true인 경우에만 enable_if<>::type이 정의됩니다.
    > 
    T Sum(T a, T b) {
        return a + b;
    }
}

TEST(TestMordern, TypeTraits) {
    {
        using namespace Enable_If_1;
        EXPECT_TRUE(Sum(10, 20) == 30); // 정수 합
        EXPECT_TRUE(Sum(std::string("Hello"), std::string("World")) == std::string("HelloWorld"));         
    }
     {
        using namespace Enable_If_2;
        EXPECT_TRUE(Sum(10, 20) == 30); // 정수 합
        // EXPECT_TRUE(Sum(std::string("Hello"), std::string("World")) == std::string("HelloWorld")); // (X) 컴파일 오류. 정수 타입이 아니어서 enable_if<>::type이 정의되지 않고, SFINIE에 의해 오버로딩된 함수 후보 목록에서 제외됩니다. 따라서 함수가 없습니다. 
    }   


}