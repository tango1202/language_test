#include "gtest/gtest.h" 
#include <typeinfo> 

namespace {
    // ----
    // 연산자 오버로딩(+ 연산자)
    // ----
    class T {
    private:
        int m_Val;
    public:
        explicit T(int val) : m_Val(val) {}
        int GetVal() const {return m_Val;}

        T& operator +=(const T& other) {
            this->m_Val += other.m_Val;
            return *this; // 자기 자신을 리턴합니다.
        }

        // 같은 T타입인 경우
        T operator +(const T& other) {
            return T(this->m_Val + other.m_Val); // 새로운 개체를 리턴합니다.
        }

        // int 형도 지원
        T operator +(int val) {
            return T(this->m_Val + val); // 새로운 개체를 리턴합니다.       
        }
    };
    // ----
    // 연산자 오버로딩(`+` 연산자 전역 정의)
    // ----

    // T t = 10 + other; 지원
    inline T operator +(int left, const T& right) {
        T result(left + right.GetVal());
        return result;
    }

    // ----
    // 열거형 연산자 오버로딩
    // ----
    class Week {
    public:
        enum Val {
            Sunday, Monday, Tuesday, Wednesday, 
            Thursday, Friday, Saturday
        };
    };
    // 전위 증가. 
    Week::Val& operator ++(Week::Val& d) { 
        return d = (Week::Saturday == d) ? Week::Sunday : static_cast<Week::Val>(d + 1);
    }    
}
TEST(TestClassicCpp, Operators) {
    // ----
    // 증감 연산자
    // ----
    {
        // 전위형 - 증감시킨 값을 리턴
        {
            int n = 10;
            int val = ++n;
            EXPECT_TRUE(val == 11); // 증가시킨 후 값
            EXPECT_TRUE(n == 11);
        }
        // 후위형 - 증감시키기 전의 값을 리턴
        {
            int n = 10;
            int val = n++;
            EXPECT_TRUE(val == 10); // (△) 비권장. 의도한 것인지 조금 헷갈립니다. 증가시킨 전 값. 
            EXPECT_TRUE(n == 11);
        }
    }

    // ----
    // 비교 연산 표현
    // ----
    {
        int x = 10;
        int y = 10;
        EXPECT_TRUE(!(x < y || y < x)); // x == y

        x = 10;
        y = 20;   
        EXPECT_TRUE((x < y || y < x)); // x != y

        x = 10;
        y = 20;
        EXPECT_TRUE((x < y)); // x < y

        x = 20;
        y = 10;
        EXPECT_TRUE((y < x)); // x > y      

        x = 10;
        y = 20;
        EXPECT_TRUE(!(y < x)); // x <= y

        x = 10;
        y = 10;
        EXPECT_TRUE(!(y < x)); // x <= y   

        x = 20;
        y = 10;
        EXPECT_TRUE(!(x < y)); // x >= y

        x = 10;
        y = 10;
        EXPECT_TRUE(!(x < y)); // x >= y   
    }
    // ----
    // 함수 호출 연산자
    // ----
    {
        class T {
        public:
            int operator ()(int a, int b) const {return a + b;}
        };

        T t;
        EXPECT_TRUE(t(10, 20) == 30); // operator() 호출
        EXPECT_TRUE(t.operator ()(10, 20) == 30); // t(10, 20) 호출과 동일. operator()를 명시적으로 호출        
    }
    // ----
    // 콤마 연산자
    // ----
    {
        class T {
        public:
            static int f(int val) {return val;}
        };

        int arr[] = {1, 2, 3};
        int i = 0;

        // 1를 증가시키고, v의 i 항목을 f 에 전달합니다.
        // (△) 비권장. 분석하기 복잡합니다.
        EXPECT_TRUE(T::f((++i, arr[i])) == 2); 
    }
    // ----
    // 생성/소멸 연산자
    // ----
    {
        class T {};

        T* p = new T; // 일반 개체는 new로 생성하고 delete로 소멸
        delete p;

        T* arr = new T[10]; // 배열은 new[] 로 생성하고, delete[] 로 소멸
        delete[] arr;
    }
    // ----
    // sizeof 
    // ----
    {
        short i;
        int arr[10];
        class T {
            int m_Val;
        };
        struct S {
            int x;
            int y;
        };
        S s;
        S& ref = s; // ref는 s의 참조자

        EXPECT_TRUE(sizeof(i) == 2); // short는 2byte
        EXPECT_TRUE(sizeof(arr) == sizeof(int) * 10); // 배열은 요소의 전체 크기
        EXPECT_TRUE(sizeof(T) == sizeof(int) * 1); // 클래스와 구조체는 멤버 변수들의 합 
        EXPECT_TRUE(sizeof(S) == sizeof(int) * 2); 
        EXPECT_TRUE(sizeof(s) == sizeof(ref)); // 참조자의 크기는 참조하는 개체의 크기와 동일
    }
    // ----
    // typeid
    // ----
    {
        // 가상 함수 없음
        class Base1 {};
        class Derived1 : public Base1 {};

        // 가상 함수 있음
        class Base2 { 
        public:
            virtual void f() {}
        };
        class Derived2 : public Base2 {};

        // Base1과 개체 b1은 이름이 동일합니다.
        {
            Base1 b1;
            const std::type_info& ti1 = typeid(Base1);
            const std::type_info& ti2 = typeid(b1);
            EXPECT_TRUE(ti1 == ti2);
            EXPECT_TRUE(ti1.name() == ti2.name());
        }
        // 가상 함수가 없는 경우의 참조 - 참조 대상이 Derived1이지만, 정의한 Base1 타입으로 변경됨
        {
            Derived1 d1;
            Base1& b1Ref = d1; // 가상 함수 없음

            // b1Ref = d1으로 bRef는 Base1 타입이 됨
            EXPECT_TRUE(typeid(b1Ref).name() == typeid(Base1).name());
        }
        // 가상 함수가 있는 경우의 참조 - 참조 대상인 Derived2 타입으로 유지됨
        {
            Derived2 d2;
            Base2& b2Ref = d2; // 가상 함수 있음

            // b2Ref = d2로 b2Ref는 다형적 동작하며, 여전히 Derived2 타입임.(원래 개체의 타입 정보)
            EXPECT_TRUE(typeid(b2Ref).name() == typeid(Derived2).name());   
        }
    }
    // ----
    // 조건 연산자
    // ----
    {
        int result = true ? 10 : 20;
        EXPECT_TRUE(result == 10);
    }
    // ----
    // 연산자 오버로딩(복사 대입 연산자)
    // ----
    {
        class T {
        private:
            int m_X;
            int m_Y;
        public:
            T& operator =(const T& other) {
                this->m_X = other.m_X;
                this->m_Y = other.m_Y;
                return *this; // 자기 자신을 리턴합니다.
            }
        }; 
        T t1, t2, t3;
        t1 = t2 = t3; // t2 = t3의 결과 t2의 참조자를 리턴하고, t1에 대입합니다.
    }
    // ----
    // 연산자 오버로딩(+= 연산자)
    // ----
    {
        class T {
        private:
            int m_Val;
        public:
            explicit T(int val) : m_Val(val) {}
            int GetVal() const {return m_Val;}

            // 같은 T타입인 경우
            T& operator +=(const T& other) {
                this->m_Val += other.m_Val;
                return *this; // 자기 자신을 리턴합니다.
            }

            // int 형도 지원
            T& operator +=(int val) {
                this->m_Val += val;
                return *this; // 자기 자신을 리턴합니다.        
            }
        };

        T t1(10), t2(20);
        t1 += t2; // operator +=(const T& other) 호출
        EXPECT_TRUE(t1.GetVal() == 30); 

        t1 += 10;// operator +=(int val) 호출
        EXPECT_TRUE(t1.GetVal() == 40); 
    }  
    // ----
    // 연산자 오버로딩(+ 연산자)
    // ----
    {
        T t1(10), t2(20);
        T t3(0);
        t3 = t1 + t2; // operator +(const T& other) 호출
        EXPECT_TRUE(t1.GetVal() == 10); 
        EXPECT_TRUE(t3.GetVal() == 30); 

        t3 = t1 + 10;// operator +(int val) 호출
        EXPECT_TRUE(t1.GetVal() == 10); 
        EXPECT_TRUE(t3.GetVal() == 20); 
    }  
    // ----
    // + 보다는 += 이 좋은 이유
    // ----
    {
        T t1(10);
        T t2(20);

        // (△) 비권장. 임시 개체가 생성됨
        T t3 = t1 + t2; // t1 + t2 인 임시 개체를 생성. t3의 복사 생성자 호출하여 임시 개체 대입
    }
    {
        T t1(10);
        T t2(20);

        // (O) 권장. 임시 개체가 생성되지 않음
        T t3 = t1; // t3의 복사 생성자를 호출하여 t1값 대입
        t3 += t2; // t3에 t2값 증가
    }
    // ----
    // 연산자 오버로딩(`+` 연산자 비멤버 버전 정의)
    // ----
    {
        T t1(10);
        T t2(0);
        T t3(0);

        t2 = t1 + 10;// t1.operator +(int val) 호출
        t2 = t1.operator +(10); // t2 = t1 + 10과 동일
        
        t3 = 10 + t1;// (X) 컴파일 오류. 10.operator +(T other) 호출합니다. int 형인 10에는 해당 operator 가 없습니다.
        EXPECT_TRUE(t2.GetVal() == 20 && t3.GetVal() == 20);
    }
    // ----
    // 증감 연산자 오버로딩
    // ----
    {
        class T {
        private:
            int m_Val;
        public:
            explicit T(int val) : m_Val(val) {}
            int GetVal() const {return m_Val;}

            // 전위형
            T& operator ++() {
                ++m_Val;
                return *this; // 자기 자신을 리턴합니다.
            }
            // 후위형. 인자 int는 전위형과 구분하기 위한 dummy입니다.
            const T operator ++(int) { // t++++가 안되도록 const T를 리턴합니다.
                T result = *this; // 복제합니다.
                ++m_Val; // this의 값을 증가시킵니다.
                return result; // 증가시키기 전에 복제한 값을 리턴합니다.
            }
        };

        T t(10);
        T t1 = ++t;
        EXPECT_TRUE(t1.GetVal() == 11); // 증가시킨 후 값
        EXPECT_TRUE(t.GetVal() == 11);

        T t2 = t++;
        EXPECT_TRUE(t2.GetVal() == 11); // (△) 비권장. 의도한 것인지 조금 헷갈립니다. 증가시킨 전 값. 
        EXPECT_TRUE(t.GetVal() == 12);
    }
    // ----
    // 비교 연산자 오버로딩
    // ----
    {
        class T {
        private:
            int m_Val;
        public:
            explicit T(int val) : m_Val(val) {}
            int GetVal() const {return m_Val;}

            bool operator <(const T& other) const {
                return this->m_Val < other.m_Val;
            } 

            bool operator ==(const T& other) const {
                return !(this->m_Val < other.m_Val || other.m_Val < this->m_Val);
            }
            bool operator !=(const T& other) const {
                return !(*this == other);
            }
            bool operator >(const T& other) const {
                return other < *this;
            }
            bool operator <=(const T& other) const {
                return !(other < *this);
            }
            bool operator >=(const T& other) const {
                return !(*this < other);
            }
        };
        {
            T t1(10);
            T t2(10);
            EXPECT_TRUE(t1 == t2);
        }
        {
            T t1(10);
            T t2(20);   
            EXPECT_TRUE(t1 != t2);
        }
        {
            T t1(10);
            T t2(20);  
            EXPECT_TRUE(t1 < t2); 
        }
        {
            T t1(20);
            T t2(10);  
            EXPECT_TRUE(t1 > t2);  
        }
        {
            T t1(10);
            T t2(20); 
            EXPECT_TRUE(t1 <= t2);
        }
        {
            T t1(10);
            T t2(10);
            EXPECT_TRUE(t1 <= t2);
        }
        {
            T t1(20);
            T t2(10); 
            EXPECT_TRUE(t1 >= t2);
        }
        {
            T t1(10);
            T t2(10); 
            EXPECT_TRUE(t1 >= t2);  
        } 
    }
    // ----
    // 열거형 연산자 오버로딩
    // ----
    {
        Week::Val val = Week::Saturday;

        EXPECT_TRUE(++val == Week::Sunday); // 토요일 에서 1 증가하면, 제일 처음 값인 일요일로 순회됨
        EXPECT_TRUE(++val == Week::Monday); // 일요일 에서 1 증가하여 월요일
    }
}
