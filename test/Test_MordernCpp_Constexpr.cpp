#include "gtest/gtest.h" 

namespace {
    template<int val>
    class T {
    public:
        int operator ()() {return val;} // (X) 컴파일 오류. 비 템플릿 개체의 인수로 사용할 수 없습니다. 
    };   

    constexpr int Factorial_11(int val) {
        // 마지막으로 1에 도달하면 재귀호출 안함
        return val == 1 ? val : val * Factorial_11(val - 1);
    }
#if 201402L <= __cplusplus // C++14~    
    // C++14 `constexpr` 함수 제약 완화
    constexpr int Factorial_14(int val) {
        int result{1}; // 초기화된 지역 변수 정의

        if (val < 1) {
            return 1; // 2개 이상의 리턴문
        }

        for (int i{val}; 0 < i; --i) { // 제어문
            result *= i;
        }

        return result;
    }   
#endif    
}

namespace Constexpr_1 {
    // class ICloneable {
    // private:
    //     ICloneable(const ICloneable& other) {}
    //     ICloneable& operator =(const ICloneable& other) {return *this;}
    // protected:
    //     ICloneable() {}  
    //     ~ICloneable() {} 
    // public:
    //     virtual ICloneable* Clone() const = 0; 
    // };

    // // D가 B를 상속하였는지 검사하는 템플릿
    // template<typename D, typename B> 
    // class IsDerivedFrom {
    //     class No {};
    //     class Yes {No no[2];};

    //     static Yes Test(B*);
    //     static No Test(...);
    // public:
    //     enum {Val = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes)};
    // };

    // template<typename T>
    // class CloneTraits {
    // public:

    //     static T* Clone(const T* ptr) {
    //         if (ptr == NULL) {
    //             return  NULL;
    //         }

    //         if (!IsDerivedFrom<T, ICloneable>::Val) {
    //             return new T(*ptr);
    //         }
    //         else {
    //             return ptr->Clone();
    //         }
    //     } 
    // };    
}

namespace Constexpr_2 {
    class ICloneable {
    private:
        ICloneable(const ICloneable& other) {}
        ICloneable& operator =(const ICloneable& other) {return *this;}
    protected:
        ICloneable() {}  
        ~ICloneable() {} 
    public:
        virtual ICloneable* Clone() const = 0; 
    };

    // D가 B를 상속하였는지 검사하는 템플릿
    template<typename D, typename B> 
    class IsDerivedFrom {
        class No {};
        class Yes {No no[2];};

        static Yes Test(B*);
        static No Test(...);
    public:
        enum {Val = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes)};
    };
#if 201703L <= __cplusplus // C++17~
    // C++17
    template<typename T>
    class CloneTraits {
    public:
        static T* Clone_17(const T* ptr) {
            if (ptr == NULL) {
                return  NULL;
            }

            // 조건에 맞는 부부만 컴파일 합니다.
            if constexpr (!IsDerivedFrom<T, ICloneable>::Val) {
                return new T(*ptr);
            }
            else {
                return ptr->Clone();
            }
        } 
    }; 
#endif       
}
namespace Consteval_1 {
#if 202002L <= __cplusplus // C++20~ 
    constexpr int Add_14(int a, int b) {return a + b;}
    consteval int Add_20(int a, int b) {return a + b;}
#endif
}
namespace Constinit_1 {
#if 202002L <= __cplusplus // C++20~
    
    constexpr int f_11() {return 10;} // 컴파일 타임 함수 입니다.
    
    // constinit int 로 받을 수도 있지만, s_Val_20과 s_m_Val_20에 대입하기 위해 const를 붙였습니다.
    constinit const int g_Val_20 = f_11(); // constinit여서 컴파일 타임에 초기화 되어야 합니다.
        
    // g_Val_20은 컴파일 타임 const 상수 입니다.
    constinit static int s_Val_20 = g_Val_20; // constinit여서 컴파일 타임에 초기화 되어야 합니다.
    
    class T_20 {
    public:
        // C++17 부터 인라인 변수를 이용하여 정적 멤버 변수를 멤버 선언부에서 초기화할 수 있습니다.
        constinit static inline int s_m_Val_20 = g_Val_20; // constinit여서 컴파일 타임에 초기화 되어야 합니다.
    };
#endif
}
namespace Constexpr_3 {
#if 202002L <= __cplusplus // C++20~ 
    constexpr void Func_20() {
        int a; // 초기화되지 않은 지역 변수

        try {}
        catch(...) {}
    }
#endif    
}

namespace Constexpr_4 {
   
    union MyUnion {
        int i;
        float f;
    };
      
    constexpr int Func_20() {
#if 202002L <= __cplusplus // C++20~     
        MyUnion myUnion{};
       
        myUnion.i = 3;
        // (X) ~C++20 컴파일 오류. change of the active member of a union from 'MyUnion::i' to 'MyUnion::f'
        // (O) C++2O~       
        myUnion.f = 1.2f; // 멤버 변수 활성 전환을 허용합니다.
#endif        
        return 1;
    }
}

namespace Constexpr_5 {
    class Base {
    public:
        virtual int Func() const {return 0;}
    };
#if 202002L <= __cplusplus // C++20~      
    class Derived_20 : public Base {
    public:
        constexpr virtual int Func() const override {return 1;} 
    };
      
    constexpr int Func_20() {
   
        Derived_20 d;

        Base* base = &d;
        Derived_20* derived_20 = dynamic_cast<Derived_20*>(base); // dynamic_cast를 사용할 수 있습니다.
        typeid(base); // typeid를 사용할 수 있습니다.
        return 1;
    }
#endif     
}

TEST(TestMordern, Constexpr) {
    // constexpr
    {
        const int size{20}; // 상수 입니다.

        enum class MyEnum_11 {Val = size}; // (O) size는 컴파일 타임 상수 입니다.
        T<size> t; // (O) size는 컴파일 타임 상수 입니다.
    }

    {
        int a{20};
        const int size{a}; // 변수로부터 const int를 초기화 해서 런타임 상수 입니다.

        // enum class MyEnum_11 {Val = size}; // (X) 컴파일 오류. size는 런타임 상수 입니다.
        // T<size> t; // (X) 컴파일 오류. size는 런타임 상수 입니다.
    }
    {
        constexpr int size_11{20}; // 컴파일 타임 상수 입니다.

        enum class MyEnum_11 {Val = size_11}; // (O)
        T<size_11> t; // (O) 
    }    
    {
        int a{20};
        // constexpr int size_11{a}; // (X) 컴파일 오류. 상수를 대입해야 합니다.
    }
    // constexpr 함수
    {
        // 컴파일 타임에 계산된 120이 Val에 대입됩니다.
        enum class MyEnum_11 {Val = Factorial_11(5)};
        EXPECT_TRUE(static_cast<int>(MyEnum_11::Val) == 1 * 2 * 3 * 4 * 5);       

        // 변수를 전달하면, 일반 함수처럼 동작합니다.
        int val{5};
        int result{Factorial_11(5)};
    }

    {
        class Area_11 {
        private:
            int m_X;
            int m_Y;
        public:
            constexpr Area_11(int x, int y) : // 컴파일 타임 상수로 사용 가능
                m_X(x),
                m_Y(y) {} 
            constexpr int GetVal_11() const {return m_X * m_Y;}
        };

        {
            constexpr Area_11 area(2, 5); // 컴파일 타임 상수로 정의

            // 컴파일 타임에 계산된 면적이 Val에 대입됩니다.
            enum class MyEnum_11 {Val = area.GetVal_11()}; // constexpr 함수 호출
            EXPECT_TRUE(static_cast<int>(MyEnum_11::Val) == 2 * 5);   
        }

        {
            int x = 10;
            // constexpr Area_11 area{x, 5}; // (X) 컴파일 오류. x는 컴파일 타임 상수가 아닙니다.
            Area_11 area{x, 5}; // 런타임 함수로 동작합니다.
        }
    }
    {
 
        class Area_11 {
        private:
            int m_X;
            int m_Y;
        public:
            constexpr Area_11(int x, int y) : // 컴파일 타임 상수로 사용 가능
                m_X{x},
                m_Y{y} {} 
            constexpr int GetVal_11() const {return m_X * m_Y;}
#if 201402L <= __cplusplus // C++14~   
            // C++14 부터는 constexpr개체에서도 멤버 변수의 값을 수정할 수 있습니다.            
            constexpr void SetX_14(int val) {m_X = val;}
            constexpr void SetY_14(int val) {m_Y = val;}    
#endif                    
        }; 
         
    } 
#if 201402L <= __cplusplus // C++14~        
    // (C++14~) constexpr 함수 제약 완화
    {
        // 컴파일 타임에 계산된 120이 Val에 대입됩니다.
        enum class MyEnum_11 {Val = Factorial_14(5)};
        EXPECT_TRUE(static_cast<int>(MyEnum_11::Val) == 1 * 2 * 3 * 4 * 5);       
    }
#endif    
    // C++17 if constexpr
    {
        using namespace Constexpr_1;

        int val;
        // int* ptr{CloneTraits<int>::Clone(&val)}; // (X) 컴파일 오류. int에 Clone() 함수가 없습니다.       
        // delete ptr;
    }
#if 201703L <= __cplusplus // C++17~    
    {
        using namespace Constexpr_2;

        int val;
        int* ptr{CloneTraits<int>::Clone_17(&val)}; // (O)
        delete ptr; 
    } 
#endif    

#if 202002L <= __cplusplus // C++20~ 
    {
        using namespace Consteval_1;
        {
            enum class MyEnum_11 {Val = Add_14(1, 2)}; // 컴파일 타임 함수로 사용
            EXPECT_TRUE(static_cast<int>(MyEnum_11::Val) == 3); 
        }
        {
            enum class MyEnum_11 {Val = Add_20(1, 2)}; // 컴파일 타임 함수로 사용
            EXPECT_TRUE(static_cast<int>(MyEnum_11::Val) == 3); 
        }

        int a{10};
        int b{20};
        EXPECT_TRUE(Add_14(a, b) == 30); // 런타임 함수로 사용
        // EXPECT_TRUE(Add_20(a, b) == 30); // (X) 컴파일 오류. 런타임 함수로 사용할 수 없습니다.
    }
#endif
#if 202002L <= __cplusplus // C++20~ 
    // (C++20~) constexpr 함수 제약 완화
    {
        class Base {
        public:
            virtual int Func() const {return 0;}
        };
        class Derived_20 : public Base {
        public:
            constexpr virtual int Func() const override {return 1;} // C++17 이하에서는 컴파일 오류가 발생했습니다.
        };

        constexpr Derived_20 a_20;
        enum class MyEnum_11 {Val = a_20.Func()}; // 컴파일 타임 상수

        const Base* ptr = &a_20;
        EXPECT_TRUE(ptr->Func() == 1); // 부모 개체의 포인터로 런타임에 가상 함수를 호출할 수 있습니다.
        // enum class MyEnum_11 {Val = ptr->Func()}; // (X) 컴파일 오류. 컴파일 타임 상수가 아닙니다.
    }
#endif

    {
        using namespace Constexpr_4;

        enum class MyEnum_11 {Val = Func_20()}; 
  
    }

}

