#include "gtest/gtest.h" 

namespace Auto {
    // (X) 컴파일 오류
    // double Func(int a, auto b) {
    //     return a + b;   
    // }
}

namespace Decltype_1 {
    template<typename T, typename U>
    void Func_11(T a, U b, decltype(a + b)* result) { // decltype(a + b) 은 int
        *result = a + b;
    }  
}
namespace Decltype_2 {
    // C++11
    template<typename T, typename U>
    auto Add_11(T a, U b) -> decltype(a + b) { 
        return a + b;
    } 
}
namespace Decltype_3 {
    int Func(int a, int b) {
        return a + b;
    } 
}
namespace Decltype_4 {
#if 201402L <= __cplusplus // C++14~    
    // C++14 리턴 타입 추론
    auto Add1_14(int a, int b) {
        return a + b;
    }
    auto Add2_14(int a, int b) {

        const int result = a + b;
        
        // 함수 템플릿 인수 추론에 의해 const int 대신 int가 사용됩니다.
        return result;
    }

    decltype(auto) Add3_14(int a, int b) {
        const int result = a + b;

        // 개체 엑세스로 평가. result 타입 그대로 평가
        return result; 
    }
    // decltype(auto) Add4_14(int a, int b) {
    //     const int result = a + b; // (X) 예외 발생. Func4의 지역 변수 참조를 전달하기 때문

    //     // 좌측값 표현식의 결과로 평가. T&형태로 평가
    //     return (result); 
    // }
    // (X) 컴파일 오류. 리턴 타입은 동일해야 합니다.
    // auto Add5_14(int a, int b) {
    //     if (a < 10) {
    //         return 10; // int
    //     }
    //     else {
    //         return 10.0F; // float
    //     }
    // }
    // class T {
    // public:
    //     // (X) 컴파일 오류. 가상 함수는 리턴 타입 추론을 할 수 없습니다.
    //     virtual auto Add_14(int a) {return a;}
    // };

    int FuncInt_14() {
        return {10};
    }
    auto FuncAuto_14() {
        // return {10}; // (X) 컴파일 오류. auto의 중괄호 초기화 특수 추론 규칙은 리턴 타입 추론에서 제공하지 않습니다.
    }
#endif    
}
TEST(TestMordern, Auto) {
    {
        auto a_11 = 10; // int
        auto b_11 = 10L; // long
        auto c_11 = 10UL; // unsigned long

        std::vector<int> v;
        auto d_11 = v.begin(); // std::vector<int>::iterator
    }
    // auto 추론 규칙
    {
        int a = 0;
        const int b = 0;

        auto c_11 = a; // int

        {
            // 배열은 포인터로 추론됩니다.
            int arr[] = {1, 2, 3};
            auto a_11 = arr; // int*
        }
        {
            // 최상위 const는 무시됩니다.
            const int constVal = 0;
            auto a_11 = constVal; // int
            a_11 = 10; // const가 아니여서 값을 대입받을 수 있습니다. 
        } 
        {
            // 참조성은 제거됩니다.
            int x = 10;
            int& ref = x;
            auto a_11 = ref; // int. 참조성이 제거됩니다.
            auto& b_11 = ref; // auto&을 이용하여 억지로 참조자로 받을 수 있습니다.
        }
    }
    // auto의 중괄호 초기화 특수 추론 규칙
    {
        // 중괄호 직접 초기화
        auto a_11{1}; //(△) a는 initializer_list<int> 또는 int
        // auto b_11{1, 2}; // (X) 컴파일 오류. auto에서는 단일 개체 대입 필요  

        // 중괄호 복사 초기화
        auto c_11 = {1}; // c는 initializer_list<int>
        auto d_11 = {1, 2}; // d는 initializer_list<int>       
    }
    // 암시적 형변환과 auto
    {
        class MyInt {
            int m_Val;
        public:
            explicit MyInt(int val) : m_Val(Clamp(val, 0, 10)) {}
            operator int() {return m_Val;} // int로 형변환 됩니다.

        private:
            int Clamp(int val, int min, int max) { // 최대/최소값으로 변경합니다.
                if (val < min) {return min;}
                if (max < val) {return max;}

                return val;
            }
        };

        {
            int val = MyInt(11); // (△) 비권장. 암시적으로 int로 변경됩니다. 

            EXPECT_TRUE(val == 10); // 최대 / 최소가 잘 한정되어 있습니다.
            EXPECT_TRUE(val + 1 == 11); // (△) 비권장. 값 한정한게 엉망이 되버렸습니다. 이게 다 암시적 형변환으로 int 복제본을 만들었기 때문이에요. 사실 const int&를 리턴하는게 좋습니다.
            int& ref = val; // (△) 비권장. int&로 변환될 수 있습니다.
        }
        {
            // auto val_11 = MyInt(11); // MyInt입니다.
            // int& ref = val_11; // (X) 컴파일 오류. MyInt는 int&로 변환될 수 없습니다.
        }
        {
            auto val_11 = static_cast<int>(MyInt(11)); // (△) 비권장. 명시적으로 int 입니다.
            int& ref = val_11; // (△) 비권장. int&로 변환될 수 있습니다.
        }
    }
}
TEST(TestMordern, Decltype) {
    {
        int a = 0;
        const int b = 0;

        decltype(a) c_11 = a; // int. a와 동일한 int로 추론됨
        decltype(b) d_11 = a; // const int. auto와는 다르게 b와 동일한 const int로 추론됨
        // d_11 = 10; // (X) 컴파일 오류. const int이므로 값대입 안됨
    }
    {
        class T {
        public:
            double m_Val;
        };
        const T* t;

        decltype(t->m_Val) a_11 = 10; // 멤버 엑세스로 평가됩니다. double
        decltype((t->m_Val)) b_11 = 10; // 괄호를 추가하면 좌측값 표현식으로 처리합니다. t가 const 이므로 const double&
    }
    // auto 와 decltype()의 차이점
    {
        // 배열
        {
            int arr[] = {1, 2, 3};
            auto a_11 = arr; // int*
            decltype(arr) d_11 = {1, 2, 3}; // int d_11[3] 
        }
        // 최상위 const
        {
            const int constInt = 10;
            auto a_11 = constInt; // int
            decltype(constInt) d_11 = constInt; // const int
        }
        // 참조성
        {
            int x = 10;
            int& ref = x;

            auto a_11 = ref; // int
            decltype(ref) d_11 = ref; // int&
        }
    }
    // 함수 인자
    {
        using namespace Decltype_1;
        int result = 0;
        Func_11(10, 20, &result);
        EXPECT_TRUE(result == 30);        
    }

    // 후행 리턴 타입
    {
        using namespace Decltype_2;
        auto result_11 = Add_11(10, 20);
        EXPECT_TRUE(result_11 == 30);  
    }
    // declval()
    {
        class T {
        public:
            int Func(int) {return 1;}
        };

        T t;
        // T::Func(int) 함수의 리턴 타입
        decltype(T().Func(10)) val_11 = t.Func(10); 
    }
    {
        class T {
        public:
            explicit T(int) {} // 기본 생성자가 없습니다.
            int Func_11(int) {return 1;}
        };

        T t(10);
        
        // decltype(T().Func_11(10)) val_11 = t.Func(10); // (X) 컴파일 오류. T에 기본 생성자가 없습니다.
        // decltype(T(10).Func_11(10)) val_11 = t.Func(10); // (O)

        // T::Func(int) 함수의 리턴 타입
        decltype(std::declval<T>().Func_11(10)) val_11 = t.Func_11(10); 
    }
#if 201402L <= __cplusplus // C++14~        
    // C++14 decltype(auto)
    {
        using namespace Decltype_3;
        
        // Func(10, 20) 함수 결과 타입
        decltype(Func(10, 20)) c_11 = Func(10, 20); // C++11
        decltype(auto) d_14 = Func(10, 20); // C++14  
    }
    // C++14 decltype(auto)
    {
        
        // 배열
        {
            int arr[] = {1, 2, 3};
            auto a_11 = arr; // int*
            decltype(arr) d_11 = {1, 2, 3}; // int d_11[3] 
            // decltype(auto) d_14 = {1, 2, 3}; // (X) 컴파일 오류. 중괄호 복사 초기화로 배열 요소의 타입까지는 추론하지 못합니다.
        }
        // 최상위 const
        {
            const int constInt = 10;
            auto a_11 = constInt; // int
            decltype(constInt) d_11 = constInt; // const int
            decltype(auto) d_14 = constInt; // const int

        }
        // 참조성
        {
            int x = 10;
            int& ref = x;

            auto a_11 = ref; // int
            decltype(ref) d_11 = ref; // int&
            decltype(auto) d_14 = ref; // int&
        }

    }
    // C++14 리턴 타입 추론
    {
        using namespace Decltype_4;

        int result1 = Add1_14(10, 20); // int를 리턴
        int result2 = Add2_14(10, 20); // const int를 리턴했지만 함수 템플릿 인수 추론 규칙에 따라 int를 리턴
        const int result3 = Add3_14(10, 20); // const int 리턴. 리턴하는 result 타입과 동일
        // const int& result4 = Add4_14(10, 20); // const int& 리턴. 리턴하는 (result) 표현식과 동일. 
    }

    {

        using namespace Decltype_4;
        
        auto a = FuncInt_14();
         
    }
#endif
#if 201703L <= __cplusplus // C++17~
    // (C++17~) 중괄호 초기화에서 auto 추론의 새로운 규칙
    {
        // C++11
        {
            // 중괄호 직접 초기화
            auto a_11{1}; //(△) a는 initializer_list<int> 또는 int
            // auto b_11{1, 2}; // (X) 컴파일 오류. auto에서는 단일 개체 대입 필요  

            // 중괄호 복사 초기화
            auto c_11 = {1}; // c는 initializer_list<int>
            auto d_11 = {1, 2}; // d는 initializer_list<int>
        }
        // C++17
        {
            // 중괄호 직접 초기화
            auto a_17{1}; // b는 int. 기존에는 initializer_list<int> 일 수 있었음
            // auto b_17{1, 2}; // (X) 컴파일 오류. auto에서는 단일 개체 대입 필요  

            // 중괄호 복사 초기화
            auto c_17 = {1}; // c는 initializer_list<int>
            auto d_17 = {1, 2}; // d는 initializer_list<int>  
        }
    }
#endif  
}
