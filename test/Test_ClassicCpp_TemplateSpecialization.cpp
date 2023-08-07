#include "gtest/gtest.h" 

namespace Specialization_1 {
    template<typename T>
    class A {
    public:
        int f() {return 1;} // #1    
    };

    template<>
    class A<int> {
    public:
        int f() {return 2;} // #2    
    };
}
namespace Specialization_2 {
    template<typename T>
    int Func(T val) {return 1;} // #1

    template<>
    int Func<int>(int val) {return 2;} // #2

    // 혹은 추론되는 타입을 아예 빼서 정의할 수 있습니다.
    // template<>
    // int Func(int val) {return 2;} // #2
}
namespace Specialization_3 {
    template<typename T>
    class A {
    public:
        template<typename U>
        class B {
        public:
            template<typename V>
            int f(V val) {return 1;} // #1
        };
    };

    template<> // 바깥쪽 주 템플릿도 특수화
    template<> // 안쪽 중첩 클래스 특수화
    template<> // 템플릿 멤버 함수 특수화
    int A<int>::B<int>::f<int>(int val) {return 2;} // #2

    // (X) 컴파일 오류. 템플릿 멤버 함수를 특수화 하면, 상위 템플릿도 특수화 해야 합니다.
    // template<typename T> 
    // template<typename U> 
    // template<> 
    // int A<T>::B<U>::f<int>(int val) {return 2;} // #2
}
namespace Specialization_4 {
    template<typename T>
    class A { // 주 템플릿
    public:
        int f() {return 1;} // #1. 
    };

    template<typename T>
    class A<T*> { // 템플릿 부분 특수화 
    public:
        int f() {return 2;} // #2. 
    };
}
namespace Specialization_5 {
    // 주 템플릿
    template<typename T, typename U> 
    class A {
    public:
        int f(); 
    };
    
    // 부분 특수화
    template<typename T>
    class A<T, int> {
    public:
        int f();
    };

    // 주 템플릿 정의
    template<typename T, typename U>
    int A<T, U>::f() {return 1;}  

    // 부분 특수화 정의
    template<typename T>
    int A<T, int>::f() {return 2;}
    
    // 전체 특수화 정의도 가능
    template<>
    int A<char, int>::f() {return 3;}    
}

namespace Specialization_6 {
    template<typename T>
    class A { // 주 템플릿
    public:
        int f() {return 1;} // #1. 
    };

    template<typename T>
    class A<T*> { // 템플릿 부분 특수화 
    public:
        int f() {return 2;} // #2. 
    };

    template<>
    class A<int*> { // 템플릿 특수화 
    public:
        int f() {return 3;} // #3. 
    };    
}

namespace Specialization_7 {
    template<typename T> 
    int f(T) {return 1;} // #1

    template<typename T> 
    int f(T*) {return 2;} // #2. 템플릿 함수 오버로딩. 부분 특수화와 모양이 비슷합니다.
}
namespace Specialization_8 {
    template<typename T> 
    int f(T) {return 1;} // #1

    template<> 
    int f(int*) {return 3;} //#3. #1의 템플릿 특수화 버전. T== int*, int f<int*>(int*)

    template<typename T> 
    int f(T*) {return 2;} // #2. 템플릿 함수 오버로딩.   
}

namespace Specialization_9 {
    template<typename T> 
    int f(T) {return 1;} // #1

    template<typename T> 
    int f(T*) {return 2;} // #2. 템플릿 함수 오버로딩. 

    template<> 
    int f(int*) {return 3;} //#3. #2의 템플릿 특수화 버전. T== int, int f<int>(int*)
}

TEST(TestClassicCpp, Specialization) {
    // 템플릿 특수화
    {
        using namespace Specialization_1;

        A<char> a;
        A<int> b;

        EXPECT_TRUE(a.f() == 1);
        EXPECT_TRUE(b.f() == 2); // int에 특수화된 버전 호출
    }
    // 템플릿 함수 특수화
    {
        using namespace Specialization_2;

        EXPECT_TRUE(Func('a') == 1);
        EXPECT_TRUE(Func(10) == 2); // int에 특수화된 버전 호출
    }
    // 템플릿 멤버 함수, 템플릿 중첩 클래스 특수화
    {  
        using namespace Specialization_3;

        A<char>::B<int> a;
        A<int>::B<int> b;

        // EXPECT_TRUE(a.f('a') == 1);
        // EXPECT_TRUE(a.f(10) == 1); 
        // EXPECT_TRUE(b.f('a') == 1); 
        // EXPECT_TRUE(b.f(10) == 2); // int에 특수화된 버전 호출
    }
    // 템플릿 부분 특수화
    {
        using namespace Specialization_4;
        
        A<int> a;
        A<int*> b;

        EXPECT_TRUE(a.f() == 1);
        EXPECT_TRUE(b.f() == 2); // 템플릿 부분 특수화 버전
    }
    // 템플릿 부분 특수화에서 선언과 정의 분리
    {
        using namespace Specialization_5;

        A<int, char> a; // 주 템플릿
        A<int, int> b;// U == int 인 부분 특수화
        A<char, int> c; // 전체 특수화
        
        EXPECT_TRUE(a.f() == 1);
        EXPECT_TRUE(b.f() == 2);
        EXPECT_TRUE(c.f() == 3);
    }
    // 템플릿 특수화 우선 순위
    {
        using namespace Specialization_6;
        
        A<int> a;
        A<int*> b;
        A<char*> c;

        EXPECT_TRUE(a.f() == 1);
        EXPECT_TRUE(b.f() == 3); // 템플릿 특수화 버전
        EXPECT_TRUE(c.f() == 2); // 템플릿 부분 특수화 버전
    } 
    // 템플릿 함수 오버로딩
    {
        using namespace Specialization_7;
        int a;
        EXPECT_TRUE(f(a) == 1); // 주 템플릿 함수 버전이 실행됩니다. T == int, f<int>(int)
        EXPECT_TRUE(f(&a) == 2); // 템플릿 특수화 버전이 실행됩니다. T == int, f<int>(int*)
    } 
    // 템플릿 함수 오버로딩과 템플릿 함수 특수화의 혼용
    {
        using namespace Specialization_8;  

        int a;
        EXPECT_TRUE(f(&a) == 2); // 템플릿 오버로딩 버전이 실행됩니다. T == int, f<int>(int*)
    } 
    {
        using namespace Specialization_8;  

        int a;
        EXPECT_TRUE(f<int*>(&a) == 3); // #1 템플릿 함수가 호출되고, 이중 특수화 버전인 #3 이 호출됩니다.
        EXPECT_TRUE(f<int>(&a) == 2); // #2 템플릿 함수가 호출됩니다.
    }     
    {
        using namespace Specialization_9;  
        int a;
        EXPECT_TRUE(f(&a) == 3); // 템플릿 특수화 버전이 실행됩니다. T == int, f<int>(int*)
    }  
      
}

