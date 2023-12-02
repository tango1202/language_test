#include "gtest/gtest.h" 
#include <typeinfo> 


TEST(TestClassicCpp, MemberFunction) {
    // ----
    // 멤버 함수
    // ----
    {
        class Date {
            int m_Year;
            int m_Month;
            int m_Day;
        public: 
            Date(int year, int month, int day) :
                m_Year(year),
                m_Month(month), 
                m_Day(day) {}

            // Getter/Setter
            int GetYear() const {return m_Year;} // 상수 멤버 함수
            int GetMonth() const {return m_Month;}
            int GetDay() const {return m_Day;}

            void SetYear(int val) {m_Year = val;} // 멤버 함수
            void SetMonth(int val) {m_Month = val;}
            void SetDay(int val) {m_Day = val;}

            // 내부적으로 전체 개월수를 계산하기 위해,
            // 데이터와 처리하는 함수를 응집하였습니다. 
            int CalcTotalMonth() const { // 상수 멤버 함수
                return m_Year * 12 + m_Month; 
            }
        };
        Date date(20, 2, 10); // 20년 2월 10일
        EXPECT_TRUE(date.CalcTotalMonth() == 20 * 12 + 2); 
    }
    // ----
    // 상수 멤버 함수
    // ----    
    {
        class T {
            int m_Val;
        public:
            // void Func() const {m_Val = 10;} // (X) 컴파일 오류. const 함수는 멤버 변수를 수정할 수 없습니다.
        };        
    }
    {
        class T {
            int m_Val;
        public:
            // int* Func() const {return &m_Val;} // (X) 컴파일 오류. int* 리턴. const 함수는 const int*를 리턴해야 합니다.
        };
    }
    {
        class T {
            int m_Val;
        public:
            // void Func() const { NonConstFunc();} // (X) 컴파일 오류. const 함수는 비 상수 멤버 함수 호출할 수 없습니다.
            void NonConstFunc() {}
        };        
    }
    // ----
    // 비 상수 멤버 함수의 비 상수성 전파
    // ----
    {
        class T {
            int m_Val;
        public:
            int GetVal() {return m_Val;} // (△) 비권장. 상수 멤버 함수인데 비 상수로 정의했습니다.
        };

        class U {
            T m_T;
        public:
            // m_T.GetVal()은 비 상수 멤버 함수여서 상수 멤버 함수인 GetInnerVal()에서 호출할 수 없습니다.
            // 어쩔 수 없이 GetInnerVal()을 비 상수로 만들어야 컴파일 할 수 있습니다.
            // int GetInnerVal() const {return m_T.GetVal();} // (X) 컴파일 오류.
        };
    }
    // ----
    // 정적 멤버 함수
    // ----
    {
        class T {
        public:
            static int f() {return 10;} // 정적 멤버 함수
        };

        EXPECT_TRUE(T::f() == 10); // (O) T의 정적 멤버 함수 호출
        T obj;
        EXPECT_TRUE(obj.f() == 10); // (△) 비권장. T의 정적 멤버 함수 호출. 되기는 합니다만 일반 멤버 함수 호출과 구분이 안되어 가독성이 떨어집니다.
    }   
    // ----
    // 가상 함수
    // ----
    {
        class Base {
        public:
            int f() {return 10;}
            virtual int v() {return 10;} // 가상 함수
        };

        class Derived : public Base {
        public:
            int f() {return 20;} // (△) 비권장. Base의 동일한 이름의 비 가상 함수를 가림
            virtual int v() {return 20;} // (O) Base의 가상 함수 재구현(오버라이딩)  굳이 virtual을 붙일 필요는 없습니다.
        };

        Derived d;
        Base* b = &d;

        EXPECT_TRUE(b->f() == 10); // (△) 비권장. Base 개체를 이용하면 Base::f()가 호출됨 
        EXPECT_TRUE(d.f() == 20); // (△) 비권장. Derived 개체를 이용하면 Derived::f()가 호출됨        
        EXPECT_TRUE(static_cast<Base&>(d).f() == 10); // (△) 비권장. 가려진 Base::f() 함수를 호출

        EXPECT_TRUE(b->v() == 20); // (O) 가상 함수여서 Derived::v() 가 호출됨
        EXPECT_TRUE(d.v() == 20); // (O) 가상 함수여서 Derived::v() 가 호출됨
    }
    // 리턴값 변경
    {
        class Base {
        public:
            virtual Base* v() {return this;} // 가상 함수
        };

        class Derived : public Base {
        public:
            virtual Derived* v() {return this;} // (O) Derived 는 Base와 상속 관계여서 가능
            // virtual int* v() {return NULL;} // (X) 컴파일 오류. 밑도 끝도 없는 타입은 안됨
        };
    }
    // ----
    // 순가상 함수
    // ----
    {
        class IEatable {
        public:
            virtual void Eat() = 0; // 순가상 함수
        };

        class Dog : public IEatable {
        public:        
            virtual void Eat() {} // 순가상 함수는 자식 개체에서 실제 구현을 해야 합니다.
        };
        // IEatable eatable; // (X) 컴파일 오류. 순가상 함수가 있기 때문에 인스턴스화 할 수 없습니다.
        Dog dog; // (O)
    }
    // ----
    // Getter
    // ----
    {
        class T {};
        class U {
            int m_Val1;
            T m_Val2;
        public:
            int GetVal1() const {return m_Val1;} // (O) 멤버 변수의 값을 리턴
            int& GetVal1() {return m_Val1;} // (O) 멤버 변수의 값을 수정하는 참조자를 리턴    

            const T& GetVal2() const {return m_Val2;} // (O) 멤버 변수의 참조자 리턴
            T& GetVal2() {return m_Val2;} // (O) 멤버 변수의 값을 수정하는 참조자를 리턴  

            // const int GetVal1() const {return m_Val1;} // (△) 비권장. 리턴값이 기본 타입이라면 어짜피 리턴값이 복제되므로, `const` 부적절
            // int GetVal1() {return m_Val1;} // (△) 비권장. 멤버 변수를 수정하지 않으므로 상수 함수가 적절
            // const int GetVal1() {return m_Val1;} // (△) 비권장. 멤버 변수를 수정하지 않으므로 상수 함수가 적절

            // (△) 비권장. 포인터 보다는 참조자가 적절
            // const int* GetVal1() const {return &m_Val1;} // (△) 비권장. 포인터 보다는 참조자가 적절
            // int* GetVal1() {return &m_Val1;} // (△) 비권장. 포인터 보다는 참조자가 적절
            // int* GetVal1() const {return &m_Val1;} // (X) 컴파일 오류. 상수성 계약 위반
            // const int* GetVal1() {return &m_Val1;} // (△) 비권장. 상수 함수가 적절 

            // const int& GetVal1() const {return m_Val1;} // (△) 비권장. int는 기본 자료형이어서 참조 보다는 값 전달이 적절
            // int& GetVal1() const {return m_Val1;} // (X) 컴파일 오류. 상수성 계약 위반
            // const int& GetVal1() {return m_Val1;} // (△) 비권장. 상수 함수가 적절

            // (△) 비권장. 복사 부하가 참조 부하보다 큰 개체여서 값 복사 보다는 참조가 적절
            // const T GetVal2() const {return m_Val2;} // (△) 비권장. 값 복사 보다는 참조가 적절. 어짜피 리턴값이 복제되므로, `const` 부적절
            // T GetVal2() const {return m_Val2;} // (△) 비권장. 값 복사 보다는 참조가 적절
            // T GetVal2() {return m_Val2;} // (△) 비권장. 값 복사 보다는 참조가 적절. 멤버 변수를 수정하지 않으므로 상수 함수가 적절
            // const T GetVal2() {return m_Val2;} // (△) 비권장. 값 복사 보다는 참조가 적절. 어짜피 리턴값이 복제되므로, `const` 부적절, 멤버 변수를 수정하지 않으므로 상수 함수가 적절

            // (△) 비권장. 포인터 보다는 참조자가 적절
            // const T* GetVal2() const {return &m_Val2;} // (△) 비권장. 포인터 보다는 참조자가 적절
            // T* GetVal2() {return &m_Val2;} // (△) 비권장. 포인터 보다는 참조자가 적절
            // T* GetVal2() const {return &m_Val2;} // (X) 컴파일 오류. 상수성 계약 위반
            // const T* GetVal2() {return &m_Val2;} // (△) 비권장. 상수 함수가 적절

            // T& GetVal2() const {return m_Val2;} // (X) 컴파일 오류. 상수성 계약 위반
            // const T& GetVal2() {return m_Val2;} // (△) 비권장. 상수 함수가 적절
        };      
    }
    // ----
    // Setter
    // ----
    {
        class T {};
        class U {
            int m_Val1;
            T m_Val2;
        public:
            void SetVal1(int val) {m_Val1 = val;} // (O) 멤버 변수에 값 대입
            void SetVal2(const T& val) {m_Val2 = val;} // (O) 참조자를 통해 전달받은 인자를 현 멤버 변수에 복사

            // void SetVal1(const int val) {m_Val1 = val1;} // (△) 비권장. 인자에 복사되므로 int와 const int는 동일 취급됨
            // void SetVal1(int* val) {m_Val = *val;} // (△) 비권장. 인자는 상수 타입이어야 함
            // void SetVal1(const int* val) {m_Val = *val;} // (△) 비권장. 포인터보다는 참조자가 좋음
            // void SetVal1(int& val) {m_Val = *val;} // (△) 비권장. 인자는 상수 타입이어야 함
            // void SetVal1(const int& val) {m_Val = *val;} // (△) 비권장. 기본 자료형의 경우 값 복사가 좋음

            // void SetVal2(T val) {m_Val2 = val;} // (△) 비권장. 값 복사 보다는 참조가 적절
            // void SetVal2(const T val) {m_Val2 = val;} // (△) 비권장. 인자에 복사되므로 T와 const T는 동일 취급됨

            // void SetVal2(T* val) {m_Val2 = *val;} // (△) 비권장. 인자는 상수 타입이어야 함
            // void SetVal2(const T* val) {m_Val2 = *val;} // (△) 비권장. 포인터보다는 참조자가 좋음
            // void SetVal2(T& val) {m_Val2 = val;} // (△) 비권장. 인자는 상수 타입이어야 함
        };
    }
}
