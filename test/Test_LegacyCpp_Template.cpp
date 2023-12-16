#include "gtest/gtest.h" 

namespace Template_1 {
    // 클래스 템플릿 정의부 - 코드가 생성되지 않은 상태
    template<typename T> 
    class MyClass {
    public:
        T Plus(T left, T right) const {return left + right;}    
    }; 
}
namespace Template_2 {
    // > 
    // template<bool b = 3 > 4> // (X) 컴파일 오류
    // class A {};
    
    template<bool b = (3 > 4)> // (O)
    class B {};
}
namespace Template_3 {
    template<typename T>
    class A {
    public:
        void f() {}
        void g(); // 함수 선언만 됨. 불완전함. 함수 정의부가 없음
    };    
}

namespace Template_4 {
    // 함수 템플릿    
    template<typename T>
    T Plus(T left, T right) {
        return left + right;
    }
}

namespace Template_5 {
    class A {
    public:
        // 템플릿 생성자
        template<typename T>
        explicit A(T val) {}

        // 일반 멤버 함수
        void f() {}

        // 멤버 함수 템플릿
        template<typename U> 
        U g(U left, U right) {return left + right;} 

        // 중첩 클래스 템플릿
        template<typename V> 
        class B {
        public:
            void h() {}
        };
    };
}

namespace Template_6 {
    template<typename T> 
    class A {
    public:    
        void f(); // 멤버 함수 선언

        template<typename U>
        U g(U val); // 멤버 함수 템플릿 선언
    };

    // 클래스 템플릿의 멤버 함수 정의
    template<typename T> // 클래스 템플릿
    void A<T>::f() {} 

    // 클래스 템플릿의 멤버 함수 템플릿수 정의 - 인자 집합 2개 필요
    template<typename T> // 바깥쪽 인자
    template<typename U> // 안쪽 인자
    U A<T>::g(U val) {return val + val;} 
}

TEST(TestLegacyCpp, Template) {
    {
        using namespace Template_1;

        // 템플릿 인스턴스부 - 이때 코드가 생성됨
        MyClass<int> myClass; 
        EXPECT_TRUE(myClass.Plus(10, 10) == 20);   
    }
    // 템플릿 정의부와 인스턴스부
    {
        using namespace Template_3;

        A<int> a;
        a.f(); // A<int>::g()를 사용하지 않았기에 컴파일 됨 
        // a.g(); // (X) 컴파일 오류. 함수 정의부가 없음
    }

    // 함수 템플릿 
    {
        using namespace Template_4;

        EXPECT_TRUE(Plus<int>(10, 10) == 20);
        EXPECT_TRUE(Plus<char>('a', 1) == 'b');
    }
    {
        using namespace Template_4;

        EXPECT_TRUE(Plus(10, 10) == 20); // (O) 인수로부터 int가 추론됨
        EXPECT_TRUE(Plus<>(10, 10) == 20); // (O) <>를 기재하여 함수 템플릿임을 명시하고, 인수로부터 추론
        // EXPECT_TRUE(Plus('a', 1) == 'b'); // (X) 컴파일 오류. 인수가 int, char로 각각 다르므로 추론이 어려움 
        EXPECT_TRUE(Plus('a', static_cast<char>(1)) == 'b'); // (O) 인수로부터 char 가 추론됨
    }   
    // 멤버 함수 템플릿과 중첩 클래스 템플릿
    {
        using namespace Template_5;
        int val;
        A a(val); // 템플릿 생성자 호출, 타입을 명시적으로 지정 못함
        a.f(); // 일반 함수 호출
        a.g(10, 10); // 멤버 함수 템플릿 호출 
        a.g<int>(10, 10); // 타입을 명시적으로 지정할 수 있음
        
        A::B<int> b; // 중첩 클래스 인스턴스화
        b.h(); 
    } 
    // 템플릿 선언과 정의 분리
    {
        using namespace Template_6;

        A<int> a;

        a.f(); // 일반 함수 호출
        a.g<int>(10); // 멤버 함수 템플릿 호출
    }
}

