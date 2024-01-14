#include "gtest/gtest.h" 

namespace ExplicitConversions {
    class T {
    public:
        // 암시적 형변환이 됩니다.
        operator bool() const {return true;}
    };

    class T_11 {
    public:
        // 명시적으로만 형변환 됩니다.
        explicit operator bool() const {return true;} 
    };
}

namespace ExplicitBool_1 {
    template<typename T>
    class A {
        T m_Val;
    public:
        explicit A(T val) : m_Val{val} {} // 모든 타입에 대해 암시적 형변환을 차단합니다.
    };
}

namespace ExplicitBool_2 {
#if 202002L <= __cplusplus // C++20~ 
    template<typename T>
    class A_20 {
        T m_Val;
    public:
        // 정수 타입인 경우만 explicit 입니다.
        explicit(std::is_integral<T>::value) A_20(T val) : m_Val{val} {}
    };
#endif
}
TEST(TestModern, ExplicitConversions) {
    {
        using namespace ExplicitConversions;
        T t;
        int val1{t}; // (△) 비권장. bool()을 이용하여 형변환 하고 암시적으로 int로 변환합니다.

        T_11 t_11;
        // int val2{t_11}; // (X) 컴파일 오류
        bool val3{static_cast<bool>(t_11)}; // 명시적으로 변환해야 사용할 수 있습니다.
    }
    // 안전한 bool 형변환
    {
        class T_11 {
        public:
            explicit operator bool() const {return true;}
        };

        T_11 t;
        // bool status = t; // (X) 컴파일 오류. explicit이므로 암시적 형변환이 되지 않습니다.

        if (t) { // if 문에서는 bool로 형변환 됩니다. 개체 유효성 평가에 유용합니다.
            EXPECT_TRUE(true);
        }
        else {
            EXPECT_TRUE(false);
        }
    }
}

TEST(TestModern, ExplicitBool) {
    {
        using namespace ExplicitBool_1;
        A<int> a{0};
        // A<int> b = 0; // (X) 컴파일 오류. explicit로 차단했습니다.

        A<std::string> c{"Hello"};
        // A<std::string> d = std::string{"World"}; // (X) 컴파일 오류. explicit로 차단했습니다.

    }
#if 202002L <= __cplusplus // C++20~ 
    {
        using namespace ExplicitBool_2;

        A_20<int> a{0};
        // A_20<int> b = 0; // (X) 컴파일 오류. explicit로 차단했습니다.

        A_20<std::string> c{"Hello"};
        A_20<std::string> d = std::string{"World"}; // (O) 정수 타입이 아니어서 암시적 형변환을 허용합니다.       
    }
#endif
}

