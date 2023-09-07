#include "gtest/gtest.h" 

namespace {
    template<int val>
    class T {
    public:
        int operator ()() {return val;} // (X) 컴파일 오류. 비 템플릿 개체의 인수로 사용할 수 없습니다. 
    };   

    constexpr int Factorial(int val) {
        // 마지막으로 1에 도달하면 재귀호출 안함
        return val == 1 ? val : val * Factorial(val - 1);
    }

    // constexpr int Factorial_14(int val) {
    //     int result = 1; // 초기화된 지역 변수 정의

    //     if (val < 1) {
    //         return 1; // 2개 이상의 리턴문
    //     }

    //     for (int i = val; 0 < i; --i) { // 제어문
    //         result *= i;
    //     }

    //     return result;
    // }   
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

    template<typename T>
    class CloneTraits {
    public:

        static T* Clone_14(const T* ptr) {
            if (ptr == NULL) {
                return  NULL;
            }

            // // 조건에 맞는 부부만 컴파일 합니다.
            // if constexpr (!IsDerivedFrom<T, ICloneable>::Val) {
            //     return new T(*ptr);
            // }
            // else {
            //     return ptr->Clone();
            // }
        } 
    };    
}

TEST(TestCppPattern, Constexpr) {
    // constexpr
    {
        const int size = 20; // 상수 입니다.

        enum class MyEnum {Val = size}; // (O) size는 컴파일 타임 상수 입니다.
        T<size> t; // (O) size는 컴파일 타임 상수 입니다.
    }

    {
        int a = 20;
        const int size = a; // 변수로부터 const int를 초기화 해서 런타임 상수 입니다.

        // enum class MyEnum {Val = size}; // (X) 컴파일 오류. size는 런타임 상수 입니다.
        // T<size> t; // (X) 컴파일 오류. size는 런타임 상수 입니다.
    }
    {
        int a = 20;
        // constexpr int size = a; // (X) 컴파일 오류. 상수를 대입해야 합니다.

        // enum class MyEnum {Val = size}; // (X)
        // T<size> t; // (X)        
    }
    // constexpr 함수
    {
        // 컴파일 타임에 계산된 120이 Val에 대입됩니다.
        enum class MyEnum {Val = Factorial(5)};
        EXPECT_TRUE(static_cast<int>(MyEnum::Val) == 1 * 2 * 3 * 4 * 5);       

        // 변수를 전달하면, 일반 함수처럼 동작합니다.
        int val = 5;
        int result = Factorial(5);
    }
    {
        // 컴파일 타임에 계산된 120이 Val에 대입됩니다.
        // enum class MyEnum {Val = Factorial_14(5)};
        // EXPECT_TRUE(static_cast<int>(MyEnum::Val) == 1 * 2 * 3 * 4 * 5);       
    }
    {
        class Area {
        private:
            int m_X;
            int m_Y;
        public:
            constexpr Area(int x, int y) : // 컴파일 타임 상수로 사용 가능
                m_X(x),
                m_Y(y) {} 
            constexpr int GetVal() const {return m_X * m_Y;}
        };

        constexpr Area area(2, 5); // 컴파일 타임 상수로 정의

        // 컴파일 타임에 계산된 면적이 Val에 대입됩니다.
        enum class MyEnum {Val = area.GetVal()}; // constexpr 함수 호출
        EXPECT_TRUE(static_cast<int>(MyEnum::Val) == 2 * 5);       
    }
    // if constexpr
    {
        using namespace Constexpr_1;

        int val;
        // int* ptr = CloneTraits<int>::Clone(&val); // (X) 컴파일 오류. int에 Clone() 함수가 없습니다.       
        // delete ptr;
    }
    {
        using namespace Constexpr_2;

        int val;
        // int* ptr = CloneTraits<int>::Clone_14(&val); // (O)
        // delete ptr; 
    }    
}
