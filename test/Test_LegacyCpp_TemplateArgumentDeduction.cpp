#include "gtest/gtest.h" 


namespace Deduction_0 {
    template<typename T, typename U>
    T f(T, U) {return 2;}
}
namespace Deduction_1 {
    template<typename T, typename U, typename V>
    void f(T, U, V) {}
}

namespace Deduction_2 {
    template<typename T>
    void f(T) {}
}

namespace Deduction_2_1 {
    template<typename T, size_t N>
    std::size_t f(T(&arr)[N]) { // 배열에 대한 참조
        return N;
    }
}

namespace Deduction_3 {
    template<typename T>
    void f(T) {}

    void Func(int) {}
}
namespace Deduction_4 {
    template<typename T>
    void f(T) {}
}
namespace Deduction_5 {
    template<typename T>
    void f(const T&) {}
}
namespace Deduction_6 {
    template<typename T>
    void f(const T*) {}
}
namespace Deduction_7 {
    template<typename T>
    class Base {};
   
    template<typename T>
    class Derived : public Base<T> {};

    template<typename T>
    void f(Base<T>*) {}
}
namespace Deduction_8 {
    template<typename T>
    class Other {};

    template<typename T>
    class Another {
    public:
        typedef T Type;
    };

    template<typename T>
    void f(Other<T>, typename Another<T>::Type) {} // typename : 템플릿 정의 내에서 종속된 타입임
}
namespace Deduction_9 {
    template<int i> 
    class A {};

    template<short s>
    void f(A<s> a) {}
}

TEST(TestLegacyCpp, TemplateDeduction) {
    // 누락된 인수를 추론합니다.
    {
        using namespace Deduction_0;

        f<int>(0, (double)0); // int f(int, double)
        f<double>(0, (int)0); // double f(double, int)
        f(0, 0); // int f(int, int)
    }
    // 값 타입과 포인터 타입은 타입 그대로, 참조자는 참조성을 제거하고 추론합니다.
    {
        using namespace Deduction_1;    

        int val = 0;
        int* ptr = NULL;
        int& ref = val;

        f(val, ptr, ref); // f<int, int*, int>(int, int*, int). 참조자가 제거됩니다.

        f<int, int*, int&>(val, ptr, ref); // f<int, int*, int&>(int, int*, int&). 명시적으로 지정하면 참조자로 사용할 수 있습니다.
    }
    // 배열 인수는 포인터로 추론합니다.
    {
        using namespace Deduction_2;

        int arr[3];
        // Argument : int[3] -> int*로 조정 
        // T : int* 
        // Parameter : int*        
        f(arr); // f<int*>(int*)
    }

    {
        using namespace Deduction_2_1;

        int arr[3];
        EXPECT_TRUE(sizeof(arr) == 3 * sizeof(int)); // 배열의 전체 용량

        // Argument : int[3] 
        // T : int(&)[3] 
        // Parameter : int(&)[3]      
        EXPECT_TRUE(f(arr) == 3); // 배열 요소 갯수
    }
    // 함수는 함수 포인터로 추론합니다.
    {
        using namespace Deduction_3;

        // Argument : void (*)(int) 
        // T : void (*)(int) 
        // Parameter : void (*)(int)
        f(Func); // f<void (*)(int)>(void (*)(int))
    }
    // 최상위 const는 무시됨 
    {
        using namespace Deduction_4;

        const int a = 0;
        // Argument : const int -> int로 조정 
        // T : int 
        // Parameter : int
        f(a); // f<int>(int)
    }
    // 참조자의 참조자는 없으므로, 참조자가 되도록 T 변환
    {
        using namespace Deduction_5;
        int a = 0;
        int& b = a;

        // Argument : int 
        // T : int 
        // Parameter : const int&
        f(a); // f<int>(const int&)

        // Argument : int& 
        // T : int 
        // Parameter : const int&
        f(b); // f<int>(const int&)
    }
    // 포인터의 포인터가 중첩되어 만들어 지지 않도록, T 변환
    {
        using namespace Deduction_6;
        int a = 0;
        int* b = &a;

        // Argument : int* 
        // T : int 
        // Parameter : const int*
        f(&a); // f<int>(const int*)
        f(b); // f<int>(const int*)
    } 
    // 자식 개체는 부모 개체로 변환
    {
        using namespace Deduction_7;  

        Derived<int> d;
        // Argument : Derived<int>* -> Base<int>* 로 조정 
        // T : int 
        // Parameter : Base<int>*
        f(&d); // f<int>(Base<int>*)
    } 
    // 다른 인수의 추론으로부터 추론
    {
        using namespace Deduction_8;

        Other<int> other;
        // Argument1 : Other<int> 
        // T : int 
        // Parameter1 : Other<int>
        // Argument2 : int -> Argument1에서 T가 int로 추론되어 Another<int>::Type  
        // Parameter2 : int
        f(other, 10); // f<int>(Other<int>, int)
    }  
    // 템플릿 템플릿 인자에서 템플릿 인자 타입 불일치
    {
        using namespace Deduction_9;

        A<10> a;
        // Argument :A<10>에서 10은 int 타입임
        // Parameter : 템플릿 인자가 A<short>이어서 A<10>을 전달받을 수 없음.
        // f(a); // (X) 컴파일 오류. A<int>와 A<short>는 서로 다른 타입이기에 암시적 변환할 수 없음

        // Argument :int 타입 10으로 만들어진 A<10>
        // T : short 타입의 10
        // Parameter : short 타입 10으로 만들어진 A<10>
        f<10>(a); // 명시적으로 10을 전달하여 A<10> 타입이 됨. 같은 타입이어서 함수 호출됨 
    }
}

namespace Overloading_1 {
    int f(int) {return 1;} // #1.
    
    template<typename T>
    T f(T) {return 2;} // #2.
}
namespace Overloading_2 {
    template<typename T>
    int f(T a) {return 1;} // #1.

    template<typename T>
    int f(T* a) {return 2;} // #2.
}
namespace Overloading_3 {
    template<typename T>
    int f(T a) {return 1;} // #1.

    template<typename T>
    int f(T* a) {return 2;} // #2.

    template<typename T>
    int f(const T* a) {return 3;} // #3.
}
namespace Overloading_4 {
    template<typename T>
    int f(T) {return 1;} // #1.

    template<typename T>
    int f(T*, int = 1) {return 2;} // #2.
}
namespace Overloading_5 {
    template<typename T>
    class A {};
    
    template<typename T>
    int f(T&) {return 1;} // #1.

    template<typename T>
    int f(A<T>&) {return 2;} // #2.    
}
namespace Overloading_6 {
    template<typename T, typename U>
    class A {};
    
    template<typename T, typename U>
    int f(T, A<T, U>* p = 0) {return 1;} // #1.
    
    template<typename T>
    int f(T, A<T, T>* p = 0) {return 2;} // #2.    
}
namespace Overloading_7 {
    template<typename T>
    class A {
    public:
        int operator +(int) const {return 1;} // #1.
    };
    
    template<typename T>
    int operator +(const T&, int) {return 2;} // #2.      
}
namespace Overloading_8 {
    template<typename T>
    class A {
    public:
        template<typename U>
        int operator +(U) const {return 1;} // #1. 멤버 버전도 함수 템플릿 입니다.
    };
    
    template<typename T, typename U>
    int operator +(const T&, U) {return 2;} // #2.    
}
namespace Overloading_9 {
    template<typename T>
    class A {
    };
    
    template<typename T, typename U>
    int operator +(const A<T>&, U) {return 1;} // #1. 비멤버 버전

    template<typename T, typename U>
    int operator +(const T&, U) {return 2;} // #2.    
}
namespace Overloading_10 {
    template<typename T>
    int f(T a) {return 1;} // #1

    // template<typename T>
    // int f(const T a) {return 2;} // #2. (X) 컴파일 오류. 재정의됨. 최상위 const는 제거되어 #1인 f(T a)와 동일합니다.
}
namespace Overloading_11 {
    template<typename T>
    int f(T) {return 1;}  // #1.

    template<typename T>
    int f(T&) {return 2;} // #2.
}
namespace Overloading_12 {
    template<typename T>
    int f(T, T*) {return 1;}   // #1.

    template<typename T>
    int f(T, int*) {return 2;} // #2.
}


TEST(TestLegacyCpp, TemplateOverloading) {

    // 오버로딩 버전에서 일반 버전을 선택함
    {
        using namespace Overloading_1;

        // #1 : f(int)
        // #2 : T == int 이면 f(int)
        EXPECT_TRUE(f(1) == 1); // 일반 버전을 선택합니다.
    }

    // T 보다는 T*가 특수화된 버전
    {
        using namespace Overloading_2;

        int* p;
        // #1 : T == int* 이면 f(int*) 
        // #2 : T == int 이면 f(int*)
        EXPECT_TRUE(f(p) == 2); // 특수화된 버전으로 선택합니다.
    }
    // T 보다는 T* 보다는 const T*가 특수화된 버전
    {
        using namespace Overloading_3;

        const int* p;
        // #1 : T == const int* 이면 f(const int*) 
        // #2 : T == const int 이면 f(const int*) 
        // #3 : T == int 이면 f(const int*) 
        EXPECT_TRUE(f(p) == 3); // 더 특수화된 버전으로 선택합니다.
    }
    // 특수화된 버전 선택시 기본 인자는 무시하고 판단함
    {
        using namespace Overloading_4;

        int* p;
        // #1 : T == int* 이면 f(int*) 인자 1개 - 인자가 1개라고 무조건 선택되지 않습니다. 인자 2개인 #2에서 기본값 인자를 뺀 버전과 경합합니다.
        // #2 : T == int 이면 f(int*, int = 1) 인자 2개 -> 기본값이 있는 인자를 빼면 f(int*)
        EXPECT_TRUE(f(p) == 2); // 더 특수화된 버전으로 선택합니다.
    }    
    // 템플릿 템플릿 인자에서 특수화된 버전 선택
    {
        using namespace Overloading_5;

        A<int> a;
        // #1 : T == A<int> 이면 f(A<int>&) 
        // #2 : T == int 이면 f(A<int>&)
        EXPECT_TRUE(f(a) == 2); // 더 특수화된 버전으로 선택합니다.
    }
    // 기본 인자가 템플릿 템플릿 인자인 경우 타입에 따라 특수화된 버전으로 선택함
    {
        using namespace Overloading_6;

        // #1 : T == int, U == char 이면 f(int, A<int, char>*) 
        // #2 : T == int 이면 f(int, A<int, int>*)
        EXPECT_TRUE(f(0, (A<int, char>*)0) == 1); // 인자가 더 잘 맞는 버전으로 선택합니다. 

        // #1 : T == int, U == int 이면 f(int, A<int, int>*) 
        // #2 : T == int 이면 f(int, A<int, int>*) 
        EXPECT_TRUE(f(0, (A<int, int>*)0) == 2); // 더 특수화된 버전으로 선택합니다.    
        EXPECT_TRUE(f(0) == 2);  
    }    
    // 멤버 버전과 전역 버전에서의 모호함
    {
        using namespace Overloading_7;
        A<int> a;
        // #1 : T == A<int> 이면 operator +(int)
        // #2 : T == A<int> 이면 operator +(A<int>&, int) 
        EXPECT_TRUE( a + 10 == 1); // 멤버 함수 버전이 호출됩니다.
    }  
    {
        using namespace Overloading_8;
        A<int> a;
        
        // #1 : T == A<int>, U == int 이면 operator +(int)
        //      비멤버 버전으로 변환 하면    operator +(const A<int>&, int)             
        // #2 : T == A<int>, U == int 이면 operator +(const A<int>&, int) 
        // EXPECT_TRUE( a + 10 == 2); // (X) 컴파일 경고. #1 과 #2 가 모호하고, 대충 비멤버 버전을 호출합니다.
    } 
    {
        using namespace Overloading_9;
        A<int> a;
        
        // #1 : T == int, U == int 이면 operator +(A<int>&, int)
        // #2 : T == A<int>, U == int 이면 operator +(A<int>&, int) 
        EXPECT_TRUE( a + 10 == 1); // (O) 더 특수화된 버전으로 선택합니다.
    } 

    // 동등한 함수 템플릿 - 최상위 const 제거
    {
        using namespace Overloading_10;
    }  

    // 참조자로 인한 모호
    {
        using namespace Overloading_11;

         int a = 0;
         int& b = a;
        // #1 : T == int 이면 f(int), 
        //      T == int& 이면 f(int&) 
        // #2 : T == int 이면 f(int&)
        // EXPECT_TRUE(f(a) == 1); // (X) 컴파일 오류. 어느것을 호출할지 모호합니다.
        // EXPECT_TRUE(f(b) == 2); // (X) 컴파일 오류. 어느것을 호출할지 모호합니다.
    }

    // 특수화 판단 모호
    {
        using namespace Overloading_12;
        char ch;
        int i;
        int* p;

        // #1 : T == char 이면 f(char, char*)
        // #2 : T == char 이면 f(char, int*) 
        EXPECT_TRUE(f(ch, p) == 2);  // (O) 인자 타입이 일치하는 #2를 선택합니다.

        // #1 : T == int 이면 f(int, int*)
        // #2 : T == int 이면 f(int, int*) 
        // EXPECT_TRUE(f(i, p) == 2); // (X) 컴파일 오류. 어느것을 호출할지 모호합니다.
    }  

}
namespace SFINAE_1 {

    class A {
    public:
        typedef int Int; // 종속 타입
        typedef char Char; // 종속 타입               
    };
    class B {
    public:
        typedef int Int; // 종속 타입
    };

    template<typename T>
    typename T::Int f(typename T::Int param) {
        return 1;
    };
    template<typename T>
    typename T::Char f(typename T::Char param) {
        return 2;
    };
}
namespace SFINAE_2 {

    class A {
    public:
        typedef int Int; // 종속 타입
        typedef char Char; // 종속 타입
    };

    template<typename T>
    typename T::Int f(typename T::Int param) {
        return 1;
    };
    template<typename T>
    typename T::Char f(typename T::Char param) {
        typename T::ResultType result = 2; // A에는 ResultType이 없습니다.
        return result;
    };
}

TEST(TestLegacyCpp, SFINAE) {
    // SFINAE
    {
        using namespace SFINAE_1;
        EXPECT_TRUE(f<A>(10) == 1); // (O) T::Int f(typename T::Int param) 버전을 호출합니다.
        EXPECT_TRUE(f<A>('a') == 2);  // (O) T::Char f(typename T::Char param) 버전을 호출합니다.

        EXPECT_TRUE(f<B>(10) == 1);  
        EXPECT_TRUE(f<B>('a') == 1); // char를 전달했는데 int 버전이 호출됩니다.  
    }  
     {
        using namespace SFINAE_2;

        EXPECT_TRUE(f<A>(10) == 1);
        // EXPECT_TRUE(f<A>('a') == 2); // (X) 컴파일 오류. ResultType이 없습니다.
    }     
}