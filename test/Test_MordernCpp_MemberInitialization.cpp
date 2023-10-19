#include "gtest/gtest.h" 

namespace {
    class A {
    public:
        explicit A(int, int) {}    
    };

    class T {
        int m_A_11 = 0; // C++11. 멤버 변수 초기화
        int m_B_11{0}; // C++11. 중괄호 초기화
        int m_C_11 = {0}; // C++11. 중괄호 초기화
        int* m_D__1 = nullptr; // C++11. 포인터 초기화
        std::vector<int> m_E_11{1,2,3}; // C++11. 중괄호 초기화
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
        class A {
        public:
            int m_X_11{0}; // 비정적 멤버 변수 초기화
            int m_Y_11{1};
        };

        // (X) ~C++14 컴파일 오류. no matching function for call to 'main()::A::A(<brace-enclosed initializer list>)'
        // (O) C++14~
        A a_14{0, 1}; // 생성자를 호출하는 중괄호 직접 초기화가 아니라 중괄호 집합 초기화 입니다.
        EXPECT_TRUE(a_14.m_X_11 == 0 && a_14.m_Y_11 == 1); 
    }

}