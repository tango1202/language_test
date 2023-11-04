#include "gtest/gtest.h" 
#include <algorithm>

namespace Invoke {
    class T {
    public:
        int Sum(int a, int b, int c) {return a + b + c;} // 멤버 함수
    };

    int Sum(int a, int b) {return a + b;} // 일반 함수
}

namespace Function_1 {
    int Sum(int a, int b, int c) {return a + b + c;}

    // 인자들의 값을 1씩 증가시킵니다.
    void Add(int& a, int& b, int& c) {
        ++a;
        ++b;
        ++c;
    }

    template<typename T>
    bool IsBind(const T&) {
        return std::is_bind_expression<T>::value;
    }
    template<typename T>
    bool IsPlaceholder(const T&) {
        return std::is_bind_expression<T>::value;
    }
}

TEST(TestMordern, Function) {
    {
        // 7보다 작은지 검사하는 함수
#if 201103L <= __cplusplus // C++11~
        class Less_7 {
            using argument_type = int;
            using result_type = bool;
#else
        class Less_7 : std::unary_function<int, bool> {
#endif
        public:
            result_type operator ()(argument_type val) {
                return val < 7 ? true : false;
            }       
        };

        std::vector<int> v{7, 4, 3};
        auto itr{std::find_if( // 7보다 작은 첫번째 요소의 이터레이터를 리턴합니다.
            v.begin(), 
            v.end(), 
            Less_7{})
        }; 

        EXPECT_TRUE(*itr == 4);    
    }
    {
        std::vector<int> v{7, 4, 3};
        auto itr{std::find_if( // 7보다 작은 첫번째 요소의 이터레이터를 리턴합니다.
            v.begin(), 
            v.end(), 
            [](int val) -> int {return val < 7 ? true : false;}) 
        };  
        EXPECT_TRUE(*itr == 4);      
    }
    {
        std::function<int(int)> func{[](int val) -> int {return val < 7 ? true : false;}};

        std::vector<int> v{7, 4, 3};
        auto itr{std::find_if(
            v.begin(), 
            v.end(), 
            func) 
        };
        EXPECT_TRUE(*itr == 4);  
    }
    // bad_function_call
    {
    std::function<void()> func{nullptr};

        try {
            func();
        }
        catch (std::bad_function_call&) {
        }
    }
    // mem_fn()
    {
        class T {
        public:
            int Add(int a, int b) const {return a + b;}
        };

        T obj;

        auto add{std::mem_fn(&T::Add)};

        EXPECT_TRUE(add(obj, 1, 2) == 3); // obj.Add(1, 2);를 호출합니다.
    }
    // reference_wrapper
    {
        int a{0};
        std::reference_wrapper<int> rw(a);

        rw.get() = 10; // reference_wrapper가 관리하는 참조자에 값 대입
        EXPECT_TRUE(a == 10);
        
        int& b = rw; // 참조자 생성시 암시적 형변환을 통해 reference_wrapper가 관리하는 참조자 대입
        b = 20;
        EXPECT_TRUE(a == 20);     
    }

    {
        int a{1};
        int b{2};
        int& ref1{a}; // 참조자 타입으로 만드려고 억지로 정의했습니다. ref()나 cref()가 더 낫습니다.
        int& ref2{b};

        // std::vector<int&> v; // (X) 컴파일 오류.
        std::vector<std::reference_wrapper<int>> v;
        v.push_back(ref1); // 참조자를 벡터에 추가할 수 있습니다.
        v.push_back(ref2);

        EXPECT_TRUE(v[0] == 1 && v[1] == 2);
        a = 10;
        b = 20;
        EXPECT_TRUE(v[0] == 10 && v[1] == 20); // 참조자를 저장했으므로 vector의 요소로 확인해도 값이 바껴있습니다.
    }
    {
        int a{1};
        int b{2};

        std::vector<std::reference_wrapper<int>> v;
        v.push_back(std::ref(a)); // std::reference_wrapper를 생성합니다.
        v.push_back(std::ref(b));

        EXPECT_TRUE(v[0] == 1 && v[1] == 2);
        a = 10;
        b = 20;
        EXPECT_TRUE(v[0] == 10 && v[1] == 20); 
    }
    // bind()
    {
        using namespace Function_1;
        // Sum 함수의 인자를 1, 2, 3으로 고정합니다.
        std::function<int()> func1{
            std::bind(Sum, 1, 2, 3)
        };
        EXPECT_TRUE(func1() == 1 + 2 + 3);

        // Sum 함수의 2번째 인자(b)를 함수 호출시의 1번째 인자로 사용합니다.
        std::function<int(int)> func2{
            std::bind(Sum, 1, std::placeholders::_1, 3)
        };
        EXPECT_TRUE(func2(4) == 1 + 4 + 3);

        // Sum 함수의 2번째 인자(b)를 함수 호출시의 1번째 인자로 사용하고,
        // 3번째 인자(c)를 함수 호출시의 2번째 인자로 사용합니다.
        std::function<int(int, int)> func3{
            std::bind(Sum, 1, std::placeholders::_1, std::placeholders::_2)
        };
        EXPECT_TRUE(func3(4, 5) == 1 + 4 + 5);
    }
    {
        using namespace Function_1;
        auto func3{
            std::bind(Sum, 1, std::placeholders::_1, std::placeholders::_2)
        };
        EXPECT_TRUE(func3(4, 5) == 1 + 4 + 5);
    }
    {
        using namespace Function_1;
        int a{1};
        int b{2};
        int c{3};

        std::function<void()> add{
            std::bind(Add, a, b, c) // a, b, c를 복제합니다.
        };
        add();
        // (X) 오동작. bind()에서 인수를 복제했기 때문에 참조성이 깨졌습니다.
        EXPECT_TRUE(a == 1 && b == 2 && c == 3);        
    }
    {
        using namespace Function_1;
        int a{1};
        int b{2};
        int c{3};

        std::function<void()> add{
            std::bind(Add, std::ref(a), std::ref(b), std::ref(c)) // 참조자 래퍼를 대입합니다.
        };
        add();
        // (O) 참조자 래퍼를 대입하여 값이 수정되었습니다.
        EXPECT_TRUE(a == 2 && b == 3 && c == 4);        
    }    
    // is_bind_expression
    {
        using namespace Function_1;
        EXPECT_TRUE(IsBind(std::bind(Sum, 1, 2, 3)) == true);  

        auto func1{std::bind(Sum, 1, 2, 3)}; 
        EXPECT_TRUE(IsBind(func1) == true); 

        std::function<int()> func2{std::bind(Sum, 1, 2, 3)}; // (X) 오동작. function 개체에 저장하면, bind에 대한 관련 속성을 잃어버립니다.
        EXPECT_TRUE(IsBind(func2) == false);        
    }
    // is_placeholder
    {
        using namespace Function_1;
        EXPECT_TRUE(IsPlaceholder(std::bind(Sum, 1, std::placeholders::_1, 3)) == true);  

        auto func1{std::bind(Sum, 1, std::placeholders::_1, 3)}; 
        EXPECT_TRUE(IsPlaceholder(func1) == true); 

        std::function<int(int)> func2{std::bind(Sum, 1, std::placeholders::_1, 3)}; // (X) 오동작. function 개체에 저장하면, bind에 대한 관련 속성을 잃어버립니다.
        EXPECT_TRUE(IsPlaceholder(func2) == false);        
    }    
#if 201703L <= __cplusplus // C++17~
    // C++17 invoke
    {
        using namespace Invoke;
        T t;
        EXPECT_TRUE(std::invoke(T::Sum, t, 1, 2, 3) == 1 + 2 + 3); // 멤버 함수를 호출합니다.

        EXPECT_TRUE(std::invoke(Sum, 1, 2) == 1 + 2); // 일반 함수를 호출합니다.
    }
#endif

}