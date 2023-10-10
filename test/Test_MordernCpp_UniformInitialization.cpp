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
        return {10, 'b'}; // return T{10, 'b'}; 와 동일
    }
}
namespace UniformInitialization_2 {
    class T {
    public:
        T() {std::cout<<"T : Default Constructor"<<std::endl;}
        T(const T&) {std::cout<<"T : Copy Constructor"<<std::endl;}
        T& operator =(const T&) {std::cout<<"T : operator =()"<<std::endl;return *this;}
    };
    void f(T param) {}
}

TEST(TestMordern, UniformInitialization) {
    // 중괄호 초기화
    {
        class T {
            int m_A;
            char m_B;
        public:
            T() {}
            T(int a, char b) : m_A(a), m_B(b) {}    
        };
        T obj(); //(△) 비권장. T를 리턴하는 obj 함수 선언

        T obj1; // 기본 생성자로 T 개체 생성
        T obj2(T()); // 기본 생성자인 T()로 생성한 개체를 obj의 복사 생성자로 복사 생성
        T obj3 = T(); // T obj(T());와 동일
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
        T obj2_11{T{}}; // 기본 생성자인 T()로 생성한 개체를 obj의 복사 생성자로 복사 생성
        T obj3_11 = T{}; // T obj = T();와 동일
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
        T a_11{}; // T a(); 는 T를 리턴하는 함수 a의 정의로 인식되어 컴파일 오류가 났었습니다.
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
                m_A_11{a}, // 생성자 초기화 리스트에서 멤버 변수 초기화에서 사용합니다.
                m_B_11{b} {}
        };
    }
    // 중괄호 복사 초기화
    {
        class T {
        public:
            T() {std::cout<<"T : Default Constructor"<<std::endl;}
            T(const T&) {std::cout<<"T : Copy Constructor"<<std::endl;}
            T& operator =(const T&) {std::cout<<"T : operator =()"<<std::endl;return *this;}
        };

        T a_11{}; // 기본 생성자 호출
        T b_11 = {}; // T b_11 = T{};와 동일
        T c_11 = T{}; // T c_11{T{}} 와 동일. 즉 T 기본 생성자를 호출하고, 복사 생성자를 호출. 컴파일러 최적화에 의해 2개의 생성을 1개의 생성으로 바꿈    }
    }
    {
        class T {
        public:
            T() {std::cout<<"T : Default Constructor"<<std::endl;}
            T(const T&) {std::cout<<"T : Copy Constructor"<<std::endl;}
            T& operator =(const T&) {std::cout<<"T : operator =()"<<std::endl;return *this;}
        };

        T t_11;
        t_11 = {}; // t = T{};과 동일  
    }
    {
        using namespace UniformInitialization_2;  
        f({}); // f(T{}); 와 동일   
    }
    {
        int arr[] = {0, 1, 2}; // 초기화 갯수 만큼 배열 할당
        int arr_11[]{0, 1, 2}; // 초기화 갯수 만큼 배열 할당     
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
        explicit T(int) {};
        };

        T a(3.14); // (△) 비권장. double 이 int로 암시적으로 변환되어 초기화 됩니다.
        // T b_11{3.14}; // (X) 컴파일 오류. 암시적 변환을 차단합니다.
    }
    {
        class T {
        public:
            explicit T(float) {};
        };  

        T a_11{3.14}; // 3.14는 float이 저장할 수 있어서 허용

        double doublVal = 3.14;   
        // T b_11{doublVal}; // (X) 컴파일 오류. double을 float으로 변환하는건 차단합니다.
    }
    {
        class T {
        public:
            explicit T(char) {};
        }; 
        
        T a_11{10}; // 10은 char에서 저장할 수 있어서 허옹
        // T b_11{255}; // (X) 컴파일 오류. 255는 char에서 저장할 수 없음. 

        int intVal = 10;  
        // T c_11{intVal}; // (X) 컴파일 오류. int를 char로 변환하는건 차단합니다.    
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
        class A {
            int m_X;
            int m_Y;
        public:
            A(int x, int y) : m_X(x), m_Y(y) {}
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
            A(int x, int y) : m_X(x), m_Y(y) {}
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
            A(int x, int y) : m_X(x), m_Y(y) {}
        };
        class B {
            int m_Val;
            A m_A;
        public:
           B(int val, A a) : m_Val(val), m_A(a){}   
        };

        B b_11{1, {2, 3}}; // B b_11{1, A{2, 3}};와 동일
    
    }
    {
        class A {
        public:
            int m_X;
            int m_Y;
        };
        class B {
        public:
            int m_Val;
            A m_A;
        };

        B b_11{1, {2, 3}}; // B b_11 = B{1, A{2, 3}};와 동일       
    }

    // initializer_list
    {
        // 이전 방식
        std::vector<int> v;
        v.push_back(1);
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
        T_11 t{{1, 2, 3}}; // {1, 2, 3} 은 initializer_list를 생성해서 전달합니다.
    }
    {
        std::initializer_list<int> data_11;
        data_11 = {1, 2, 3}; // {1, 2, 3} 은 initializer_list를 생성해서 전달합니다.         
        
        std::vector<int> v;
        auto itr = data_11.begin();
        auto endItr = data_11.end();
        for (;itr != endItr; ++itr) {
            v.push_back(*itr);
        }
    }
    {
        std::vector<int> v;
        for (auto a_11 : {1, 2, 3}) { // {1, 2, 3} 은 initializer_list를 생성하고 범위 기반 for문에서 사용됩니다.
            v.push_back(a_11);
        }        
    }
    // 중괄호 초기화 우선 순위
    {
        char arr_11[] = {"abc"}; // char arr_11[] = "abc"; 와 동일
    }
    {
        class T_11 {
        public:
            T_11(int, int, int, int, int) {}
            T_11(std::initializer_list<int>) {}
            T_11(std::initializer_list<int>, int, int) {}
        };
        T_11 a{1, 2, 3, 4, 5}; // T_11(std::initializer_list<int>)
        T_11 b{ {1, 2, 3}, 4, 5}; // T_11(std::initializer_list<int>, int, int)
    } 
    // 기존 생성자와 initializer_list 생성자와의 충돌
    {
        // 요소가 2개인 vector를 생성합니다.
        std::vector<int> v(2);
        EXPECT_TRUE(v.size() == 2 && v[0] == 0 && v[1] == 0);

        // 요소값이 2인 vector를 생성합니다.
        std::vector<int> v_11{2};
        EXPECT_TRUE(v_11.size() == 1 && v_11[0] == 2);        
    }

    // (C++17~) 중괄호 초기화에서 auto 추론의 새로운 규칙
    {
        int a_17{1}; // a는 int
        auto b_17{1}; // b는 int
        auto c_17 = {1}; // c는 initializer_list<int>
        auto d_17 = {1, 2}; // d는 initializer_list<int>  
        // auto e_11{1, 2}; // (X) 컴파일 오류. auto에서는 단일 개체 대입 필요  
    }


}