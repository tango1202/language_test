#include "gtest/gtest.h" 
#include <algorithm>

namespace Lambda_1 {
    std::function<int(int, int)> f_11;
    int g(const std::function<int(int, int)>& lambda_11, int a, int b) { // 함수의 인자로 람다를 지정합니다.
        return lambda_11(a, b);
    }

    class T_11 {
    public: 
        T_11() {std::cout << "T_11::Default Constructor" << std::endl;}
        T_11(const T_11&) {std::cout << "T_11T::Copy Constructor" << std::endl;}
        T_11(T_11&&) {std::cout << "T_11::Move Constructor" << std::endl;}
        ~T_11() {std::cout << "T_11::Destructor" << std::endl;}
        void operator =(const T_11&) {std::cout << "T_11::operator =()" << std::endl;}
    };   
#if 201402L <= __cplusplus // C++14~  
    template<typename T, typename U> 
    auto add_14(T a, U b) { // add_14(T a, T b) 가 아닙니다. 
                            // auto이다 보니 a, b가 같은 타입이라는 보장이 없습니다.
        return a + b;
    } 
#endif
}

namespace Lambda_2 {

    int f_11(int&) {return 1;}
    int f_11(int&&) {return 2;}
}
namespace Lambda_3 {
    extern int AddDeclare(int, int); // 함수 선언
    int AddDefine(int a, int b) {return a + b;} // 함수 정의
}
namespace Lambda_4 {
    int Sum(int a, int b, int c) {return a + b + c;}
    void Add(int& result, int a, int b, int c) {result += a + b + c;}
    
#if 202002L <= __cplusplus // C++20~            
    template <typename... Params>
    auto Sum_20(Params&&... params) { // 전달 참조입니다. 정수형 상수는 int&& 로 전달받습니다. 
        // 값 캡쳐
        // 값 캡쳐는 복제본을 만들어 const로 사용하므로
        // lambdaParams는 const int로 대입받습니다. 
        return [...lambdaParams = std::forward<Params>(params)] { 
                                                                  
            // lambdaParams은 const int여서
            // forward<Params>로 사용하면, forward<int&&>이므로 상수성 계약 위반입니다.
            // 따라서 forward<const Params>으로 전달해야 합니다.
            return Sum(std::forward<const Params>(lambdaParams)...); 
        }(); // 클로저를 실행합니다.
    }

    template <typename... Params>
    void Add_20(Params&&... params) { // 전달 참조입니다. 이름이 있는 좌측값은 int&, 정수형 상수는 int&& 로 전달받습니다.

        // 참조 캡쳐이기 때문에 참조성이 더해집니다.
        // 즉 params[1] == int& 일때, lambdaParams[1] == int& + & == int&& 입니다.
        // 따라서, int& 를 int&&에 바인딩할 수 없다는 컴파일 오류가 발생합니다.
        // [&...lambdaParams = std::forward<Params>(params)] { // (X) 컴파일 오류. 

        // 참조 캡쳐에서 참조성을 더해서 대입받는 문제를 우회하기 위해
        // forward의 T에 Params& 로 참조성을 억지로 추가하여 형변환 합니다.
        // ** 사례 분석 : params[1]이 int& 인 경우 **
        //    1. lambdaParams[1] == int& + 참조 캡쳐에 의한 & == int&& 입니다.
        //    2. params[1]은 좌측값 참조이므로, 좌측값 참조를 인자로 받는 forward() 함수가 호출됩니다. 
        //    3. forward 함수에 전달되는 템플릿 인자는 T == params[1] + & == int& + & == int&& 가 전달되어 전개됩니다.
        //    4. forward 함수는 static_cast<T&&>를 리턴하므로 static_cast<int&& &&> 를 리턴하므로 참조 축약에 의해 int&&을 리턴합니다.
        //    5. lambdaParam[1] 은 int&&을 참조하는 좌측값이 입니다.
        // ** 사례 분석 : params[2]가 int&& 인 경우 **
        //    1. lambdaParams[2] == int&& + 참조 캡쳐에 의한 & == int& 입니다.
        //    2. params[1]은 int&&를 참조하지만, 이름을 부여받았으므로 좌측값 참조를 인자로 받는 forward() 함수가 호출됩니다. 
        //    3. forward 함수에 전달되는 템플릿 인자는 T == params[2] + & == int&& + & == int& 가 전달되어 전개됩니다.
        //    4. forward 함수는 static_cast<T&&>를 리턴하므로 static_cast<int& &&> 를 리턴하므로 참조 축약에 의해 int&을 리턴합니다.
        //    5. lambdaParams[2] 는 int&인 좌측값 참조입니다.
        // 즉, 좌측값 참조와 우측값 참조가 바뀝니다.
        [&...lambdaParams = std::forward<Params&>(params)] { 

            // 애초에 캡쳐할때의 Params로 forward()를 호출합니다.
            // ** 사례 분석 : params[1]이 int& 이고 lambdaParams[1]이 int&&인 경우 **
            //    1. lambdaParams[1]이 int&&를 참조하지만, 이름을 부여받았으므로, 좌측값 참조를 인자로 받는 forward() 함수가 호출됩니다. 
            //    2. forward 함수에 전달되는 템플릿 인자는 T == params[1] == int& 가 전달되어 전개됩니다.
            //    3. forward 함수는 static_cast<T&&>를 리턴하므로 static_cast<int& &&> 를 리턴하므로 참조 축약에 의해 int&을 리턴합니다.
            //    4. Add() 함수에 int&로 전달합니다.
            // ** 사례 분석 : params[2]가 int&& 이고 lambdaParams[2]가 int& 인 경우 **
            //    1. lambdaParams[1]은 좌측값 참조이므로, 좌측값 참조를 인자로 받는 forward() 함수가 호출됩니다.
            //    2. forward 함수에 전달되는 템플릿 인자는 T == params[2] == int&& 가 전달되어 전개됩니다.
            //    3. forward 함수는 static_cast<T&&>를 리턴하므로 static_cast<int&& &&> 를 리턴하므로 참조 축약에 의해 int&&을 리턴합니다.
            //    4. Add() 함수에 int&&로 전달합니다.
            // 즉, lambdaParams가 좌측값 참조와 우측값 참조를 바꿔서 저장하고, 다시 Add() 함수에 원복해서 전달합니다.
            Add(std::forward<Params>(lambdaParams)...); 
        }(); // 클로저를 실행합니다
    }

#endif      
}
TEST(TestMordern, Lambda) {
    {
        auto f_11{
            [](int a, int b) -> int {return a + b;}
        };
        int val{f_11(10, 20)}; // 함수 호출하듯이 f_11(10, 20) 로 호출합니다.
        EXPECT_TRUE(val == 30);
    }
    {
        int val_11{
            [](int a, int b) -> int {return a + b;}(10, 20) // 람다 표현식에 (10, 20)을 붙여 호출합니다.
        }; 
        EXPECT_TRUE(val_11 == 30);
    }
    // 람다 캡쳐
    {
        int sum{0};
        std::vector<int> v_11{1, 2, 3};

        std::for_each( // 시퀀스 안의 요소들에 대해 f(요소)를 실행합니다. f는 람다 표현식입니다.
            v_11.begin(), 
            v_11.end(),
            [&sum](int val) {sum += val;} // 람다 캡쳐된 sum에 시퀀스 요소의 값(val)을 누적합니다.
        );

        EXPECT_TRUE(sum == 1 + 2 + 3);
    }
    // 람다 캡쳐 시기
    {
        int val{1};
        auto f_11{
            [=]() -> int {return val;} // 클로저가 생성되는 시점에 val을 람다 캡쳐합니다.
        }; 
        val = 2;

        EXPECT_TRUE(f_11() == 1); // 람다 캡쳐할 때의 값을 사용하므로 1입니다.        
    }
    // 람다 캡쳐 개체의 수정
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
        [=]() mutable { // 값캡쳐했지만 수정할 수 있습니다.
            a = 10; // (△) 비권장. 복제본이 수정될 뿐, 원본이 수정되는 건 아닙니다.
            *ptr = 20;
            ref = 30; // (△) 비권장. 복제본이 수정될 뿐, 원본이 수정되는 건 아닙니다.
        }();

        EXPECT_TRUE( a == 1 && b == 20 && c == 3);     
    }
    // 개체 멤버 함수 람다 캡쳐
    {
        class T_11 {
            int m_Member{1};
        public:
            int GetMember() const {return m_Member;}
            int Func() {
                int local{2};

                auto f_11{

#if 202002L <= __cplusplus // C++20~ 
                    [=, this]() -> int {
#else
                    [=]() -> int {
#endif
                        m_Member = 10; // this->m_Member = 10; 과 동일. 멤버 변수의 값을 수정합니다.
                        return m_Member + local;
                    }
                };  

                return f_11();
            }
        };

        T_11 t;
        EXPECT_TRUE(t.Func() == 12);
        EXPECT_TRUE(t.GetMember() == 10); // 멤버 변수가 수정되어 있습니다.
    }
    {
        class T_11 {
            int m_Member{1};
        public:
            int GetMember() const {return m_Member;}
            int Func() {
                int local{2};

                auto f_11{
                    [&]() -> int { // C++20에서도 여전히 this를 참조로 캡쳐합니다.

                        m_Member = 10; // this->m_Member = 10; 과 동일. 멤버 변수의 값을 수정합니다.
                        return m_Member + local;
                    }
                };  

                return f_11();
            }
        };  
        T_11 t;
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
        [&]() { // 람다 캡쳐한 개체를 수정할 있습니다.
            a = 10; 
            *ptr = 20;
            ref = 30;
        }();

        EXPECT_TRUE( a == 10 && b == 20 && c == 30);     
    }
    // 람다로의 개체 이동
    {
        class A_11 {
        public:
            A_11() {}
            A_11(const A_11& other) = delete;
            A_11(A_11&& other) {
                std::cout << "A : Lambda Move" << std::endl;
            }
            A_11& operator =(const A_11& other) = delete;
            A_11& operator =(A_11&& other) = delete; 
        };

        A_11 a; // 기본 생성과 이동 생성만 가능한 개체입니다.

        // [a]() {a;}(); // (X) 컴파일 오류. 복사 생성자가 delete 되었습니다.
        [&a]() {a;}(); // (O) 참조로 사용해서 컴파일 오류는 없습니다. 그러나 이동된 것은 아니죠.

        auto func_11 { // #1. bind 개체를 저장합니다. func_11() 형태로 호출할 수 있습니다.
            std::bind( // #2. bind 개체를 생성합니다. lambda_11(우측값 참조로 생성한 개체) 를 호출하는 함수자를 만듭니다. 이시점에 이동 생성자가 호출됩니다. 
                [](const A_11& param) {param;}, // #3. 이동 생성자로부터 생성된 개체(#4에서 생성된 개체)를 참조합니다. 
                std::move(a) // #4. 우측값 참조. bind 내에서 이동 생성자로부터 생성된 개체가 만들어 집니다.
            )
        };
        func_11(); // #5. 함수를 호출합니다.
#if 201402L <= __cplusplus // C++14~  
        [&a]() {a;}(); // (O) 참조로 사용해서 컴파일 오류는 없습니다. 그러나 이동된 것은 아니죠.

        auto f_14{
            [b = std::move(a)](){b;} // a를 이동 생성한 b를 사용합니다.
        }; 
        f_14();
#endif
        
    }

    // 클로저 저장
    {
        auto f_11{
            [](int a, int b) -> int {return a + b;}
        };
        EXPECT_TRUE(f_11(10, 20) == 30);       
    }
    {
        typedef int (*Func)(int, int); // 함수 포인터 typedef
        Func f_11{
            [](int a, int b) -> int {return a + b;}
        };
        EXPECT_TRUE(f_11(10, 20) == 30);        
    }
    {
        using namespace Lambda_1;

        int c{30};
        f_11 = [=](int a, int b) -> int {return a + b + c;}; // 람다 캡쳐를 사용하는 람다 표현식도 사용할 수 있습니다.      
        EXPECT_TRUE(g(f_11, 10, 20) == 60); // g() 함수에 클로저를 저장한 f_11을 전달합니다.       
    }
    // 클로저 복사 부하
    {
        using namespace Lambda_1;
        
        T_11 t;
        [=]() {std::cout << "Run Lambda" << std::endl;}(); // t를 사용하지 않았습니다.        
    }
    {
        using namespace Lambda_1;
        
        T_11 t;
        [=]() {
            t; // t;로 사용합니다. 람다 캡쳐시 복사 부하가 있습니다.    
            std::cout << "Run Lambda" << std::endl;
        }();    
    }
    {
        using namespace Lambda_1;

        T_11 t;
        auto f_11{
            [=]() { // 변수에 저장하고, 호출합니다.   
                t; 
                std::cout << "Run Lambda" << std::endl;
            }
        };      
        f_11();
    }
    {
        using namespace Lambda_1;

        T_11 t;
        auto f1_11{
            [=]() {
                t; 
                std::cout << "Run Lambda" << std::endl;
            }
        }; 
        auto f2_11{f1_11}; // 클로저를 복제합니다. 복사 부하가 발생합니다.
        f1_11();
        f2_11();
    }

    {
        using namespace Lambda_1;

        T_11 t;
        auto f1_11{
            [&]() {
                t;
                std::cout << "Run Lambda" << std::endl;
            }
        }; // 람다 캡쳐시 복사 부하가 없습니다.
        auto f2_11{f1_11}; // 대입시 복사 부하가 없습니다.
        f1_11();
        f2_11();
    }
#if 201402L <= __cplusplus // C++14~  
    // C++14 람다 캡쳐 초기화
    {
        // 클로저 생성시 람다 내에서 사용할 수 있는 val_14 변수를 만들어 람다 캡쳐 합니다.
        // 람다 캡쳐된 val_14 을 수정하기 위해 mutable로 만듭니다.
        auto f_14{
            [val_14 = 0]() mutable -> int {return ++val_14;}
        }; 

        // 호출시마다  람다 캡쳐된 val이 증가합니다.
        EXPECT_TRUE(f_14() == 1);
        EXPECT_TRUE(f_14() == 2);
        EXPECT_TRUE(f_14() == 3);
    } 
#endif     
#if 201402L <= __cplusplus // C++14~          
    // C++14~ 일반화된 람다 표현식
    {
        auto add_14{
            [](auto a, auto b) {return a + b;}
        }; 

        EXPECT_TRUE(add_14(1, 2) == 3);
        EXPECT_TRUE(add_14(std::string{"hello"}, std::string{"world"}) == "helloworld");
    }
    {
        using namespace Lambda_1;

        EXPECT_TRUE(add_14(1, 2) == 3);
        EXPECT_TRUE(add_14(std::string{"hello"}, std::string{"world"}) == "helloworld");
    }
    {
        auto add_14{
            // b는 a와 같은 타입입니다.
            [](auto a, decltype(a) b) {return a + b;}
        }; 

        EXPECT_TRUE(add_14(1, 2) == 3);
        EXPECT_TRUE(add_14(std::string{"hello"}, std::string{"world"}) == "helloworld");        
    }

    {

        using namespace Lambda_2;

        // auto Forwarding_14{
        //     [](auto&& param) {
        //         return f_11(std::forward<??>(param)); // (X) 컴파일 오류. 템플릿이라면 템플릿 인자 T를 전달하면 되는데, ??에 무얼 전달해야 할까요? 
        //     }
        // };    
        auto Forwarding_14{
            [](auto&& param) {
                // 전달 참조일때 decltype(param)은 
                // param이 좌측값 참조이면 T&, param이 우측값 참조이면 T&& 입니다.
                return f_11(std::forward<decltype(param)>(param)); 
            }
        }; 

        int val;
        int& ref = val;
        EXPECT_TRUE(Forwarding_14(val) == 1); // 전달 참조에서 값 타입은 좌측값 참조입니다.
        EXPECT_TRUE(Forwarding_14(ref) == 1); // 전달 참조에서 좌측값 참조는 그대로 좌측값 참조입니다.
        EXPECT_TRUE(Forwarding_14(std::move(val)) == 2); // 전달 참조에서 우측값 참조는 그대로 우측값 참조입니다.       
    }

#endif    
#if 201703L <= __cplusplus // C++17~
    // (C++17~) constexpr 람다 표현식
    {
        // 명시적 constexpr 람다 표현식 입니다.
        auto id_17{
            [](int n) constexpr {return n;}
        };
        static_assert(id_17(10) == 10);

        // 암시적 constexpr 람다 표현식 입니다.
        auto add_17{
            [](int a, int b) {return a + b;}
        };
        static_assert(add_17(1, 2) == 3);

        // 인자로 전달한 a_11, b_11는 컴파일 타임 상수이기 때문에 요구사항이 맞아 암시적 constexpr 람다 표현식 입니다.
        constexpr int a_11{1};
        constexpr int b_11{2};
        static_assert(add_17(a_11, b_11) == 3);

        // 인자로 전달한 c, d는 런타임 변수이기 때문에 요구사항이 맞지 않아 런타임 람다 표현식 입니다.
        int c{1};
        int d{2};
        EXPECT_TRUE(add_17(c, d) == 3);
    }
#endif  
    // 람다 표현식에서 템플릿 인자
    {
        auto add_11 {
            [](int a, int b) {return a +b;} // 람다 표현식
        };
        EXPECT_TRUE(add_11(1, 2) == 3);
#if 201402L <= __cplusplus // C++14~          
        auto add_14{
            [](auto a, decltype(a) b) {return a + b;} // 일반화된 람다 표현식
        }; 
        EXPECT_TRUE(add_14(1, 2) == 3);
#endif        
#if 202002L <= __cplusplus // C++20~         
        auto add_20{
            []<typename T>(T a, T b) {return a + b;} // 람다 표현식에서 템플릿 인자 지원
        };
        EXPECT_TRUE(add_20(1, 2) == 3);      
#endif        
    }
    // 람다 캡쳐에서 파라메터 팩
    {
        using namespace Lambda_4;
#if 202002L <= __cplusplus // C++20~   
        EXPECT_TRUE(Sum_20(1, 2, 3) == 1 + 2 + 3);

        int result = 10;
        Add_20(result, 1, 2, 3);
        EXPECT_TRUE(result == 10 + 1 + 2 + 3); // result 값이 잘 수정되어 있습니다.
#endif        
    }
#if 202002L <= __cplusplus // C++20~       
    // (C++20~) 람다 캡쳐에서 구조화된 바인딩 지원
    {
        class A_11 {
        public:
            int m_X{1};
            int m_Y{2};
        };

        const auto& [x_17, y_17]{A_11{}}; // const auto&로 받아서 임시 개체인 A_11의 수명이 연장됩니다.
       
        auto lambda_20{
            [x_17, y_17] {return x_17 + y_17;} // 구조화된 바인딩을 람다 캡쳐합니다.
        };
        EXPECT_TRUE(lambda_20() == 1 + 2);
    }  
#endif     
    // 상태없는 람다 표현식의 기본 생성과 복사 대입
    {
        auto lambda_11{
            [](int left, int right) {return left > right;} // 큰수에서 작은 수로 정렬합니다.
        };

        decltype(lambda_11) a_11{lambda_11}; // 복사 생성은 가능합니다.
        // decltype(lambda_11) b_11; // (X) 컴파일 오류. 기본 생성은 불가능합니다.
        // a_11 = lambda_11; // (X) 컴파일 오류. 복사 대입은 불가능합니다.      
    }
#if 202002L <= __cplusplus // C++20~      
    {
        auto lambda_11{
            [](int left, int right) {return left > right;} // 캡쳐를 사용하지 않습니다.
        };

        decltype(lambda_11) a_20{lambda_11}; // 복사 생성은 가능합니다.
        decltype(lambda_11) b_20; // (O) 기본 생성이 가능합니다.
        a_20 = lambda_11; // (O) 복사 대입이 가능합니다. 
    }
    {
        auto lambda_11{
            [=](int left, int right) {return left > right;} // 캡쳐를 사용합니다.
        };

        decltype(lambda_11) a_20{lambda_11}; // 복사 생성은 가능합니다.
        // decltype(lambda_11) b_20; // (O) 기본 생성은 불가능합니다.
        // a_20 = lambda_11; // (X) 컴파일 오류. 복사 대입은 불가능합니다.
    }    
#endif 
    {
        auto lambda_11{
            [](int left, int right) {return left > right;} 
        };

        std::set<int, decltype(lambda_11)> data_11(lambda_11); // compare 개체를 전달해야 합니다.
        // std::set<int, decltype(lambda_11)> data_11{1, 2, 3}; // (X) 컴파일 오류. compare개체가 전달되지 않았습니다.
        data_11.insert(1);
        data_11.insert(2);
        data_11.insert(3);
        for (auto i : data_11) {
            std::cout << "set : " << i << std::endl; // 3 2 1 의 순서로 출력합니다.
        }
    }
#if 202002L <= __cplusplus // C++20~  
    {
        auto lambda_11{
            [](int left, int right) {return left > right;} 
        };
        
        std::set<int, decltype(lambda_11)> data_20; // 상태없는 람다 표현식은 기본 생성하므로 compare 개체를 전달할 필요가 없습니다.
        data_20.insert(1);
        data_20.insert(2);
        data_20.insert(3);
        for (auto i : data_20) {
            std::cout << "set : " << i << std::endl;
        }
    }
    {
        auto lambda_11{
            [](int left, int right) {return left > right;} 
        };

        std::set<int, decltype(lambda_11)> data_20{1, 2, 3}; // initializer_list로 초기화 할 수 있습니다.
        for (auto i : data_20) {
            std::cout << "set : " << i << std::endl;
        }
    }
#endif

    // 미평가 표현식에서의 람다 표현식 허용
    {
        using namespace Lambda_3;

        decltype(*AddDeclare) AddFunc_11{AddDefine}; // AddDeclare함수 선언으로부터 변수 AddFunc_11을 정의하고 
                                                     // AddDefine으로 초기화합니다. 
        EXPECT_TRUE(AddFunc_11(10, 20) == 30); // 함수 실행
    }
#if 202002L <= __cplusplus // C++20~  
    // 미평가 표현식에서의 람다 표현식 허용
    {
        std::set<
            int, 
            decltype(
                [](int left, int right) {return left > right;} // 미평가 표현식에서 람다 표현식을 사용할 수 있습니다.
            )
        > data_20{1, 2, 3}; // initializer_list로 초기화 할 수 있습니다.
        for (auto i : data_20) {
            std::cout << "set : " << i << std::endl; // 3 2 1 의 순서로 출력합니다.
        }            
    }
#endif        



}




