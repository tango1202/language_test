#include "gtest/gtest.h" 


namespace Deduction_1 {
    template<typename T, typename U>
    T f(T, U) {return 2;}
}
namespace Deduction_2 {
    template<typename T>
    void f(T) {}
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

TEST(TestClassicCpp, TemplateDeduction) {
    // 누락된 인수를 추론합니다.
    {
        using namespace Deduction_1;

        f<int>(0, (double)0); // int f(int, double)
        f<double>(0, (int)0); // double f(double, int)
        f(0, 0); // int f(int, int)
    }
    // 배열 인수는 포인터로 추론합니다.
    {
        using namespace Deduction_2;

        int arr[3];
        // Argument : int[3] -> int*로 조정 
        // T : int* 
        // Parameter : int*        
        f(arr);
    }
    // 함수는 함수 포인터로 추론합니다.
    {
        using namespace Deduction_3;

        // Argument : void (*)(int) 
        // T : void (*)(int) 
        // Parameter : void (*)(int)
        f(Func);
    }
    // 최상위 const는 무시됨 
    {
        using namespace Deduction_4;

        const int a = 0;
        // Argument : const int -> int로 조정 
        // T : int 
        // Parameter : int
        f(a);
    }
    // 참조자의 참조자는 없으므로, 참조자가 되도록 T 변환
    {
        using namespace Deduction_5;
        int a = 0;
        int& b = a;

        // Argument : int 
        // T : int 
        // Parameter : const int&
        f(a);

        // Argument : int& 
        // T : int 
        // Parameter : const int&
        f(b);
    }
    // 포인터의 포인터가 중첩되어 만들어 지지 않도록, T 변환
    {
        using namespace Deduction_6;
        int a = 0;
        int* b = &a;

        // Argument : int* 
        // T : int 
        // Parameter : const int*
        f(&a);
        f(b);
    } 
    // 자식 개체는 부모 개체로 변환
    {
        using namespace Deduction_7;  

        Derived<int> d;
        // Argument : Derived<int>* -> Base<int>* 로 조정 
        // T : int 
        // Parameter : Base<int>*
        f(&d);
    } 
    // 다른 인수의 추론으로 부터 추론
    {
        using namespace Deduction_8;

        Other<int> other;
        // Argument1 : Other<int> 
        // T : int 
        // Parameter1 : Other<int>
        // Argument2 : int -> Argument1에서 T가 int로 추론되어 Another<int>::Type  
        // Parameter2 : int
        f(other, 10);
    }  
    // 템플릿 템플릿 인자에서 템플릿 인자 타입 불일치
    {
        using namespace Deduction_9;

        A<10> a;
        // Argument :A<10>에서 10은 int 타입임
        // Parameter : A<short>이어서 A<10>을 전달받을 수 없음.
        // f(a); // (X) 컴파일 오류. A<int>와 A<short>는 서로 다른 타입이기에 암시적 변환할 수 없음

        // Argument :A<10> 
        // Parameter : A<10>
        f<10>(a); // s 로 10을 전달하여 Parameter에서 A<10> 이 됨. 같은 타입이어서 함수 호출됨   
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
        int operator +(U) const {return 1;} // #1. 멤버 버전
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
    int f(T a) {return 1;} // #1.

    // template<typename T>
    // int f(const T a) {return 2;} // #2. (X) 컴파일 오류. 재정의됨. 최상위 const는 제거되어 f(T a)와 동일
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

TEST(TestClassicCpp, TemplateOverloading) {

    // 오버로딩 버전에서 일반 버전을 선택함
    {
        using namespace Overloading_1;

        // #1 : f(int)
        // #2 : T == int 이면 f(int)
        // 일반 버전을 선택함
        EXPECT_TRUE(f(1) == 1); 
    }

    // T 보다는 T*가 특수화된 버전
    {
        using namespace Overloading_2;

        int* p;
        // #1 : T == int* 이면 f(int*) 
        // #2 : T == int 이면 f(int*)
        // 특수화된 버전으로 선택함.
        EXPECT_TRUE(f(p) == 2);
    }
    // T 보다는 T* 보다는 const T*가 특수화된 버전
    {
        using namespace Overloading_3;

        const int* p;
        // #1 : T == const int* 이면 f(const int*) 
        // #2 : T == const int 이면 f(const int*) 
        // #3 : T == int 이면 f(const int*) 
        // 더 특수화된 버전으로 선택함.    
        EXPECT_TRUE(f(p) == 3);
    }
    // 특수화된 버전 선택시 기본 인자는 무시하고 판단함
    {
        using namespace Overloading_4;

        int* p;
        // #1 : T == int* 이면 f(int*) 인자 1개 - 인자가 1개라고 무조건 선택되지 않음. 인자 2개인 #2에서 기본값 인자를 뺀 버전과 경합
        // #2 : T == int 이면 f(int*, int = 1) 인자 2개
        // 더 특수화된 버전으로 선택함.
        EXPECT_TRUE(f(p) == 2);
    }    
    // 템플릿 템플릿 인자에서 특수화된 버전 선택
    {
        using namespace Overloading_5;

        A<int> a;
        // #1 : T == A<int> 이면 f(A<int>&) 
        // #2 : T == int 이면 f(A<int>&)
        // 더 특수화된 버전으로 선택함.
        EXPECT_TRUE(f(a) == 2);
    }
    // 기본 인자가 탬플릿 탬플릿 인자인 경우 타입에 따라 특수화된 버전으로 선택함
    {
        using namespace Overloading_6;

        // #1 : T == int, U == char 이면 f(int, A<int, char>*) 
        // #2 : T == int 이면 f(int, A<int, int>*)
        // 인자가 더 잘 맞는 버전으로 선택함.
        EXPECT_TRUE(f(0, (A<int, char>*)0) == 1); 

        // #1 : T == int, U == int 이면 f(int, A<int, int>*) 
        // #2 : T == int 이면 f(int, A<int, int>*) 
        // 더 특수화된 버전으로 선택함.
        EXPECT_TRUE(f(0, (A<int, int>*)0) == 2); 
        EXPECT_TRUE(f(0) == 2);  
    }    
    // 멤버 버전과 전역 버전에서의 모호함
    {
        using namespace Overloading_7;
        A<int> a;
        // #1 : T == A<int> 이면 A<int>&.operator +(int)
        // #2 : T == A<int> 이면 operator +(A<int>&, int) 
        // 멤버 함수 버전 호출
        EXPECT_TRUE( a + 10 == 1);
    }  
    {
        using namespace Overloading_8;
        A<int> a;
        
        // #1 : T == A<int>, U == int 이면 A<int>&.operator +(int)
        //      비멤버 버전 변환        operator +(A<int>&, int)             
        // #2 : T == A<int>, U == int 이면 operator +(A<int>&, int) 
        // (X) 컴파일 경고. #1 과 #2 가 모호하고, 대충 비멤버 버전 호출함
        // EXPECT_TRUE( a + 10 == 2);
    } 
    {
        using namespace Overloading_9;
        A<int> a;
        
        // #1 : T == int, U == int 이면 operator +(A<int>&, int)
        // #2 : T == A<int>, U == int 이면 operator +(A<int>&, int) 
        // (O) 더 특수화된 버전으로 선택함.
        EXPECT_TRUE( a + 10 == 1);
    } 

    // 동등한 템플릿 함수 - 최상위 const 제거
    {
        using namespace Overloading_10;
        // #1 : T == int 이면 f(int) 
        // #2 : T == int 이면 f(const int)인데, 오버로딩시 최상위 const는 제거되므로 f(int)
        // (X) 컴파일 오류. 최상위 const는 제거됨. 오버로딩 함수가 중복됨
        // EXPECT_TRUE(f(a) == 1); 
        // EXPECT_TRUE(f(b) == 2);
    }  

    // 참조자로 인한 모호
    {
        using namespace Overloading_11;

         int a = 0;
         int& b = a;
        // #1 : T == int 이면 f(int), 
        //      T == int& 이면 f(int&) 
        // #2 : T == int 이면 f(int&)
        // (X) 컴파일 오류. 어느것을 호출할지 모호함.
        // EXPECT_TRUE(f(a) == 1);
        // EXPECT_TRUE(f(b) == 2);
    }

    // 특수화 판단 모호
    {
        using namespace Overloading_12;
        char ch;
        int i;
        int* p;

        // #1 : T == char 이면 f(char, char*)
        // #2 : T == char 이면 f(char, int*) 
        // 인자 타입이 일치하는 #2를 선택함.
        EXPECT_TRUE(f(ch, p) == 2);

        // #1 : T == int 이면 f(int, int*)
        // #2 : T == int 이면 f(int, int*) 
        // (X) 컴파일 오류. 어느것을 호출할지 모호함.
        // EXPECT_TRUE(f(i, p) == 2);
    }      
}
