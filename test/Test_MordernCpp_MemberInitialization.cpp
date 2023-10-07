#include "gtest/gtest.h" 

namespace {
    class A {
    public:
        explicit A(int, int) {}    
    };

    class T {
        int m_A = 0; // C++11. 멤버 변수 초기화
        int m_B{0}; // C++11. 중괄호 초기화
        int m_C = {0}; // C++11. 중괄호 초기화
        int* m_D = nullptr; // C++11. 포인터 초기화
        std::vector<int> m_E{1,2,3}; // C++11. 중괄호 초기화
        A m_F{10, 20}; // C++11. 중괄호 초기화
        // A m_F(10, 20); // (X) 컴파일 오류. A를 리턴하는 m_F 함수 정의

        const int m_G = 0; // 기존과 동일하게 상수 멤버 변수 초기화
        static const int m_H = 0; // 기존과 동일하게 선언부에 정적 상수 멤버 변수 초기화
        // static int m_H = 0; // (X) 컴파일 오류. 기존과 동일하게 정적 멤버 변수는 별도 초기화해야 함 
    };    
}

TEST(TestMordern, MemberInitialization) {
    // C++14 비정적 멤버 변수의 멤버 선언부 초기화시 집합 초기화 허용
    {
        class A_14 {
        public:
            int m_X{0};
            int m_Y{1};
        };

        A_14 a{0, 1}; // (X) 컴파일 오류. no matching function for call to 'main()::A::A(<brace-enclosed initializer list>)'
        EXPECT_TRUE(a.m_X == 0 && a.m_Y == 1); 
    }

}