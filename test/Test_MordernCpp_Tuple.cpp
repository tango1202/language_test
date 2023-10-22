#include "gtest/gtest.h" 

namespace Tuple_1 {
    std::tuple<int, std::string, char> Func() {
        return std::tuple<int, std::string, char>{10, "Name", 'a'}; // return {10, "Name", 'a'}; 와 동일합니다.
    }
}
namespace Tuple_2 {
    std::tuple<int, std::string, char> Func() {
        return std::make_tuple(10, "Name", 'a');
    }
}
namespace Tuple_3 {
    int Sum(int a, int b, int c) {
        return a + b + c;
    }
}        

TEST(TestMordern, Tuple) {

    {
        using namespace Tuple_1;
        // 3개의 데이터로 구성된 tuple 입니다.
        std::tuple<int, std::string, char> result{Func()};

        // get()함수를 이용하여 순서를 지정하여 데이터에 접근합니다.
        EXPECT_TRUE(
            std::get<0>(result) == 10 && 
            std::get<1>(result) == "Name" && 
            std::get<2>(result) == 'a'
        );
    }
    // tie
    {
        using namespace Tuple_2;

        int val;
        std::string name;
        char ch;

        // tie 함수로 지역 변수의 참조자로 구성된 tuple을 만듭니다.
        std::tie(val, name, ch) = Func();

        EXPECT_TRUE(
            val == 10 && 
            name == "Name" && 
            ch == 'a'
        );    
    }
    // tuple_cat
    {
        auto t1{std::make_tuple(10, "Name", 'a')};
        auto t2{std::make_tuple("Hello", 20, 'b')};

        auto t3{std::tuple_cat(t1, t2)};   
        EXPECT_TRUE(
            std::get<0>(t3) == 10 && 
            std::get<1>(t3) == "Name" && 
            std::get<2>(t3) == 'a' &&
            std::get<3>(t3) == "Hello" && 
            std::get<4>(t3) == 20 && 
            std::get<5>(t3) == 'b'
        );             
    }
    // tuple_size
    {
        static_assert(std::tuple_size<std::tuple<int, std::string, char>>::value == 3);
    }
    // tuple_element
    {
        using MyTuple = std::tuple<int, std::string, char>;
        MyTuple data{10, "Name", 'a'};

        std::tuple_element<0, MyTuple>::type intVal{std::get<0>(data)};
        std::tuple_element<1, MyTuple>::type stringVal{std::get<1>(data)};
        std::tuple_element<2, MyTuple>::type charVal{std::get<2>(data)};

        EXPECT_TRUE(
            intVal == 10 && 
            stringVal == "Name" && 
            charVal == 'a'
        );
    }
    // piecewise_construct
    {
        class A {
            int m_Val;
            const char* m_Str;
            char m_Ch;
        public:
            A(int val, const char* str, char ch) : 
                m_Val(val),
                m_Str(str),
                m_Ch(ch) {}
            int GetVal() const {return m_Val;}
            const char* GetStr() const {return m_Str;}
            char GetCh() const {return m_Ch;}
        }; 

        class B {
            int m_Val;
        public:
            B(int a, int b, int c, int d) : m_Val(a + b + c + d) {}
            int GetVal() const {return m_Val;}

        };

        std::pair<A, B> data{
            std::piecewise_construct, // A, B 개체 생성시 tuple의 값을 인수로 전달함
            std::forward_as_tuple(10, "Name", 'a'), // A 생성자에 전달할 인수들을 tuple로 전달
            std::forward_as_tuple(1, 2, 3, 4) // B 생성자에 전달할 인수들을 tuple로 전달
        };

        EXPECT_TRUE(
            data.first.GetVal() == 10 &&
            data.first.GetStr() == "Name" &&
            data.first.GetCh() == 'a' &&
            data.second.GetVal() == 1 + 2 + 3 + 4
        );
    }
    // C++14~ 타입 기반 get()
    {
        using namespace Tuple_1;
        
        // 3개의 데이터로 구성된 tuple 입니다.
        std::tuple<int, std::string, char> result{Func()};

        // 타입들이 서로 다르다면 타입을 이용하여 데이터에 접근할 수 있습니다.
        EXPECT_TRUE(
            std::get<int>(result) == 10 && 
            std::get<std::string>(result) == "Name" &&
            std::get<char>(result) == 'a' 
        );
    }
    // C++17 apply()
    {
        using namespace Tuple_3;

        std::tuple<int, int, int> data{1, 2, 3};

        // tuple의 각 요소를 전개해야 합니다.
        EXPECT_TRUE(Sum(std::get<0>(data), std::get<1>(data), std::get<2>(data)) == 1 + 2 + 3);
    }
#if 201703L <= __cplusplus // C++17~    
    {
        using namespace Tuple_3;

        std::tuple<int, int, int> data{1, 2, 3};

        // tuple의 요소를 전개하지 않아도 됩니다.
        EXPECT_TRUE(std::apply(Sum, data) == 1 + 2 + 3);

    }
#endif    

#if 201703L <= __cplusplus // C++17~
    // (C++17~) make_from_tuple
    {
        class T{
        public:
            T(int a, int b, int c) {}
        };

        std::tuple<int, int, int> data{1, 2, 3};

        // tuple의 요소를 전개하지 않아도 됩니다.
        T t{std::make_from_tuple<T>(std::move(data))};
    }
#endif    
}