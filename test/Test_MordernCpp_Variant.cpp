#include "gtest/gtest.h" 

namespace {
    void Func(int val) {std::cout << val << std::endl;}
    void Func(std::string& str) {std::cout << str << std::endl;}
}

TEST(TestMordern, Variant) {

    // C++ 17 Variant
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
    }
    {
        std::variant<int, int> var{};
        // var = 1; // (X) 컴파일 오류. 타입이 동일하면 사용할 수 없습니다.
    }
    // valueless_by_exception
    {
        class T {
        public:
            T() {}
            T(const T&) {throw std::exception();}
        };

        std::variant<int, T> var{10}; 
        EXPECT_TRUE(var.valueless_by_exception() == false);

        try {
            var = T{}; // 내부적으로 복사 생성되면서 예외를 발생합니다.
        }
        catch (std::exception&) {}

        EXPECT_TRUE(var.valueless_by_exception() == true); // 유효하지 않습니다.
    }
    // monostate
    {
        std::variant<std::monostate, int, std::string> var{};
        EXPECT_TRUE(var.valueless_by_exception() == false);        
    }
    // visit
    {
        std::vector<std::variant<int, std::string>> v{
            10, // v[0]
            20, // v[1]
            "hello" // v[2]
        };

        // 각 타입에 따라 검사한후 타입에 맞게 get해야 합니다.
        for (auto& var : v) {
            if (std::holds_alternative<int>(var)) {
                Func(std::get<int>(var));            
            }
            if (std::holds_alternative<std::string>(var)) {
                Func(std::get<std::string>(var));            
            }
        }        
    }
    {
        std::vector<std::variant<int, std::string>> v{
            10, // v[0]
            20, // v[1]
            "hello" // v[2]
        };

        // 타입에 따라 검사하는 코드를 visit() 함수 안에 은닉합니다.
        for (auto& element : v) {
            std::visit([](auto&& val){Func(val);}, element);
        }  
    }
}