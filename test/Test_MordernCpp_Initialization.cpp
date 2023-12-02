#include "gtest/gtest.h"

namespace UniformInitialization_1 {
    // 리턴문 작성 간소화
    class T {
        int m_A;
        char m_B;
    public:
        T() {}
        T(int a, char b) : m_A(a), m_B(b) {}
    };

    T Func_11() {
        return {10, 'b'}; // return T{10, 'b'}; 와 동일. 임시 개체가 생성되어 전달됩니다.
    }
}
namespace UniformInitialization_2 {
    class T {
    public:
        T() {std::cout << "T : Default Constructor" << std::endl;}
        T(const T&) {std::cout << "T : Copy Constructor" << std::endl;}
        T& operator =(const T&) {std::cout << "T : operator =()" << std::endl;return *this;}
    };
    void f(T param) {}
}

TEST(TestMordern, UniformInitialization) {
    // 개요
    {
        int a = 10;

        int b(10); // 괄호 초기화
        int b_11{10}; // 중괄호 직접 초기화
        int c_11 = {10}; // 증괄호 복사 초기화
    }
    // 중괄호 초기화
    {
        class T {
            int m_A;
            char m_B;
        public:
            T() {}
            T(int a, char b) : m_A(a), m_B(b) {}
        };
        // T obj(); // (△) 비권장. T를 리턴하는 obj 함수 선언

        T obj1; // 기본 생성자로 T 개체 생성
        // T obj2(T()); // (△) 비권장. T 타입의 기본 생성자로 생성한 것을 T obj2에 복사 생성하고 싶지만, T 타입을 리턴하고, T(*)()함수 포인터를 인자로 전달받는 함수 obj2를 선언합니다.
        T obj3 = T(); // T obj(T());와 유사. T()로 기본 생성된 것을 T obj3에 복사 생성. 단 컴파일러 최적화로 1회만 생성될 수 있음
        T obj4(10, 'b'); // m_A == 10, m_B == `b`인 T 개체 생성

        T arr[] = {T(), T(10, 'b')}; // T 요소 2개인 배열 생성

        struct U {
            int m_A;
            char m_B;
        };
        U objs = {10, 'b'}; // m_A == 10, m_B == `b`인 U 개체 생성
    }
    {
        class T {
            int m_A;
            char m_B;
        public:
            T() {}
            T(int a, char b) : m_A(a), m_B(b) {}
        };

        T obj1_11{}; // 기본 생성자로 T 개체 생성
        T obj2_11{T{}}; // 기본 생성자인 T()로 생성한 개체를 obj2_11의 복사 생성자로 복사 생성
        T obj3_11 = T{}; // T obj2_11{T{}}와 유사. T{}로 기본 생성된 것을 T obj2_11 복사 생성. 단 컴파일러 최적화로 1회만 생성될 수 있음
        T obj4_11{10, 'b'}; // T(int a, char b) 생성자 호출. m_A == 10, m_B == `b`인 T 개체 생성

        T arr_11[]{T{}, T{10, 'b'}}; // T 요소 2개인 배열 생성

        struct U {
            int m_A;
            char m_B;
        };
        U objs_11{10, 'b'}; // m_A == 10, m_B == `b`인 U 개체 생성
    }
    // 중괄호 직접 초기화
    {
        class T {
        public:
            T() {}
            T(int, char) {}
        };
        // T a(); // (△) 비권장. T를 리턴하는 함수 a의 선언입니다.
        T a_11{}; // (O) T의 기본 생성자로 생성합니다.

        T b_11{10, 'a'}; // T(int, char)로 생성합니다.
        T{10, 'a'}; // T(int, char) 로 임시 개체를 생성합니다.
        T* c_11 = new T{10, 'a'}; // new 시 T(int, char)로 생성한 포인터를 d에 저장합니다.
        delete c_11;
    }
    {
        class T {
        public:
            int m_A_11{0}; // 멤버 변수를 초기화합니다.
            char m_B_11{'a'};
        };
    }
    {
        class T {
        public:
            int m_A_11;
            char m_B_11;
        public:
            T(int a, char b) :
                m_A_11{a}, // 생성자 초기화 리스트에서 사용합니다.
                m_B_11{b} {}
        };
    }
    // 중괄호 복사 초기화
    {
        class T {
        public:
            T(int) {}
            T(int, int) {}
        };

        // 암시적으로 T(int), T(int, int) 생성자를 호출하고, a_11과 b_11, c_11에 복사 생성합니다.
        T a_11 = {1}; 
        T b_11 = {1, 2};
        T c_11 = T{1, 2};

        class U {
        public:
            explicit U(int) {}
            explicit U(int, int) {} // 생성자의 인자가 여러개이더라도 = {} 표현을 차단하기 위해 explicit를 사용합니다.
        };

        // (X) 컴파일 오류. explicit 생성자여서 암시적으로 생성자를 호출할 수 없습니다.
        // U d_11 = {1};         
        // U e_11 = {1, 2};
        U f_11 = U{1, 2}; // 명시적으로 생성할 있습니다. 중괄호 직접 초기화인 U{1, 2}을 f_11에 복사 생성하는 표현입니다.
    }
    {
        class T {
        public:
            T() {}
            T(int) {}
            T(const T&) = delete; // 복사 생성자를 사용할 수 없습니다.
        };

        T a_11{10};
        // T b_11 = {10}; // (X) 컴파일 오류. 복사 생성자를 사용할 수 없습니다.
    }
    {
        class T {
        public:
            T() {std::cout << "T : Default Constructor" << std::endl;}
            T(const T&) {std::cout << "T : Copy Constructor" << std::endl;}
            T& operator =(const T&) {std::cout << "T : operator =()" << std::endl;return *this;}
        };

        T a_11{}; // 기본 생성자 호출
        T b_11 = T{}; // T 기본 생성자를 호출하고, 복사 생성자를 호출. 컴파일러 최적화에 의해 2개의 생성을 1개의 생성으로 바꿈 
        T c_11 = {}; // T c_11 = T{}; 와 동일
    }

    {
        class T {
        public:
            T() {std::cout << "T : Default Constructor" << std::endl;}
            T(const T&) {std::cout << "T : Copy Constructor" << std::endl;}
            T& operator =(const T&) {std::cout << "T : operator =()" << std::endl;return *this;}
        };

        T t_11;
        t_11 = {}; // t = T{};과 동일. 임시 개체가 생성되어 대입됩니다.
    }
    {
        using namespace UniformInitialization_2;
        f({}); // f(T{}); 와 동일. 임시 개체가 생성되어 전달됩니다.
    }

    // 중괄호 집합 초기화
    {
        class T {
        private:
            int m_X;
            int m_Y;
        public:
            T(int x, int y) : m_X(x), m_Y(y) {}
        };

        T a_11{1, 2}; // 중괄호 직접 초기화. 생성자를 호출합니다.
        T b_11 = {1, 2}; // 중괄호 복사 초기화. T(int x, int y)를 이용해서 생성하고 복사 생성자를 호출합니다.

        class U { // 집합 타입은 사용자 정의 생성자와 소멸자가 없어야 합니다.
        public: // 집합 타입은 모든 멤버 변수가 public 이어야 합니다
            int m_X;
            int m_Y;
        };

        U c_11{1, 2}; // 중괄호 집합 초기화. 생성자가 없더라도 멤버 변수들을 직접 초기화 합니다. 
        U d_11 = {1, 2}; // U d_11{1, 2}; 과 동일
    }
    {
        int arr[] = {0, 1, 2}; // 초기화 갯수 만큼 배열 할당
        int arr_11[]{0, 1, 2}; // 초기화 갯수 만큼 배열 할당
        int* ptr_11 = new int[3]{1, 2, 3}; // new[]로 생성하는 경우에는 배열 크기를 명시해야 합니다.
        delete ptr_11;
    }
    {
        int arr[3] = {0, 1,}; // 초기값이 모자르면 0으로 채움
        int arr_11[3]{0, 1,}; // 초기값이 모자르면 0으로 채움
        EXPECT_TRUE(arr[2] == 0 && arr_11[2] == 0);
    }
    // 인자의 암시적 형변환 차단
    {
        class T {
        public:
        explicit T(int) {}
        };

        T a(3.14); // (△) 비권장. 3.14는 double입니다. int로 암시적으로 변환되어 초기화 됩니다.
        // T b_11{3.14}; // (X) 컴파일 오류. 암시적 변환을 차단합니다.
    }
    {
        class T {
        public:
        explicit T(double) {}
        };

        T a(3); // (△) 비권장. int가 double로 승격 변환되어 초기화 됩니다.
        T b_11{3}; // (△) 비권장. int가 double로 승격 변환되어 초기화 됩니다.
    }
    {
        class T {
        public:
            explicit T(float) {};
        };

        double doubleVal = 3.14;
        // T a_11{doubleVal}; // (X) 컴파일 경고. double을 float으로 변환하는건 경고합니다.
        
        T b_11{3.14}; // 상수 표현식은 값 범위라면 허용. 3.14는 float이 저장할 수 있어서 허용
    }
    {
        class T {
        public:
            explicit T(char) {};
        };

        int intVal = 10;
        // T a_11{intVal}; // (X) 컴파일 경고. int를 char로 변환하는건 경고합니다.

        T b_11{10}; // 상수 표현식은 값 범위라면 허용. 10은 char에서 저장할 수 있어서 허옹
        // T c_11{255}; // (X) 컴파일 오류. 255는 char에서 저장할 수 없음.
    }
    {
        int* ptr;
        bool b(ptr);
        // bool b_11{ptr}; // (X) 컴파일 경고. int*에서 bool로 변환하는건 경고합니다.
    }
    {
        class A {};
        class B {
        public:
            B(A) {} // explicit가 없는 형변환 생성자. A로 암시적으로 생성됩니다.
        };
        class T {
        public:
            explicit T(B) {}
        };

        A a;
        T t_11{a}; // (△) A->B로의 암시적 변환을 허용하면 차단되지 않습니다.
    }
    // 중괄호 초기화 중첩
    {
        {
            int a = 10;
            int b(10);        
        }
        {
            int a(int(10)); // int(10)으로 생성한 개체를 int a 의 복사 생성자를 호출하여 생성합니다.
            int b = int(10); // int(10)으로 생성한 개체를 int a 의 복사 생성자를 호출하여 생성합니다.
        }
        // 중괄호 변환
        {
            int a_11{
                int{10}
            };
            int b_11 = int{10};
        }
        // int{10}에서 int를 생략
        {
            // int c_11{ // (X) 컴파일 오류. 기본 타입은 중괄호 중첩을 지원하지 않습니다.
            //    {10}
            // };
            int d_11 = {10};
        }

        {
            class T {
            public:
                T(int) {}
            };

            {
                T a_11{ 
                    T{10} // (O)
                };
                T b_11 = T{10}; // (O)
            }
            {
                // 축약형
                T a_11{ 
                    {10} // (O) T{10}
                }; 
                T b_11 = {10}; // (O)
            }
        }
    }    
    {
        class A {
            int m_X;
            int m_Y;
        public:
            A(int x, int y) : m_X{x}, m_Y{y} {}
        };

        A arr_11[]{
            A{1, 2}, // 배열의 첫번째 요소
            A{2, 3} // 배열의 두번째 요소
        };
    }
    {
        class A {
            int m_X;
            int m_Y;
        public:
            A(int x, int y) : m_X{x}, m_Y{y} {}
        };

        A arr_11[]{
            {1, 2}, // A{1, 2} 와 동일
            {2, 3} // A{2, 3} 과 동일
        };
    }
    {
        class A {
            int m_X;
            int m_Y;
        public:
            A(int x, int y) : m_X{x}, m_Y{y} {}
        };
        class B {
            int m_Val;
            A m_A;
        public:
           B(int val, A a) : m_Val{val}, m_A{a} {}
        };

        B b_11{1, {2, 3}}; // B b_11{1, A{2, 3}};와 동일

    }
    {
        class A {
        public:
            int m_X; // 내부 멤버가 public인 집합 타입이면 값 생성자가 없어도 됩니다.
            int m_Y;
        };
        class B {
        public:
            int m_Val; // 내부 멤버가 public인 집합 타입이면 값 생성자가 없어도 됩니다.
            A m_A;
        };

        B b_11{1, {2, 3}}; // B b_11 = B{1, A{2, 3}};와 동일
    }

    // initializer_list
    {
        // 이전 방식
        std::vector<int> v;
        v.push_back(1); // 일일이 push_back을 작성해야 해서 번거롭습니다.
        v.push_back(2);
        EXPECT_TRUE(v[0] == 1 && v[1] == 2);

        // {} 를 이용하여 initializer_list로 초기화
        std::vector<int> v1_11{1, 2};
        EXPECT_TRUE(v1_11[0] == 1 && v1_11[1] == 2);

        // = {} 형태도 지원
        std::vector<int> v2_11 = {1, 2};
        EXPECT_TRUE(v2_11[0] == 1 && v2_11[1] == 2);
    }
    // initializer_list의 암시적 생성
    {
        class T_11 {
        public:
            explicit T_11(std::initializer_list<int>) {}
        };
        T_11 t{
            {1, 2, 3} // {1, 2, 3} 은 initializer_list를 생성해서 전달합니다.
        };
    }
    {
        class T_11 {
        public:
            void operator =(std::initializer_list<int> data_11) { // 복사 대입 연산자가 initializer_list를 전달받습니다.
                std::vector<int> v;
                const int* itr = data_11.begin();
                const int* endItr = data_11.end();
                for (;itr != endItr; ++itr) {
                    v.push_back(*itr);
                }
            }
        };
        T_11 t;
        t = {1, 2, 3}; // {1, 2, 3} 은 initializer_list를 생성해서 전달합니다.
    }
    {
        std::vector<int> v;
        for (int a_11 : {1, 2, 3}) { // {1, 2, 3} 은 initializer_list를 생성하고 범위 기반 for문에서 사용됩니다.
            v.push_back(a_11);
        }
    }
    // 중괄호 초기화 우선 순위
    {
        char arr_11[] = {"abc"}; // char arr_11[] = "abc"; 와 동일
    }
    {
        class T { // 집합 타입은 사용자 정의 생성자와 소멸자가 없어야 합니다.
        public: // 집합 타입은 모든 멤버 변수가 public 이어야 합니다.
            int m_X;
            int m_Y;
        };
        
        T a{1, 2};
        T b = {10, 20};

        EXPECT_TRUE(a.m_X == 1 && a.m_Y == 2);
        EXPECT_TRUE(b.m_X == 10 && b.m_Y == 20);         
    }
    {
        class T {
        private: // 집합 타입이 아닙니다. 집합 타입은 모든 멤버 변수가 public 이어야 합니다.
            int m_X;
            int m_Y;
        };
        
        // T a{1, 2}; // (X) 컴파일 오류. 집합 타입이 아니어서 생성자를 호출하는데, 생성자가 없습니다.
        // T b = {10, 20}; // (X) 컴파일 오류. 집합 타입이 아니어서 생성자를 호출하는데, 생성자가 없습니다.        
    }
    {
        // std::vector<int> v(); // (△) 비권장.  std::vector<int>를 리턴하는 함수 v의 선언입니다.
        std::vector<int> v_11{}; // 기본 생성자        
    }
    {
        class T_11 {
        public:
            explicit T_11(int, int, int, int, int) {}
            explicit T_11(std::initializer_list<int>) {}
            explicit T_11(std::initializer_list<int>, int, int) {}
        };
        T_11 a{1, 2, 3, 4, 5}; // T_11(std::initializer_list<int>)
        T_11 b{
            {1, 2, 3}, 4, 5 // T_11(std::initializer_list<int>, int, int)
        }; 
    }
    // 기존 생성자와 initializer_list 생성자와의 충돌
    {
        class T_11 {
        public:
            T_11() {std::cout << "T_11::Default Constructor" << std::endl;}
            T_11(const T_11&) {std::cout << "T_11::Copy Constructor" << std::endl;}
            T_11(std::initializer_list<int>) {std::cout << "T_11::initializer_list Constructor" << std::endl;}    
        };
        T_11 t{
            {} // initializer_list를 암시적으로 생성합니다.
        };        
    }
    {
        class T_11 {
        public:
            T_11() {std::cout << "T_11::Default Constructor" << std::endl;}
            T_11(const T_11&) {std::cout << "T_11::Copy Constructor" << std::endl;}
            T_11(std::initializer_list<int>) {std::cout << "T_11::initializer_list Constructor" << std::endl;}    
        };
        T_11 t{
            T_11{} // 명시적으로 사용했습니다.
        };        
    }    
    {
        std::vector<int> v1(2); // 요소 갯수가 2개인 vector 생성
        EXPECT_TRUE(v1.size() == 2 && v1[0] == 0 && v1[1] == 0);

        std::vector<int> v2_11{2}; // 요소값이 2인 vector 생성
        EXPECT_TRUE(v2_11.size() == 1 && v2_11[0] == 2);  
    }

    // 지명 초기화
    {
        class T_11 {
        public:
            int a, b, c, d, e;
        };

        T_11 obj_11{1, 2, 3, 4, 5}; // 갯수가 많아지면 어느값이 어느 멤버 변수를 초기화 하는지 헷갈립니다.
        T_11 obj_20{.a = 1, .b = 2, .c = 3, .d = 4, .e = 5}; // 초기화할 변수명과 값을 지명합니다.
    }
#if 202002L <= __cplusplus // C++20~
    {
        class T { // 집합 타입은 사용자 정의 생성자와 소멸자가 없어야 합니다.
        public: // 집합 타입은 모든 멤버 변수가 public 이어야 합니다.
            int m_Val;
        };  
        class U {
        private: // 집합 타입이 아닙니다. 집합 타입은 모든 멤버 변수가 public 이어야 합니다.
            int m_Val;
        };      
        class V {
        public:
            int m_Val;
            V() {}; // 사용자 정의 생성자가 있어서 집합 타입이 아닙니다.
        };

        T t_20{.m_Val = 1};
        // U u_20{.m_Val = 1}; // (X) 컴파일 오류. 집합 타입이 아닙니다.
        // V v_20{.m_Val = 1}; // (X) 컴파일 오류. 집합 타입이 아닙니다.        
    }
    {
        // 배열은 지원하지 않습니다.
        // int arr_20[3]{[1] = 1}; // (X) 컴파일 오류. 배열은 지원하지 않습니다.
    }
    {
        class T {
        public:
            int m_X;
            int m_Y;
            int m_Z;
        };
        T a_20{.m_X = 0, .m_Y = 1, .m_Z = 2}; // (O)
        // T b_20{.m_X = 0, .m_Z = 1, .m_Y = 2}; // (X) 컴파일 오류. m_Z와 m_Y가 선언 순서와 다릅니다.
    }
    {
        class T {
        public:
            int m_X;
            int m_Y;
        };  

        // T a_20{.m_X = 0, 1}; // (X) 컴파일 오류. m_X를 지명했으면, 다른 멤버도 지명해야 합니다.        
    }
    {
        class T {
        public:
            int m_X;
            int m_Y;
            int m_Z;
        };

        T t_20{.m_X = 0, .m_Z = 2}; // (O)
        EXPECT_TRUE(t_20.m_Y == 0); // 생략된 것은 기본 생성자로 초기화됩니다.        
    }

    {
        class Inner { // 집합 타입입니다.
        public:
            int m_Val1; 
            int m_Val2;
        };
        
        class T {
        public:
            int m_Val{0};
            Inner m_Inner; // 내부 개체 입니다.
        };        

        T a_20{0, {1, 2}}; // {1, 2}는 Inner 개체를 초기화 합니다.
        T b_20{.m_Val = 0, .m_Inner = {1, 2}}; // (O)
        // T c_20{.m_Val = 0, .m_Inner.m_Val1 = 1, .m_Inner.m_Val2 = 2}; // // (X) 컴파일 오류. 지명 초기화는 내부 개체를 지명할 수 없습니다.
    }
    {
        class A {};
        class B {
        public:
            B(A) {} // explicit가 없는 형변환 생성자. A로 암시적으로 생성됩니다.
        };
        class T {
            
        public:
            int m_X{1};
            int m_Y{2};
            float m_Z{3.F}; // float 입니다.
            bool m_Bool;
            B m_B{A{}}; // A 기본 생성자로 생성합니다.
        };   

        // T a_20{.m_X = 1.5}; // (X) 컴파일 오류. 실수를 정수로 변환시 컴파일 오류가 발생합니다.

        // double doubleVal{3.14};
        // T b_20{.m_Z = doubleVal}; // (X) 컴파일 경고. double을 float으로 변환하는건 경고합니다.
        T c_20{.m_Z = 3.14}; // 상수값을 저장할 수 있다면 허용합니다.

        int* ptr;
        // T d_20{.m_Bool = ptr}; // (X) 컴파일 경고. 포인터 타입에서 bool 변환하는건 경고합니다.

        T k_20{.m_B = A{}}; // (△) 비권장. A->B로의 암시적 변환을 허용하면 차단되지 않습니다.
    }
    // c언어 지명 초기화
    {
        // struct A { 
        //     int m_X;
        //     int m_Y; 
        // };
        // struct B { 
        //     A m_A; 
        // };

        // int arr_20[3] = {[1] = 5}; // valid C, invalid C++ (배열)   
        // A a_20 = {.m_Y = 1, .m_X = 2}; // valid C, invalid C++ (순서 불일치)
        // A b_20 = {.m_X = 1, 2}; // valid C, invalid C++ (혼합)
        // B c_20 = {.m_A.m_X = 0}; // valid C, invalid C++ (내부 개체 지명)
    }
#endif

    // new에서 중괄호 집합 초기화로 배열 크기 추론
    {

    int arr[]{1, 2, 3}; // 중괄호 집합 초기화로 배열 크기를 추론합니다.
    //int* arr_11 = new int[]{1, 2, 3}; // (X) 컴파일 오류. new[]에서 중괄호 집합 초기화로 배열 크기를 추론하지 못합니다.
    int* arr_11 = new int[3]{1, 2, 3}; // 배열 크기를 명시해야 합니다.
#if 202002L <= __cplusplus // C++20~ 
    int* arr_20 = new int[]{1, 2, 3}; // new[]를 사용해도 중괄호 집합 초기화로 배열 크기를 추론합니다.
#endif

    }
}

namespace MemberInitialization_1 {
    class A {
    public:
        explicit A(int, int) {}    
    };

    class T {
        int m_A_11 = 0; // C++11. 멤버 변수 초기화
        int m_B_11{0}; // C++11. 중괄호 초기화
        int m_C_11 = {0}; // C++11. 중괄호 초기화
        int* m_D__1 = nullptr; // C++11. 포인터 초기화
        std::vector<int> m_E_11{1, 2, 3}; // C++11. 중괄호 초기화
        A m_F_11{10, 20}; // C++11. 중괄호 초기화
        // A m_F_11(10, 20); // (X) 컴파일 오류. A를 리턴하는 m_F 함수 정의

        const int m_G_11 = 0; // 기존과 동일하게 상수 멤버 변수 초기화
        static const int m_H_11 = 0; // 기존과 동일하게 선언부에 정적 상수 멤버 변수 초기화
        // static int m_H_11 = 0; // (X) 컴파일 오류. 기존과 동일하게 정적 멤버 변수는 별도 초기화해야 함 
    };    
}

TEST(TestMordern, MemberInitialization) {
    // C++14 비정적 멤버 변수의 멤버 선언부 초기화시 집합 초기화 허용
    {
        class A { // 집합 타입은 사용자 정의 생성자와 소멸자가 없고, 모든 멤버 변수가 public 이어야 합니다.
        public: 
            int m_X;
            int m_Y;
        };

        A a_11{0, 1}; // 중괄호 집합 초기화
        EXPECT_TRUE(a_11.m_X == 0 && a_11.m_Y == 1);        
    }
    {
        using namespace MemberInitialization_1;
        class A_11 { // 집합 타입은 사용자 정의 생성자와 소멸자가 없고, 모든 멤버 변수가 public 이어야 합니다.
        public:
            int m_X{0}; // 비정적 멤버 선언부 초기화
            int m_Y{1};
        };

#if 201402L <= __cplusplus // C++14~    
        // (X) ~C++14 컴파일 오류. no matching function for call to 'A_11::A_11(<brace-enclosed initializer list>)'
        // (O) C++14~
        A_11 a_14{0, 1}; // A_11 에는 생성자가 없습니다.
                         // 따라서 생성자를 호출하는 중괄호 직접 초기화가 아니라 
                         // 중괄호 집합 초기화 입니다.
        EXPECT_TRUE(a_14.m_X == 0 && a_14.m_Y == 1); 
#endif        
    }
#if 202002L <= __cplusplus // C++20~ 
    // 비트 필트 초기화
    {
        class Flag_20 {
        public:
            unsigned char m_Val1 : 2{3}; // 2bit 이므로 값의 범위는 00(0), 01(1), 10(2), 11(3)
            unsigned char m_Val2 : 3{7}; // 3bit 이므로 값의 범위는 000(0), 001(1), 010(2), 011(3), 100(4), 101(5), 110(6), 111(7)
        };

        Flag_20 flag;
        EXPECT_TRUE(sizeof(flag) == sizeof(unsigned char));

        EXPECT_TRUE(flag.m_Val1 == 3); // 3 저장
        EXPECT_TRUE(flag.m_Val2 == 7); // 7 저장
    }
#endif


}