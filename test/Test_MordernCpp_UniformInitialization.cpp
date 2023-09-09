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
        T obj3 = T(); // T obj = T();와 동일
        T obj4(10, 'b'); // // m_A == 10, m_B == `b`인 T 개체 생성

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
    // 인자의 암시적 형변환 차단
    {
        class T {
        public:
        explicit T(int) {};
        };

        T obj1(3.14); // (△) 비권장. double 이 int로 암시적으로 변환되어 초기화 됩니다.
        // T obj2{3.14}; // (X) 컴파일 오류. 암시적 변환을 차단합니다.
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
    {
        // 요소가 2개인 vector를 생성합니다.
        std::vector<int> v1(2);
        EXPECT_TRUE(v1.size() == 2 && v1[0] == 0 && v1[1] == 0);

        // 요소값이 2인 vector를 생성합니다.
        std::vector<int> v2{2};
        EXPECT_TRUE(v2.size() == 1 && v2[0] == 2);        
    }
    {
        int a{1}; // a는 int
        auto b{1}; // b는 int
        auto c = {1}; // c는 initializer_list<int>
        // auto d{1, 2}; // (X) 컴파일 오류. auto에서는 단일 개체 대입 필요     
        auto e = {1, 2}; // e는 initializer_list<int>  
    }
}