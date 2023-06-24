#include "gtest/gtest.h" 

namespace {
    // ----
    // 테스트용 클래스
    // ----
    class T {
    public:
        T() {} // 기본 생성자
        explicit T(int t) {} // int를 전달받는 생성자. explicit를 주어 암시적 형변환을 막음
        T(const T& other) {} // 복사 생성자

        void operator =(const T& other) {} // 대입 연산자
    };
}

TEST(TestClassicCpp, Initialization) {
    // ----
    // 기본 초기화
    // ----
    {
         T obj; // 기본 생성자가 있어야 함
    }
    // ----
    // 값 초기화
    // ----
    {
        T obj1(); // (△) 비권장. 초기화 아님. T를 리턴하는 obj1 함수 정의
        T obj2(1); // int를 전달받는 생성자 호출

        // int를 전달받는 생성자 호출.
        // (△) 비권장. T(2) 로 개체 생성후 obj의 복사 생성자 호출하는 느낌을 줌
        T obj = T(2); 
    }
    // ----
    // 복사 초기화
    // ----   
    T other;
    {
        // T obj = 1; // (X) 컴파일 오류. explicit 가 아니라면 int를 전달받는 값 생성자
        T obj5 = other; // 타입이 같다면 복사 생성자
        T obj6(other); // 복사 생성자
    }
    // ----
    // 생성 후 대입 : 하지 마라.
    // ----
    {
        T obj; // 기본 생성자
        obj = other; // (△) 비권장. 생성하고 대입하지 말고, 완전하게 생성하세요.
    } 
    // ----
    // 배열 초기화
    // ----   
    {
        // int arr1[] = {}; // (X) 컴파일 오류. 갯수가 지정되지 않음. 오류
        int arr2[] = {0, 1, 2}; // 갯수만큼 초기화
        int arr3[3] = {}; // 3개 모두 0으로 초기화
        int arr4[3] = {0, 1, }; // 갯수가 적거나 같아야 함. 모자라면 0

        EXPECT_TRUE(arr2[2] == 2);
        EXPECT_TRUE(arr3[0] == 0 && arr3[1] == 0 && arr3[2] == 0);
        EXPECT_TRUE(arr4[2] == 0);
    }
    {
        char str1[] = "abc"; // {'a', `b`, 'c', '\0'};
        EXPECT_TRUE(str1[0] == 'a');
        EXPECT_TRUE(str1[1] == 'b');
        EXPECT_TRUE(str1[2] == 'c');
        EXPECT_TRUE(str1[3] == '\0'); // 널문자가 추가됨

        wchar_t str2[] = L"abc"; // {L'a', L`b`, L'c', L'\0'};
        EXPECT_TRUE(str2[0] == L'a');
        EXPECT_TRUE(str2[1] == L'b');
        EXPECT_TRUE(str2[2] == L'c');
        EXPECT_TRUE(str2[3] == L'\0'); // 널문자가 추가됨
    }
    // ----
    // 구조체 초기화
    // ----     
    {
        struct T { int x; int y; }; 
        T t = {10, 20}; // 중괄호로 초기화

        EXPECT_TRUE(t.x == 10 && t.y == 20);
    }
}
