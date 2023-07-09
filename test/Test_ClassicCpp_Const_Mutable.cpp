#include "gtest/gtest.h" 

TEST(TestClassicCpp, Const) {
    // ----
    // 상수 변수
    // ----
    {
        // const int x; // (X) 컴파일 오류. 초기값 없음
        const int x = 10; // (O) x의 값은 이제 변경될 수 없음
    }
    {
        int obj = 10;
        int* p1 = &obj; // *p1 수정 가능. p1 수정 가능
        *p1 = 20;

        const int* p2 = &obj; // *p2 수정 불가. p2 수정 가능
        // *p2 = 20; // (X) 컴파일 오류
        p2 = p1;

        int* const p3 = &obj; // *p3 수정 가능. p3 수정 불가
        *p3 = 20;
        // p3 = p1; // (X) 컴파일 오류

        const int* const p4 = &obj; // *p4 수정 불가. p4 수정 불가
        // *p4 = 20; // (X) 컴파일 오류
        // p4 = p1; // (X) 컴파일 오류
    }
    // ----
    // 리턴값의 상수성
    // ----
    {
        class T {
            int m_X;
        public:
            // (O) 멤버 변수의 값을 리턴하는 const 함수
            int GetX1() const {return m_X;} 

            // (△) 비권장. 리턴값을 쓸데없이 const로 리턴하는 const 함수. 
            const int GetX2() const {return m_X;}            
 
            // (O) 멤버 변수의 값을 수정하지 않는 const 함수
            const int* GetX3() const {return &m_X;}    

            // (△) 비권장. 멤버 변수의 값을 몰래 수정할 수 있는 const 함수
            int* GetX4() const {return const_cast<int*>(&m_X);}

            // (O) 맴버 변수의 값을 수정하는 none-const 함수      
            int* GetX5() {return &m_X;} 	 

        };
    }
    // ----
    // 인자(함수 선언에 작성된 Parameter)의 상수성
    // ----
    {
        void f(int x); // (O) 인수를 x에 복사해서 사용함.
        void f(const int x); // (△) 비권장. 인수를 x에 복사해서 쓰되 f에서 수정하지 않음. 호출하는 쪽에선 무의미

        void f(int* x); // (O) 인수를 f에서 수정함.
        void f(int& x); 

        void f(const int* x); // (O) 인수를 f에서 수정하지 않음.  
        void f(const int& x); 
    }
    // 상수 함수
    {
        class T {
        private:
            int m_Val; 
        public:
            void Func() const { // 상수 멤버 함수입니다.
                // m_Val = 10; // (X) 컴파일 오류. 상수 멤버 함수에서 멤버 변수를 수정할 수 없습니다.
            }
        };
    }
}
TEST(TestClassicCpp, Mutable) {

    // ----
    // 변경 가능 지정자(mutable)
    // ----
    class T {
    public:
        // 메모리를 절약하기 위해 GetString() 으로 실제 데이터를 요청할때 문자열을 채울겁니다.
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
