#include "gtest/gtest.h" 

TEST(TestMordern, Variant) {

    // C++ Variant
    {
        std::variant<int, std::string> var{};

        // 정수 값을 입력하여 인덱스는 0입니다.
        var = 1;
        EXPECT_TRUE(std::holds_alternative<int>(var) == true);
        EXPECT_TRUE(var.index() == 0 && std::get<0>(var) == 1 && std::get<int>(var) == 1);

        // 문자열을 입력하여 인덱스는 1입니다.
        std::string str{"Hello"};
        var = str;
        EXPECT_TRUE(std::holds_alternative<std::string>(var) == true);
        EXPECT_TRUE(var.index() == 1 && std::get<1>(var) == "Hello" && std::get<std::string>(var) == "Hello");

        // 값이 없으면 예외를 발생합니다.
        try {
            var = 1;
            std::get<std::string>(var);
        }
        catch (std::bad_variant_access&) {
            std::cout << "bad_variant_access" << std::endl;
        }

        // 값이 없으면 널을 리턴합니다.
        EXPECT_TRUE(get_if<std::string>(var) == nullptr);    

// visit
// get_if
// monostate
// bad_variant_access
    }
}