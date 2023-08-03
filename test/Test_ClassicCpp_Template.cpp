#include "gtest/gtest.h" 

namespace Template_1 {
    // 템플릿 클래스 정의부 - 코드가 생성되지 않은 상태
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
    // 멤버 엑세스 지정
    template<typename T> 
    class A {
    private:
        typedef T TType;
    };  

    template<typename T> 
    class B : public T { // T로 A<int> 를 전달할 예정임. A<int>::TType은 int이나 private여서 접근 불가
        typename T::TType m_Member; // typename은 종속 타입 참고
    };
}
namespace Template_4 {
    template<typename T>
    class A {
    public:
        void f() {}
        void g(); // 함수 선언만 됨. 불완전함. 함수 정의부가 없음
    };    
}
namespace Template_5 {
    template<typename T>
    class A {};

    template<typename T>
    using Alias = A<T>; // A의 별칭
}

namespace Template_6 {
    // 템플릿 함수    
    template <typename T>
    T Plus(T left, T right) {
        return left + right;
    }
}

namespace Template_7 {
    class A {
    public:
        // 템플릿 생성자
        template<typename T>
        explicit A(T val) {}

        // 일반 멤버 함수
        void f() {};

        // 템플릿 멤버 함수
        template<typename U> 
        U g(U left, U right) {return left + right;} 

        // 템플릿 중첩 클래스
        template<typename V> 
        class B {
        public:
            void h() {}
        };
    };
}

namespace Template_8 {
    template<typename T> 
    class A {
    public:    
        void f(); // 멤버 함수 선언

        template<typename U>
        U g(U val); // 템플릿 멤버 함수 선언
    };

    // 템플릿 클래스의 멤버 함수 정의
    template<typename T> // 템플릿 클래스
    void A<T>::f() {} 

    // 템플릿 멤버 함수 정의 - 인자 집합 2개 필요
    template<typename T> // 바깥쪽 인자
    template<typename U> // 안쪽 인자
    U A<T>::g(U val) {return val + val;} 
}

namespace Template_9 {
    template<typename T>
    class A {
    public:    
        int f(int) {return 1;} // #1. 멤버 함수
    
        template<typename U>
        int f(U) {return 2;} // #2. 템플릿 멤버 함수
    };    
}

TEST(TestClassicCpp, Template) {
    {
        using namespace Template_1;

        // 템플릿 인스턴스부 - 이때 코드가 생성됨
        MyClass<int> myClass; 
        EXPECT_TRUE(myClass.Plus(10, 10) == 20);   
    }
    // 템플릿 정의부와 인스턴스부
    {
        using namespace Template_3;
        // B<A<int>> b; // (X) 컴파일 오류. 템플릿 인스턴스화시 private 멤버 접근 오류 발생
    } 
    {
        using namespace Template_4;

        A<int> a;
        a.f(); // A<int>::g() 를 사용하지 않았기에 컴파일 됨 
        // a.g(); // (X) 컴파일 오류. 함수 정의부가 없음
    }
    // 템플릿 별칭 
    {
        using namespace Template_5;
    
        Alias<int> alias;
    } 
    // 템플릿 함수 
    {
        using namespace Template_6;

        EXPECT_TRUE(Plus<int>(10, 10) == 20);
        EXPECT_TRUE(Plus<char>('a', 1) == 'b');
    }
    {
        using namespace Template_6;

        EXPECT_TRUE(Plus(10, 10) == 20); // (O) 인수로부터 int가 추론됨
        EXPECT_TRUE(Plus<>(10, 10) == 20); // (O) <>를 기재하여 템플릿 함수를 명시하고, 인수로부터 추론
        // EXPECT_TRUE(Plus('a', 1) == 'b'); // (X) 인수가 int, char로 각각 다르므로 추론이 어려움 
        EXPECT_TRUE(Plus('a', static_cast<char>(1)) == 'b'); // (O) 인수로부터 char 가 추론됨
    }   
    // 템플릿 멤버 함수와 템플릿 중첩 클래스
    {
        using namespace Template_7;
        int val;
        A a(val); // 템플릿 생성자 호출, 타입을 명시적으로 지정 못함
        a.f(); // 일반 함수 호출
        a.g<int>(10, 10); // 템플릿 멤버 함수 호출
        
        A::B<int> b; // 중첩 클래스 인스턴스화
        b.h(); 
    } 
    // 템플릿 선언과 정의 분리
    {
        using namespace Template_8;

        A<int> a;

        a.f(); // 일반 함수 호출
        a.g<int>(10); // 템플릿 멤버 함수 호출
    }
    // 함수 오버로딩
    {
        using namespace Template_9;
        A<int> a;

        EXPECT_TRUE(a.f(10) == 1); // f(int) 호출
        EXPECT_TRUE(a.f<>(10) == 2); // f<int>(int) 호출
        EXPECT_TRUE(a.f('a') == 2); // f<char>(char) 호출
    }
}

