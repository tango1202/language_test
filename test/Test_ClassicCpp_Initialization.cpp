#include "gtest/gtest.h" 

namespace {
    // ----
    // 테스트용 클래스
    // ----
    class T {
    public:
        T() {} // 기본 생성자
        explicit T(int x) {} // int를 전달받는 생성자. explicit를 주어 암시적 형변환을 막음
        T(int x, int y) {} // 값 2개를 전달받는 생성자       
        T(const T& other) {} // 복사 생성자

        void operator =(const T& other) {} // 복사 대입 연산자
    };
}

TEST(TestClassicCpp, Initialization) {
    // ----
    // 기본 초기화
    // ----
    {
        T obj; // (O) 기본 생성자가 호출
        // T obj(); // (△) 비권장. 초기화 아님. T를 리턴하는 obj 함수 선언임
   }
    // ----
    // 값 초기화
    // ----
    {
        T obj(1); // (O) int를 전달받는 생성자 호출
    }
    {
        T obj(1, 2); // (O) 값 2개를 전달받는 생성자 호출    
    }
    {
        // T obj = 1; // (△) 비권장. T t(int x); 를 호출을 위한 것인지, 정수 1을 T에 잘못 대입한 것인지 헷갈립니다.
    }
    {
        T obj = T(1); // (△) 비권장. T(1)로 생성한 개체를 T obj 의 복사 생성자를 호출하여 생성합니다.    
    }
    {
        T obj(T(1)); // (△) 비권장. T(1)로 생성한 개체를 T obj 의 복사 생성자를 호출하여 생성합니다.
    }
    // ----
    // 복사 초기화
    // ----   
    T other;
    {
        // T obj1 = 1; // (△) 비권장. explicit 가 아니라면 int를 전달받는 값 생성자가 호출됨
        T obj1 = other; // (O) 타입이 같다면 복사 생성자가 호출됨
        T obj2(other); // (O) 명시적으로 복사 생성자 호출됨
    }
    // ----
    // 초기화 파싱 오류
    // ----
    {
        // T f(); // T 타입을 리턴하는 함수 f()를 선언합니다.
        // T obj(); // (△) 비권장. T 타입의 obj 개체를 기본 생성자로 생성하고 싶지만, 사실은 T 타입을 리턴하는 함수 obj()를 선언합니다.
        // T obj(T()); // (△) 비권장. T 타입의 기본 생성자로 생성한 것을 T obj에 복사 생성하고 싶지만, T 타입을 리턴하고, T(*)()함수 포인터를 인자로 전달받는 함수 obj를 선언합니다. 
        T obj;
    }
    {
        T obj = T();
    }
    // ----
    // 생성자 호출 및 함수 인수 전달 최적화
    // ----
        {
        class T {
        public:
            T() {} // 기본 생성자
            explicit T(int x) {} // int를 전달받는 생성자. explicit를 주어 암시적 형변환을 막음
            T(int x, int y) {} // 값 2개를 전달받는 생성자       
            T(const T& other) {} // 복사 생성자

            void operator =(const T& other) = delete; // 복사 대입 연산자
        };
       
        T obj1 = T();
        //T obj3 = T(2);

    }




    {
        // int를 전달받는 생성자 호출.
        // (△) 비권장. T(2) 로 개체 생성후 obj3의 복사 생성자 호출. 컴파일러 최적화로 복사 생성자는 호출되지 않을 수 있음. 컴파일러 최적화가 안될 수도 있으니 T obj3(2); 로 호출하는게 더 좋음.
        T obj3 = T(2); // T obj3(T(2)); 와 동일
    }

    // ----
    // 생성 후 대입 : 하지 마라.
    // ----
    {
        T obj; // 기본 생성자
        obj = other; // (△) 비권장. 생성하고 대입하지 말고, 완전하게 생성하세요. T obj(other); 가 낫습니다.
    } 
    // ----
    // 배열 초기화
    // ----
    {
        int arr1[3]; // (△) 비권장. int 형 3개 정의. 초기화 되지 않아 비권장 
        // int arr1[]; // (X) 컴파일 오류. 갯수가 지정되지 않음. 오류
        // int arr1[] = {}; // (X) 컴파일 오류. 갯수가 지정되지 않음. 오류
        int arr2[] = {0, 1, 2}; // (O) 갯수만큼 초기화
        int arr3[3] = {}; // (O) 3개 모두 0으로 초기화
        int arr4[3] = {0, 1, }; // (O) 갯수가 적거나 같아야 함. 모자라면 0

        EXPECT_TRUE(arr2[2] == 2);
        EXPECT_TRUE(arr3[0] == 0 && arr3[1] == 0 && arr3[2] == 0);
        EXPECT_TRUE(arr4[2] == 0);

        char str1[] = "abc"; // (O) {'a', `b`, 'c', '\0'};
        EXPECT_TRUE(str1[0] == 'a');
        EXPECT_TRUE(str1[1] == 'b');
        EXPECT_TRUE(str1[2] == 'c');
        EXPECT_TRUE(str1[3] == '\0'); // 널문자가 추가됨

        wchar_t str2[] = L"abc"; // (O) {L'a', L`b`, L'c', L'\0'};
        EXPECT_TRUE(str2[0] == L'a');
        EXPECT_TRUE(str2[1] == L'b');
        EXPECT_TRUE(str2[2] == L'c');
        EXPECT_TRUE(str2[3] == L'\0'); // 널문자가 추가됨
    }
    // ----
    // 구조체 초기화
    // ----     
    {
        struct T {int x; int y;}; 
        T t = {10, 20}; // (O) 중괄호로 초기화

        EXPECT_TRUE(t.x == 10 && t.y == 20);
    }
}
