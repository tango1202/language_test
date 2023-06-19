#include "gtest/gtest.h" 

TEST(TestClassicCpp, Const) {
    // 상수 변수 정의
    {
        // const int x; // (X) 컴파일 오류. 초기값 없음
        const int x = 10; // (O) x의 값은 변경될 수 없음
    }
    {
        int x = 10; 
        const int* p1; // (O) *p를 수정할 수 없음. *p = 30; 불가
        int const* p2; // (O) const int* 와 동일(자주 사용하지 않는 형식)
        int* const p3 = &x; // (O) p의 값을 수정할 수 없음. *p = 30;은 가능
        const int* const p4 = &x; // (O) p, *p를 수정할 수 없음.
    }
    {
        class T {
            int m_X;
        public:
            // (O) 멤버 변수의 값을 리턴하는 const 함수
            int GetX1() const { return m_X; } 

            // (X) 비권장. 멤버 변수의 값을 쓸데없이 const로 리턴하는 const 함수. 
            // int k = t.GetX2(); 로 실행하므로 const int 리턴은 무의미함. 
            // operator ++() 에서는 유효할 수도 있다. 연산자 오버로딩 참고
            const int GetX2() const { return m_X; }            
 
            // (O) 멤버 변수의 값을 수정하지 않는 const 함수
            const int* GetX3() const { return &m_X; }    

            // (X) 비권장. 멤버 변수의 값을 몰래 수정할 수 있는 const 함수
            int* GetX4() const { return const_cast<int*>(&m_X); }

            // (O) 맴버 변수의 값을 수정하는 none-const 함수      
            int* GetX5() { return &m_X; } 	 

        };
    }
    {
        void f(int x); // (O) 인수를 x에 복사해서 사용함.
        void f(const int x); // (X) 비권장. 인수를 x에 복사해서 쓰되 f에서 수정하지 않음. 호출하는 쪽에선 무의미

        void f(int* x); void f(int& x); // (O) 인수를 f에서 수정함.
        void f(const int* x); void f(const int& x); // (O) 인수를 f에서 수정하지 않음.  
    }

    // EXPECT_TRUE(arr2[2] == 2);
    // EXPECT_TRUE(arr3[0] == 0 && arr3[1] == 0 && arr3[2] == 0);
    // EXPECT_TRUE(arr4[2] == 0);
}
TEST(TestClassicCpp, Mutable) {
    class T {
    public:
        mutable std::wstring m_Lazy; // const 함수에서 수정할 수 있습니다.
        
        // 개념적으로 내부 String을 리턴하므로 const 함수
        // 하지만 내부에서 m_Lazy를 세팅하기 때문에 mutable을 사용합니다.
        const std::wstring& GetString() const {
            m_Lazy = L"Lazy String";
            return m_Lazy;
        }
    };
    T t;
    EXPECT_TRUE(t.GetString() == L"Lazy String");
}