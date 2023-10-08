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

    T Func() {
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

        T obj1{}; // 기본 생성자로 T 개체 생성
        T obj2(T{}); // 기본 생성자인 T()로 생성한 개체를 obj의 복사 생성자로 복사 생성
        T obj3 = T{}; // T obj = T();와 동일
        T obj4{10, 'b'}; // // m_A == 10, m_B == `b`인 T 개체 생성

        T arr[]{T{}, T{10, 'b'}}; // T 요소 2개인 배열 생성

        struct U {
            int m_A;
            char m_B;
        };
        U objs{10, 'b'}; // m_A == 10, m_B == `b`인 U 개체 생성       
    }
    // 중괄호 직접 초기화
    {
        class T {
        public:
            T() {}
            T(int, char) {}
        };
        T a{}; // T a(); 는 T를 리턴하는 함수 a의 정의로 인식되어 컴파일 오류가 났었습니다.
        T b{10, 'a'}; // T(int, char)로 생성합니다.
        T{10, 'a'}; // T(int, char) 로 임시 개체를 생성합니다.
        T* c = new T{10, 'a'}; // new 시 T(int, char)로 생성한 포인터를 d에 저장합니다.
        delete c;
    }
    {
        class T {
        public:
            int m_A{0}; // 멤버 변수를 초기화합니다.
            char m_B{'a'}; 
        };       
    }
    {
        class T {
        public:
            int m_A; 
            char m_B;
        public:
            T(int a, char b) :
                m_A{a}, // 생성자 초기화 리스트에서 멤버 변수 초기화에서 사용합니다.
                m_B{b} {}
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

        T a{}; // 기본 생성자 호출
        T b = {}; // T b = T{};와 동일
        T c = T{}; // T c{T{}} 와 동일. 즉 T 기본 생성자를 호출하고, 복사 생성자를 호출. 컴파일러 최적화에 의해 2개의 생성을 1개의 생성으로 바꿈
    }
    {
        class T {
        public:
            T() {std::cout<<"T : Default Constructor"<<std::endl;}
            T(const T&) {std::cout<<"T : Copy Constructor"<<std::endl;}
            T& operator =(const T&) {std::cout<<"T : operator =()"<<std::endl;return *this;}
        };

        T t;
        t = {}; // t = T{};과 동일  
    }
    {
        using namespace UniformInitialization_2;  
        f({}); // f(T{}); 와 동일   
    }
    {
        int arr1[] = {0, 1, 2}; // 초기화 갯수 만큼 배열 할당
        int arr2[]{0, 1, 2}; // 초기화 갯수 만큼 배열 할당     
    }
    {
        int arr1[3] = {0, 1,}; // 초기값이 모자르면 0으로 채움
        int arr2[3]{0, 1,}; // 초기값이 모자르면 0으로 채움
        EXPECT_TRUE(arr1[2] == 0 && arr2[2] == 0);
    }
    // 인자의 암시적 형변환 차단
    {
        class T {
        public:
        explicit T(int) {};
        };

        T a(3.14); // (△) 비권장. double 이 int로 암시적으로 변환되어 초기화 됩니다.
        // T b{3.14}; // (X) 컴파일 오류. 암시적 변환을 차단합니다.
    }
    {
        class T {
        public:
            explicit T(float) {};
        };  

        T a{3.14}; // 3.14는 float이 저장할 수 있어서 허용

        double doublVal = 3.14;   
        // T b{doublVal}; // (X) 컴파일 오류. double을 float으로 변환하는건 차단합니다.
    }
    {
        class T {
        public:
            explicit T(char) {};
        }; 
        
        T a{10}; // 10은 char에서 저장할 수 있어서 허옹
        // T b{255}; // (X) 컴파일 오류. 255는 char에서 저장할 수 없음. 

        int intVal = 10;  
        // T c{intVal}; // (X) 컴파일 오류. int를 char로 변환하는건 차단합니다.    
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
        T t{a}; // (△) A->B로의 암시적 변환을 허용하면 차단되지 않습니다.
    }

    // initializer_list
    {
        // 이전 방식
        std::vector<int> v1;
        v1.push_back(1);
        v1.push_back(2);
        EXPECT_TRUE(v1[0] == 1 && v1[1] == 2);

        // {} 를 이용하여 initializer_list로 초기화
        std::vector<int> v2{1, 2};
        EXPECT_TRUE(v2[0] == 1 && v2[1] == 2);       

        // = {} 형태도 지원
        std::vector<int> v3 = {1, 2};
        EXPECT_TRUE(v1[0] == 1 && v1[1] == 2); 
    }
    // initializer_list의 암시적 생성
    {
        class T {
        public:
            explicit T(std::initializer_list<int>) {}
        };
        T t({1, 2, 3}); // {1, 2, 3} 은 initializer_list를 생성해서 전달합니다.
    }
    {
        std::initializer_list<int> data;
        data = {1, 2, 3}; // {1, 2, 3} 은 initializer_list를 생성해서 전달합니다.         
        
        std::vector<int> v;
        auto itr = v.begin();
        auto endItr = v.end();
        for (;itr != endItr; ++itr) {
            v.push_back(*itr);
        }
    }
    {
        std::vector<int> v;
        for (auto a : {1, 2, 3}) { // {1, 2, 3} 은 initializer_list를 생성하고 범위 기반 for문에서 사용됩니다.
            v.push_back(a);
        }        
    }
    // auto와 initializer_list
    {
        int a{1}; // a는 int
        auto b{1}; // b는 int

        std::string s = typeid(b).name();



        auto c = {1}; // c는 initializer_list<int>
        // auto d{1, 2}; // (X) 컴파일 오류. auto에서는 단일 개체 대입 필요     
        auto e = {1, 2}; // e는 initializer_list<int>  

        std::cout << "a type : " << typeid(a).name() << std::endl;
        std::cout << "b type : " << typeid(b).name()  << std::endl;
        std::cout << "c type : " << typeid(c).name()  << std::endl;
        std::cout << "e type : " << typeid(e).name()  << std::endl;
    }
    // 중괄호 초기화 우선 순위
    {
        char arr[] = {"abc"}; // char arr[] = "abc"; 와 동일
    }
    {
        class T {
        public:
            T(int, int, int, int, int) {}
            T(std::initializer_list<int>) {}
            T(std::initializer_list<int>, int, int) {}
        };
        T a{1, 2, 3, 4, 5}; // T(std::initializer_list<int>)
        T b{ {1, 2, 3}, 4, 5}; // T(std::initializer_list<int>, int, int)
    } 
    // 기존 생성자와 initializer_list 생성자와의 충돌
    {
        // 요소가 2개인 vector를 생성합니다.
        std::vector<int> v1(2);
        EXPECT_TRUE(v1.size() == 2 && v1[0] == 0 && v1[1] == 0);

        // 요소값이 2인 vector를 생성합니다.
        std::vector<int> v2{2};
        EXPECT_TRUE(v2.size() == 1 && v2[0] == 2);        
    }


}