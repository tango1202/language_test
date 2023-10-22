#include "gtest/gtest.h" 
#include <algorithm>

namespace {
    std::function<int(int, int)> f_11;
    int g(const std::function<int(int, int)>& lambda_11, int a, int b) { // 함수의 인자로 람다를 지정합니다.
        return lambda_11(a, b);
    }

    class T {
    public: 
        T() {std::cout << "T::Default Constructor" << std::endl;}
        T(const T&) {std::cout << "T::Copy Constructor" << std::endl;}
        T(T&&) {std::cout << "T::Move Constructor" << std::endl;}
        ~T() {std::cout << "T::Destructor" << std::endl;}
        void operator =(const T&) {std::cout << "T::operator =()" << std::endl;}
    };   
}
TEST(TestMordern, Lambda) {
    {
        auto f_11{[](int a, int b) -> int {return a + b;}};
        int val{f_11(10, 20)}; // 함수 호출하듯이 f_11(10, 20) 로 호출합니다.
        EXPECT_TRUE(val == 30);
    }
    {
        int val_11{[](int a, int b) -> int {return a + b;}(10, 20)}; // 클로저 개체에 (10, 20)을 붙여 호출합니다.
        EXPECT_TRUE(val_11 == 30);
    }
    // 캡처
    {
        int sum{0};
        std::vector<int> v_11{1, 2, 3};

        std::for_each( // 시퀀스 안의 요소들에 대해 f(요소)를 실행합니다. f는 람다 함수입니다.
            v_11.begin(), 
            v_11.end(),
            [&sum](int val) {sum += val;} // 캡처된 sum에 시퀀스 요소의 값(val)을 누적합니다.
        );

        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    // 캡쳐 시기
    {
        int val{1};
        auto f_11{[=]() -> int {return val;}}; // 클로저 개체가 생성되는 시점에 val을 캡쳐합니다.
        val = 2;

        EXPECT_TRUE(f_11() == 1); // 캡쳐할 때의 값을 사용하므로 1입니다.        
    }
    // 캡쳐 개체의 수정
    {
        int a{1};
        int b{2};
        int c{3};
        int* ptr{&b};
        int& ref{c};
        [=]() { // 값으로 복제하며 `const` 입니다.
            // a = 10; // (X) 컴파일 오류. const 이므로 수정할 수 없습니다.
            // ptr = &a; // (X) 컴파일 오류. const 이므로 수정할 수 없습니다.
            *ptr = 20; // int* const 여서 ptr 은 수정할 수 없지만, ptr이 가리키는 개체는 수정할 수 있습니다.
            // ref = 30; // (X) 컴파일 오류. const 이므로 수정할 수 없습니다.
        }();

        EXPECT_TRUE(b == 20);         
    }
    // mutable
    {
        int a{1};
        int b{2};
        int c{3};
        int* ptr{&b};
        int& ref{c};
        [=]() mutable { // 값으로 캡쳐했지만 수정할 수 있습니다.
            a = 10; // (△) 비권장. 복제본이 수정될 뿐, 원본이 수정되는 건 아닙니다.
            *ptr = 20;
            ref = 30; // (△) 비권장. 복제본이 수정될 뿐, 원본이 수정되는 건 아닙니다.
        }();

        EXPECT_TRUE( a == 1 && b == 20 && c == 3);     
    }
    // 개체 멤버 함수 캡쳐
    {
        class T {
            int m_Member{1};
        public:
            int GetMember() const {return m_Member;}
            int Func() {
                int local{2};

                auto f_11{[=]() -> int {
                    // this->m_Member = 10; 
                    m_Member = 10; // this->m_Member = 10; 과 동일. 멤버 변수의 값을 수정합니다.
                    return m_Member + local;
                }};  

                return f_11();
            }
        };

        T t;
        EXPECT_TRUE(t.Func() == 12);
        EXPECT_TRUE(t.GetMember() == 10); // 멤버 변수가 수정되어 있습니다.
    }
    // 참조 캡쳐
    {
        int a{1};
        int b{2};
        int c{3};
        int* ptr{&b};
        int& ref{c};
        [&]() { // 캡쳐한 개체를 수정할 있습니다.
            a = 10; 
            *ptr = 20;
            ref = 30;
        }();

        EXPECT_TRUE( a == 10 && b == 20 && c == 30);     
    }

    // 클로저 개체 저장
    {
        auto f_11{[](int a, int b) -> int {return a + b;}};
        EXPECT_TRUE(f_11(10, 20) == 30);       
    }
    {
        typedef int (*Func)(int, int); // 함수 포인터 typedef
        Func f_11{[](int a, int b) -> int {return a + b;}};
        EXPECT_TRUE(f_11(10, 20) == 30);        
    }
    {
        int c{30};
        f_11 = [=](int a, int b) -> int {return a + b + c;}; // 캡쳐를 사용하는 람다 표현식도 사용할 수 있습니다.      
        EXPECT_TRUE(g(f_11, 10, 20) == 60); // g() 함수에 클로저 개체를 저장한 f_11을 전달합니다.       
    }
    // 클로저 개체 복사 부하
    {
        T t;
        [=]() {std::cout << "Run Lambda" << std::endl;}(); // t를 사용하지 않았습니다.        
    }
    {
        T t;
        [=]() {
            t; // t;로 사용합니다. 캡쳐시 복사 부하가 있습니다.    
            std::cout << "Run Lambda" << std::endl;
        }();    
    }
    {
        T t;
        auto f_11{[=]() { // auto 변수에 저장하고, 호출합니다.   
            t; 
            std::cout << "Run Lambda" << std::endl;
        }};      
        f_11();
    }
    {
        T t;
        auto f1_11{[=]() {
            t; 
            std::cout << "Run Lambda" << std::endl;
        }}; 
        auto f2_11{f1_11}; // 복사 부하가 발생합니다.
        f1_11();
        f2_11();
    }

    {
        T t;
        auto f1_11{[&]() {t; std::cout << "Run Lambda" << std::endl;}}; // 캡쳐시 복사 부하가 없습니다.
        auto f2_11{f1_11}; // 대입시 복사 부하가 없습니다.
        f1_11();
        f2_11();
    }
#if 201402L <= __cplusplus // C++14~  
    // C++14 람다 캡쳐 초기화
    {
        // 클로저 개체 생성시 람다 내에서 사용할 수 있는 val_14 변수를 만들어 캡쳐 합니다.
        // 캡쳐된 val_14 을 수정하기 위해 mutable로 만듭니다.
        auto f_14{[val_14 = 0]() mutable -> int {return ++val_14;}}; 

        // 호출시마다  캡쳐된 val이 증가합니다.
        EXPECT_TRUE(f_14() == 1);
        EXPECT_TRUE(f_14() == 2);
        EXPECT_TRUE(f_14() == 3);
    } 
#endif     
#if 201402L <= __cplusplus // C++14~          
    // C++14~ 일반화된 람다 함수
    {
        auto add_14{[](auto a, auto b) {return a + b;}}; 

        EXPECT_TRUE(add_14(1, 2) == 3);
        EXPECT_TRUE(add_14(std::string{"hello"}, std::string{"world"}) == "helloworld");
    }
#endif    
#if 201703L <= __cplusplus // C++17~
    // (C++17~) constexpr 람다 함수
    {
        // 명시적 constexpr 람다 함수 입니다.
        auto id_17{[](int n) constexpr {return n;}};
        static_assert(id_17(10) == 10);

        // 암시적 constexpr 람다 함수 입니다.
        auto add_17{[](int a, int b) {return a + b;}};
        static_assert(add_17(1, 2) == 3);

        // 인자로 전달한 a_11, b_11는 컴파일 타임 상수이기 때문에 요구사항이 맞아 암시적 constexpr 람다 함수 입니다.
        constexpr int a_11{1};
        constexpr int b_11{2};
        static_assert(add_17(a_11, b_11) == 3);

        // 인자로 전달한 c, d는 변수이기 때문에 요구사항이 맞지 않아 런타임 람다 함수 입니다.
        int c{1};
        int d{2};
        EXPECT_TRUE(add_17(c, d) == 3);
    }
#endif    

}




