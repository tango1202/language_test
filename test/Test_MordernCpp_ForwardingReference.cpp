#include "gtest/gtest.h" 

namespace ForwardingReference_1 {
    template<typename U, typename V, typename W>
    void f(U, V, W) {}
}
namespace ForwardingReference_2 {
    class A {};

    int f_11(A&) {return 1;} // 인자가 좌측값 참조이면 호출됩니다.
    int f_11(A&&) {return 2;} // 인자가 우측값 참조이면 호출됩니다.

    int Forwarding_11(A&& param) { // 함수 인자는 이름이 부여됐으므로 좌측값입니다.
        return f_11(param); // 좌측값으로만 호출합니다. 이동 연산이 물거품이 될 수 있습니다.
    }    
}
namespace ForwardingReference_3 {
    template<typename T>
    void f(T) {}
}
namespace ForwardingReference_4 {
    template<typename T>
    class Convert_11 {
    public:
        using LRef = T&; // T 타입에 좌측값 참조를 더합니다.
        using RRef = T&&; // T 타입에 우측값 참조를 더합니다.
    };  
}
namespace ForwardingReference_5 {
    template<typename T>
    void f_11(T&& val) {} // 전달 참조입니다. val 타입으로 추론합니다.

    template<typename T> 
    class A {
    public:
        void f_11(T&& val) {} // 전달 참조가 아닙니다. A<int> val; 와 같이 인스턴스화 된 뒤에는 f(int&& val)로 구체화될 수 있기 때문에 val 타입으로 추론된다고 확신할 수 없습니다. 

        template<typename U> 
        void g_11(U&& val) {} // 전달 참조 입니다. val 타입으로 추론합니다.
    }; 
}
namespace ForwardingReference_6 {
    template<typename T>
    void f_11(T&& param1, T&& param2) {} // 전달 참조가 아닙니다. param1, param2중 어느 하나가 추론된 뒤에는 나머지는 추론된 것으로 부터 구체화 될 수 있기 때문입니다. 
}
namespace ForwardingReference_7 {
    template<typename T, typename U>
    void f_11(T&& param1, U&& param2) {} // param1, param2 모두 전달 참조입니다.  
}
namespace ForwardingReference_8 {
    template<typename T>
    struct remove_reference_11 { 
        using type = T; 
    };
    // 좌측값 참조인 경우 템플릿 부분 특수화. 좌측값 참조를 제거한 T를 type으로 사용합니다.
    template<typename T>
    struct remove_reference_11<T&> { 
        using type = T; 
    };
    // 우측값 참조인 경우 탬플릿 부분 특수화. 우측값 참조를 제거한 T를 type으로 사용합니다.
    template<typename T>
    struct remove_reference_11<T&&> { 
        using type = T; 
    };

    template<typename T>
    typename std::remove_reference<T>::type&& move(T&& param) {
        using ReturnType = typename std::remove_reference<T>::type&&; // 참조성을 뗀 뒤 &&을 더합니다.

        return static_cast<ReturnType>(param);
    }  
}

namespace ForwardingReference_10 {
    // class A {};
    // void f_11(A param1, A& param2, A&& param3) {}

    // void Forwarding_11(A param1, A& param2, A&& param3) {
    //     f_11(
    //         param1, // (△) 비권장. A로 전달합니다. 복사 대입합니다.
    //         param2, // A&로 전달합니다.
    //         // param3 // (X) 컴파일 오류. param3은 우측값을 참조하는 좌측값입니다. A&로 전달합니다.
    //     );
    // }  

    class A {};
    void f_11(A param1, A& param2, A&& param3) {}

    void Forwarding_11(A param1, A& param2, A&& param3) {
        f_11(
            std::forward<A>(param1), // A&&로 전달합니다. param1은 Forwarding_11 함수에서 임시로 복사 대입 받은 것이므로, 임시 개체로 취급하고, f_11에 이동 대입한 후 버립니다.
            std::forward<A&>(param2), // A&로 전달합니다.
            std::forward<A&&>(param3) // A&&로 전달합니다. std::forward<A>(param3) 도 동일합니다.
        );
    } 
}
namespace ForwardingReference_11 {
    class A {};
    void f_11(A param1, A& param2, A&& param3) {}

    template<typename U, typename V, typename W>
    void Forwarding_11(U&& param1, V&& param2, W&& param3) {
        f_11(
            std::forward<U>(param1), // A&로 전달합니다. 
            std::forward<V>(param2), // A&로 전달합니다.
            std::forward<W>(param3) // A&&로 전달합니다.
        );
    }
}

namespace ForwardingReference_12 {
    class A {};
    void f(A val) {}
    void Forwarding_11(A param) {
        f(std::forward<A>(param));
    }
}

namespace ForwardingReference_13 {

    class A {};
    void f(A& val) {}
    void Forwarding_11(A& param) {
        f(std::forward<A&>(param));
    }
}

namespace ForwardingReference_14 {

    class A {};
    void f(A&& val) {}
    void Forwarding_11(A&& param) {
        f(std::forward<A&&>(param)); // param은 우측값을 참조하는 좌측값입니다.
    }
}
namespace ForwardingReference_15 {

    class A {};
    void f(A&& val) {}
    void Forwarding_11(A& param) {
        f(std::forward<A&&>(std::move(param))); // move() 함수로 우측값으로 형변환 합니다.
    }
}
namespace ForwardingReference_16 {
    class A {};
    void f(A param) {}
    void g(A& param) {}
    void h_11(A&& param) {}

    template<typename U, typename V, typename W>
    void Forwarding_11(U&& param1, V&& param2, W&& param3) {

        // 전달 참조는 이름이 있는 값 타입을 좌측값 참조로 받습니다.
        // 따라서, U == A&, param1 == A& 이며,
        // forward<U>는 T == A&로 템플릿 인스턴스화 합니다.
        // template<typename T> 
        // A& + && forward(A + & param) {
        //    return static_cast<A& + &&>(param);
        // }
        // 즉, A&를 A&로 형변환 합니다.
        f(std::forward<U>(param1));

        // 전달 참조는 좌측값 참조 타입을 좌측값 참조로 받습니다.
        // 따라서, V == A&, param1 == A& 이며,
        // forward<V>는 T == A&로 템플릿 인스턴스화 합니다.
        // template<typename T> 
        // A& + && forward(A + & param) {
        //    return static_cast<A& + &&>(param);
        // }
        // 즉, A&를 A&로 형변환 합니다.
        g(std::forward<V>(param2));

        // 전달 참조는 우측값 참조 타입을 우측값 참조로 받습니다.
        // 또는 이름이 없는 값 타입인 임시 개체도 우측값 참조로 받습니다.
        // 따라서, W == A&&, param3 == A& 이며,(param3은 이름이 부여됐으므로 좌측값입니다.)
        // forward<W>는 T == A&&로 템플릿 인스턴스화 합니다.
        // template<typename T> 
        // A&& + && forward(A + & param) {
        //    return static_cast<A&& + &&>(param);
        // }
        // 즉, A&를 A&&로 형변환 합니다.
        h_11(std::forward<W>(param3));
    }
}
namespace ForwardingReference_17 {
    class A {};
    void f(A val) {}

    // #1. 전달 참조 버전
    template<typename U>
    void Forwarding1_11(U&& param) { // 좌측값 참조를 받아서
        f(std::forward<U>(param));  // val이 값 타입 이기 때문에 임시 개체를 생성하고 복사합니다.
    }
    // #2. 일반 버전
    void Forwarding2_11(A param) { // param을 복사 생성하고
        f(std::forward<A>(param)); // 우측값으로 val에 이동시킵니다.
    }
}

namespace ForwardingReference_18 {

    class A_11 {
        std::string m_String;
    public:
        template<typename T>
        void SetString_11(T&& param) { // 전달 참조
            // const T& 를 전달받으면 const T& 전달
            // T&& 를 전달받으면 T&& 전달
            // const char* 를 전달받으면 cosnt char* 전달
            m_String = std::forward<T>(param); 
        }
    }; 
}
namespace ForwardingReference_18_1 {

    class A_11 {
    public:
        template<typename T>
        int Func_11(T&& param) {return 1;} // 어지간 하면, 다 전달 참조 버전이 호출됩니다.
        int Func_11(int) {return 2;}
    }; 
}

namespace ForwardingReference_18_2 {
    class A_11 {
    public:
        template<typename T>
        int Func_11(T&& param) {
            return FuncInternal_11(
                std::forward<T>(param),
                std::is_integral<typename std::remove_reference<T>::type>() // 정수 타입이면 true_type, 아니면 false_type
            );
        }

        template<typename T> 
        int FuncInternal_11(T&& param, std::false_type) {return 1;} // 정수 외 계열이 호출합니다.

        int FuncInternal_11(int, std::true_type) {return 2;} // 정수 계열이 호출합니다.
    };
}
namespace ForwardingReference_18_3 {

    class A_11 {
    public:
        int m_Val;
    public:
        A_11() {m_Val = 0;}

        template<typename T>
        A_11(T&& param) {m_Val = 1;} // 어지간 하면, 다 전달 참조 버전이 호출됩니다.
        
        A_11(const A_11& other) {m_Val = 2;} // 복사 생성자
    }; 
}
namespace ForwardingReference_18_4 {

    class A_11 {
    public:
        int m_Val;
    public:
        A_11() {m_Val = 0;}

        template< 
            typename T, 
            typename = typename std::enable_if< // 조건이 거짓이면 type이 정의되지 않아 오버로딩 함수 후보 목록에서 제외됩니다.
                std::is_integral< 
                    typename std::decay<T>::type // 전달된 T의 참조성을 제거한 타입을 검사합니다.
                >::value // 조건. 정수형 타입이면 true 입니다.
            >::type // 조건이 true인 경우에만 enable_if<>::type이 정의됩니다.
        >
        A_11(T&& param) {m_Val = 1;} // 정수 타입만, 전달 참조 버전이 호출되도록 합니다.
        
        A_11(const A_11& other) {m_Val = 2;} // 복사 생성자
    }; 
}
namespace ForwardingReference_18_5 {
    int f(const std::vector<int>& v) {return v.size();}
}
namespace ForwardingReference_18_6 {
    template<typename T>
    int f_11(T&& param) {return param.size();}    
}
namespace ForwardingReference_19 {

    class A_11 {
        std::string m_String;
    public:
        template<typename T>
        void SetString_11(T&& str) { // 전달 참조
            // forward() 가 적합한데, move()를 사용했습니다. 무조건 이동 연산 합니다.
            m_String = std::move<T>(str); 
        }
    }; 

}
namespace ForwardingReference_20 {
    class A_11 {
    public:
        A_11() {std::cout << "A_11 : Default Constructor" << std::endl;}    
        A_11(const A_11&) {std::cout << "A_11 : Copy Constructor" << std::endl;}  
        A_11(A_11&&) {std::cout << "A_11 : Move Constructor" << std::endl;}  
        
        A_11& operator =(const A_11& other) = delete;
        A_11& operator =(A_11&& other) noexcept = delete;
    };

    A_11 f() {
        A_11 result; // 지역 변수를 정의합니다.
        return result; // 리턴하면서 임시 개체를 생성합니다.
    } 
    A_11 f_11() {
        A_11 result; // 지역 변수를 정의합니다.
        return std::move(result); // 리턴하면서 개체를 이동합니다.
    }      
}

TEST(TestMordern, Forwarding) {

    // 개요
    {
        int a;
        int b = a; // 복사 대입 됩니다.
        int* p1 = &a; 
        int* p2 = p1; // 포인터가 가리키는 값 말고 포인터 값은 복사 대입됩니다.       
    }
    {
        using namespace ForwardingReference_1;
        int val = 0;
        int* ptr = NULL;
        int& ref = val;

        f(val, ptr, ref); // f<int, int*, int>(int, int*, int). 참조자가 제거됩니다.
       
    }
    {
        using namespace ForwardingReference_2;
        A lvalue;
        A&& rvalue_11 = std::move(lvalue); // rvalue는 이름이 부여됐으므로 좌측값입니다.
        EXPECT_TRUE(f_11(rvalue_11) == 1); // f_11(A&)를 호출합니다.
    }
    {
        using namespace ForwardingReference_2;   
        A val;
        EXPECT_TRUE(Forwarding_11(std::move(val)) == 1); // (△) 비권장. 우측값을 전달했지만 좌측값으로 호출됩니다.    
    }
    // 참조 축약 
    {
        int val;
        int* p;
        int** pp = &p; // 포인터의 포인터는 합법입니다. 다차원적으로 구성됩니다.

        int& r = val;
        // int& & rr = r; // (X) 컴파일 오류. 참조자의 참조는 불법입니다.
        int& r2 = r; // 또다른 별칭일 뿐입니다.        
    }

    {
        using namespace ForwardingReference_3;
        int val = 0;
        int& ref = val;
        f(ref); // f<int>(int). 기본적으로 참조자가 제거됩니다.
        f<int&>(ref); // f<int&>(int&). 명시적으로 지정하면 참조자로 사용됨
    }
    {
        using namespace ForwardingReference_4;

        class A {};
        A obj;

        // 기본 형태
        {
            Convert_11<A>::LRef a = obj; // A&
            Convert_11<A>::RRef b = std::move(obj); // A&& 
        }
        // & + & 과 & + &&
        {
            Convert_11<A&>::LRef a = obj; // A&, & + &
            Convert_11<A&>::RRef b = obj; // A&, & + &&
        }
        // && + & 과 && + &&
        {
            Convert_11<A&&>::LRef a = obj; // A&, && + &
            Convert_11<A&&>::RRef b = std::move(obj);; // A&&, && + &&  
        } 

    } 
    // 전달 참조
    {
        class A {};
        A val;
        // A& ref = val; // 좌측값을 참조할 수 있습니다.
        // A& ref = std::move(val); // (X) 컴파일 오류. 우측값을 참조할 수 없습니다.
        // A& ref = A(); // (X) 컴파일 오류. 임시 개체도 우측값이므로 참조할 수 없습니다.     
    }
    {
        class A {};
        A val;
        A& ref = val;

        {
            // A&& ref_11 = val; // (X) 컴파일 오류. 좌측값을 참조할 수 없습니다.(형변환을 해야 합니다.)
        }
        {
            A&& ref_11 = std::move(val); // A&&는 우측값만 받을 수 있습니다.
        }

        {
            // A&& ref_11 = ref; // (X) 컴파일 오류, 좌측값을 참조할 수 없습니다.(형변환을 해야 합니다.)
        }
        {
            A&& ref_11 = std::move(ref); // A&&는 우측값만 받을 수 있습니다.
        }
    }   
    {
        class A {};
        A val;       
        auto&& a_11 = val; // A&. 전달 참조입니다. val 타입으로 추론합니다.
        // const auto&& b_11 = val; // (X) 컴파일 오류. const를 붙여서 전달 참조가 아닙니다. &&은 우측값만 받을 수 있습니다.
        // A&& c_11 = val; // (X) 컴파일 오류. 타입 추론을 안하므로 전달 참조가 아닙니다. &&은 우측값만 받을 수 있습니다.
    }
    {
        class A {};
        A val;

        auto&& a_11 = val; // A&. val는 좌측값. 이를 참조로 만들면 A&인 좌측값 참조
        auto&& b_11 = A(); // A&&. A()는 이름이 없는 임시 개체인 우측값. 이를 참조로 만들면 A&&인 우측값 참조  
    }
    {
        class A {};
        A val;
        A& ref = val;
        A&& rref_11 = static_cast<A&&>(val);   

        auto&& a_11 = ref; // A&. ref는 좌측값 참조
        auto&& b_11 = rref_11; // A&. rref_11는 이름이 있으니 좌측값
        auto&& c_11 = std::move(val); // A&& 
    }
    {
        class A {};
        const A val;
        const A& ref = val;
        const A&& rref_11 = static_cast<const A&&>(val);   

        auto&& a_11 = val; // const T&. val는 좌측값. 이를 참조로 만들면 const T&
        auto&& b_11 = ref; // const T&. ref는 좌측값 참조
        auto&& c_11 = rref_11; // const T&. rref_11는 이름이 있으니 좌측값
        auto&& d_11 = std::move(val); // const T&& 
    }
    // move() 원리
    {
        using namespace ForwardingReference_8;

        class T {};
        remove_reference_11<T&&>::type val;
    }

    // forward() 와 완벽한 전달
    {
        using namespace ForwardingReference_10;
        A a;
        A b;
        A& ref = a;

        Forwarding_11(a, ref, std::move(b)); 
    } 
    {
        using namespace ForwardingReference_11;   
        A a;
        A b;
        A& ref = a;

        Forwarding_11(a, ref, std::move(b));   
    }  
    // forward() 원리
    {
        using namespace ForwardingReference_12;
        A a;
        Forwarding_11(a);
    }
    {
        using namespace ForwardingReference_13;
        A a;
        A& ref = a;
        Forwarding_11(ref);
    }
    {
        using namespace ForwardingReference_14;  
        A a;
        Forwarding_11(std::move(a));      
    }
    {
        using namespace ForwardingReference_15;  
        A a;
        Forwarding_11(a);      
    }
    {
        using namespace ForwardingReference_16; 
        A a;
        A b;
        A& ref = a;

        Forwarding_11(a, ref, std::move(b));        
    }    
    // 전달 참조를 이용한 함수 오버로딩
    {
        class A_11 {
            std::string m_String;
        public:
            void SetString(const std::string& str) {m_String = str;} // lvalue 로 세팅
            void SetString(std::string&& str) {m_String = str;} // 임시 개체이면 이동 연산을 위해 rvalue로 세팅
            void SetString(const char* str) {m_String = str;} // const char*로 세팅.    
        }; 
    }
    {
        using namespace ForwardingReference_18; 
        A_11 a;

        std::string str = "Hello";
        a.SetString_11(str); // 값타입으로 전달. string&로 전달합니다.
        a.SetString_11(std::move(str)); // 우측값 참조로 전달, string&&로 전달
        a.SetString_11("World"); // (const char*)& 로 전달        
    }
    {
        using namespace ForwardingReference_18_1; 

        A_11 a;

        EXPECT_TRUE(a.Func_11(1) == 2);
        EXPECT_TRUE(a.Func_11((short)1) == 1); // (△) 비권장. 전달 참조 버전이 호출됩니다.
        EXPECT_TRUE(a.Func_11((char)'a') == 1); // (△) 비권장. 전달 참조 버전이 호출됩니다.
    }

    {
        using namespace ForwardingReference_18_2;      
        A_11 a;

        EXPECT_TRUE(a.Func_11(1) == 2); // (O) 정수 계열은 FuncInternal_11(int, true_type) 이 호출됩니다.
        EXPECT_TRUE(a.Func_11((short)1) == 2); // (O) 정수 계열은 FuncInternal_11(int, true_type) 이 호출됩니다.
        EXPECT_TRUE(a.Func_11((char)'a') == 2); // (O) 정수 계열은 FuncInternal_11(int, true_type) 이 호출됩니다. 
        EXPECT_TRUE(a.Func_11("Hello") == 1);           
    }
    {
        using namespace ForwardingReference_18_3; 
        A_11 a;

        A_11 b(a); // (△) 비권장. 복사 생성자를 불러봐도 A_11&로 평가되어 전달 참조 버전이 호출됩니다.
        EXPECT_TRUE(b.m_Val == 1);

        A_11 c(const_cast<const A_11&>(a)); // 강제로 const A_11& 로 바꾸면 복사 생성자가 호출됩니다.
        EXPECT_TRUE(c.m_Val == 2);      
    } 
    // enable_if
    {
        using namespace ForwardingReference_18_4;

        A_11 a;

        A_11 b(a); // 복사 생성자가 호출됩니다.
        EXPECT_TRUE(b.m_Val == 2);

        int val;
        A_11 c(val); // 전달 참조 버전이 호출됩니다.
    } 
    // 전달 참조와 중괄호 초기화
    {
        using namespace ForwardingReference_18_5;

        EXPECT_TRUE(f({1, 2, 3}) == 3); // {1, 2, 3}는 암시적으로 vector<int> 입니다.
    } 
    {
        using namespace ForwardingReference_18_6;
     
        // EXPECT_TRUE(f_11({1, 2, 3}) == 3); // (X) 컴파일 오류. 중괄호 초기화를 무슨 타입으로 생성할지 모릅니다.
        
        // 1. 명시적으로 vector<int> 개체를 전달합니다.
        {
            std::vector<int> v_11{1, 2, 3};
            EXPECT_TRUE(f_11(v_11) == 3); 
        }

        // 2. auto로 초기화한 후 auto를 전달합니다.
        {
            auto v_11 = {1, 2, 3}; // initializer_list로 초기화 됩니다.
            EXPECT_TRUE(f_11(v_11) == 3); // initializer_list로 암시적으로 vector를 생성하여 전달합니다.
        }
    }
    {
        using namespace ForwardingReference_19; 

        A_11 a;

        std::string str = "Hello";
        a.SetString_11(str); // str은 이동 연산되어 무효화됩니다.

        std::string str2 = str; // 이동 연산되어 무효화된 값으로 str2를 초기화했습니다.
        EXPECT_TRUE(str2.empty() == true); 
    }
    // 리턴문에서 move()와 forward() 사용
    {
        using namespace ForwardingReference_20;
        A_11 a = f(); // f() 가 리턴한 임시 개체로 a를 생성합니다
    }
    {
        using namespace ForwardingReference_20;
        A_11 a = f_11(); // f() 가 리턴한 임시 개체로 a를 생성합니다
    }
}
