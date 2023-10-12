#include "gtest/gtest.h" 

TEST(TestMordern, Variant) {

    // C++ Variant
    {
        // 기본 생성하면 0번째 타입의 기본 생성값으로 초기화 합니다.
        std::variant<int, std::string> var{};
        EXPECT_TRUE(std::holds_alternative<int>(var) == true);
        EXPECT_TRUE(var.index() == 0 && std::get<0>(var) == 0 && std::get<int>(var) == 0);

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
        EXPECT_TRUE(std::get_if<std::string>(&var) == nullptr);    

// visit
// get_if
// monostate
// bad_variant_access
    }

    {
        std::variant<int, int> var{};
        // var = 1; // (X) 컴파일 오류. 타입이 동일하면 사용할 수 없습니다.
    }
    {
        class T {
        public:
            T() = delete;
            explicit T(int, int) {}
        };

        // 기본 생성하면 0번째 타입의 기본 생성값으로 초기화 합니다.
        // std::variant<T, int> var{}; // (X) 컴파일 오류. 0번째 타입인 T는 기본 생성자가 없어 variant를 기본 생성할 수 없습니다.
        std::variant<T, int> var{T{10 , 20}}; 

        EXPECT_TRUE(std::holds_alternative<T>(var) == true);
        EXPECT_TRUE(var.index() == 0);

    }
}