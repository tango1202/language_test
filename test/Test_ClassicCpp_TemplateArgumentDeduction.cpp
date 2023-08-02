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
    void f(Other<T>, typename Another<T>::Type) {} // typename : 템플릿 정의 내에서 종속된 타입임을 컴파일러에게 Hint 전달, 없으면 static 개체일수도 있어 모호함
}
namespace Deduction_9 {
    template<int i> 
    class A {};

    template<int i>
    void f(A<i> a) {}
}
namespace Deduction_10 {
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
        // Argument : int[3] -> int*로 조정 -> T : int* -> Parameter : int*
        f(arr);
    }
    // 함수는 함수 포인터로 추론합니다.
    {
        using namespace Deduction_3;

        // Argument : void (*)(int) -> T : void (*)(int) -> Parameter : void (*)(int)
        f(Func);
    }
    // 최상위 const는 무시됨 
    {
        using namespace Deduction_4;

        const int a = 0;
        // Argument : const int -> int로 조정 -> T : int -> Parameter : int
        f(a);
    }
    // 참조자의 참조자는 없으므로, 참조자가 되도록 T 변환
    {
        using namespace Deduction_5;
        int a = 0;
        int& b = a;

        // Argument : int -> T : int -> Parameter const int&
        f(a);

        // Argument : int& -> T : int -> Parameter : const int&
        f(b);
    }
    // 포인터의 포인터가 중첩되어 만들어 지지 않도록, T 변환
    {
        using namespace Deduction_6;
        int a = 0;
        int* b = &a;

        // Argument : int* -> T : int -> Parameter : const int*
        f(&a);
        f(b);
    } 
    // 자식 개체는 부모 개체로 변환
    {
        using namespace Deduction_7;  

        Derived<int> d;
        // Argument : Derived<int>* -> Base<int>* 로 조정 -> T : Base<T> -> Parameter : Base<T>*
        f(&d);
    } 
    // 다른 인수의 추론으로 부터 추론
    {
        using namespace Deduction_8;

        Other<int> other;
        // Argument1 : Other<int> -> T : int -> Parameter : Other<int>
        // Argument2 : int -> Argument1에서 T가 int로 추론되어 Another<int>::Type 확인 -> Parameter : int
        f(other, 10);
    }  
    // 템플릿 템플릿 인자에서 템플릿 인자 타입 불일치
    {
        using namespace Deduction_9;

        A<10> a;
       
        // Argument :A<10> -> Parameter : A<int> 
        f(a);
    }
    {
        using namespace Deduction_10;

        A<10> a;
        // Argument :A<10> -> Parameter : A<short>
        // f(a); // (X) 컴파일 오류. A<10>을 A<short>는 서로 다른 타입이기에 암시적 변환할 수 없음

        // Argument :A<10> -> Parameter : A<10>
        f<10>(a); // s 로 10을 전달하여 Parameter에서 A<10> 이 됨. 같은 타입이어서 함수 호출됨   
    }
}

namespace Overloading_1 {
    int f(int) {return 1;}
    
    template<typename T>
    T f(T) {return 2;}
}
namespace Overloading_2 {
    template<typename T>
    int f(T a) {return 1;}

    // template<typename T>
    // int f(const T a) {return 2;} // (X) 컴파일 오류. 재정의됨. 최상위 const는 제거되어 f(T a)와 동일
}
namespace Overloading_3 {
    template<typename T>
    int f(T a) {return 1;}

    template<typename T>
    int f(T* a) {return 2;} 
}
namespace Overloading_4 {
    template<typename T>
    int f(T a) {return 1;}

    template<typename T>
    int f(T* a) {return 2;}

    template<typename T>
    int f(const T* a) {return 3;}
}
namespace Overloading_5 {
    class A {};
    template<typename T>
    class B {
    public:
        template<typename U>
        int operator +(U&) {return 1;} // #1
    };
    
    template<typename T, typename U>
    int operator +(T&, U&) {return 2;} // #2     
}

namespace Overloading_6 {
    template<typename T>
    int f(T, T*) {return 1;}   // #1

    template<typename T>
    int f(T, int*) {return 2;} // #2
}

namespace Overloading_7 {
    template<typename T>
    int f(T) {return 1;}  // #1

    template<typename T>
    int f(T&) {return 2;} // #2
}

namespace Overloading_8 {
    template<typename T>
    class A {};
    
    template<typename T>
    int f(T&) {return 1;} // #1

    template<typename T>
    int f(A<T>&) {return 2;} // #2    
}
namespace Overloading_9 {
    template<typename T>
    int f(T) {return 1;} // #1

    template<typename T>
    int f(T*, int = 1) {return 2;} // #2
}
namespace Overloading_10 {
    template<typename T, typename U>
    class A {};
    
    template<typename T, typename U>
    int f(T, A<T, U>* p = 0) {return 1;} // #1
    
    template<typename T>
    int f(T, A<T, T>* p = 0) {return 2;} // #2    
}
namespace Overloading_11 {
    template<typename T>
    int f(T) {return 1;}    // #1

    template<typename T>
    int f(T*) {return 2;}   // #2: #1의 오버로딩

    template<>
    int f(int*) {return 3;} // #3: 탬플릿 특수화   
}

namespace Overloading_12 {
    template<typename T>
    int f(T) {return 1;}    // #1

    template<>
    int f(int*) {return 3;} // #3: 탬플릿 특수화   

    template<typename T>
    int f(T*) {return 2;}   // #2: #1의 오버로딩
}

TEST(TestClassicCpp, TemplateOverloading) {

    // 오버로딩 버전에서 일반 버전을 채택함
    {
        using namespace Overloading_1;

        // #1 : f(int)
        // #2 : T == int 이면 f(int). 일반 버전을 채택함
        EXPECT_TRUE(f(1) == 1); 
    }
    // 최상위 const 제거
    {
        // (X) 컴파일 오류. 최상위 const는 제거됨. 오버로딩 함수가 중복됨
        // #1 : T == int 이면 f(int) 
        // #2 : T == int 이면 f(const int)인데, 오버로딩시 최상위 const는 제거되므로 f(int)
        // EXPECT_TRUE(f(a) == 1); 
        // EXPECT_TRUE(f(b) == 2);
    }
    // T 보다는 T*가 전문화된 버전
    {
        using namespace Overloading_3;

        int* p;
        // #1 : T == int* 이면 f(int*) 
        // #2 : T == int 이면 f(int*)로 추론됨. 더 전문화된 버전으로 채택함.
        EXPECT_TRUE(f(p) == 2);
    }
    // T 보다는 T* 보다는 const T*가 전문화된 버전
    {
        using namespace Overloading_4;

        const int* p;
        // #1 : T == const int* 이면 f(const int*) 
        // #2 : T == const int 이면 f(const int*) 
        // #3 : T == int 이면 f(const int*) 로 추론됨. 더 전문화된 버전으로 채택함.    
        EXPECT_TRUE(f(p) == 3);
    }
    // 멤버 버전과 전역 버전에서의 모호함
    {
        using namespace Overloading_5;
        A a;
        B<int> b;
        // (X) 컴파일 오류.
        // #1 : B<int>&.operator +(A&)
        // #2 : operator +(B<int>&, A&) 로 추론됨. 어느것을 호출할지 모호함.
        // EXPECT_TRUE( b + a == 2);
    }
    // 기초 타입의 차이는 전문화 판단에 관여하지 않음
    {
        using namespace Overloading_6;
        char ch;
        int i;
        int* p;

        // #1 : T == char 이면 f(char, char*)
        // #2 : T == char 이면 f(char, int*) 로 추론됨. 인자 타입이 일치하여 채택됨.
        EXPECT_TRUE(f(ch, p) == 2);

        // (X) 컴파일 오류.
        // #1 : T == int 이면 f(int, int*)
        // #2 : T == int 이면 f(int, int*) 로 추론됨. 어느것을 호출할지 모호함.
        // EXPECT_TRUE(f(i, p) == 2);
    }
    // 개체 타입과 개체 참조 타입의 모호함
    {
        using namespace Overloading_7;

         int a = 0;
         int& b = a;
        // (X) 컴파일 오류.
        // #1 : T == int 이면 f(int), T == int& 이면 f(int&) 
        // #2 : T == int 이면 f(int&) 로 추론됨. 어느것을 호출할지 모호함.
        // EXPECT_TRUE(f(a) == 1);
        // EXPECT_TRUE(f(b) == 2);
    }
    // 템플릿 템플릿 인자에서 전문화된 버전 선택
    {
        using namespace Overloading_8;

        A<int> a;
        // #1 : T == A<int> 이면 f(A<int>&) 
        // #2 : T == int 이면 f(A<int>&) 로 추론됨. 더 전문화된 버전으로 채택함.
        EXPECT_TRUE(f(a) == 2);
    }
    // 전문화된 버전 선택시 기본 인자는 무시하고 판단함
    {
        using namespace Overloading_9;

        int* p;
        // #1 : T == int* 이면 f(int*)  
        // #2 : T == int 이면 f(int*, int = 1) 로 추론됨. 더 전문화된 버전으로 채택함.
        EXPECT_TRUE(f(p) == 2);
    }
    // 기본 인자가 탬플릿 탬플릿 인자인 경우 타입에 따라 전문화된 버전으로 채택함
    {
        using namespace Overloading_10;

        // #1 : T == int, U == char 이면 f(int, A<int, char>*) 로 추론됨. 
        // #2 : T == int 이면 f(int, A<int, int>*) 로 추론되어 타입이 다름.
        EXPECT_TRUE(f(0, (A<int, char>*)0) == 1); 

        // #1 : T == int, U == int 이면 f(int, A<int, int>*) 
        // #2 : T == int 이면 f(int, A<int, int>*) 로 추론됨. 더 전문화된 버전으로 채택함.
        EXPECT_TRUE(f(0, (A<int, int>*)0) == 2); 
        EXPECT_TRUE(f(0) == 2); 
    }
    // 오버로딩 버전에서 일치하는 것을 채택하고, 없다면 특수화 버전에서 정의한 것으로 채택하다는데, GCC 에서는 가장 마지막 정의된 것을 채택함.
    {
        using namespace Overloading_11;

        int a;
        int* p = &a;
        // #1 : T == int* 이면 f(int*)
        // #2 : T == int 이면 f(int*) 
        // #3 : f(int*) - #2와 #3의 정의 순서가 바뀌면 마지막에 정의된 것으로 채택함
        EXPECT_TRUE(f(p) == 3); // (△) 비권장. 정의 순서에 따라 바뀜
    }
    {
        using namespace Overloading_12;

        int a;
        int* p = &a;
        // #1 : T == int* 이면 f(int*)
        // #3 : f(int*) - #2와 #3의 정의 순서가 바뀌면 마지막에 정의된 것으로 채택함
        // #2 : T == int 이면 f(int*) 
        EXPECT_TRUE(f(p) == 2); // (△) 비권장. 정의 순서에 따라 바뀜
    }
}
