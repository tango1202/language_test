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
         T obj; // (O) 기본 생성자가 호출
         T obj1(); // (△) 비권장. 초기화 아님. T를 리턴하는 obj1 함수 선언임
   }
    // ----
    // 값 초기화
    // ----
    {
         T obj2(1); // (O) int를 전달받는 생성자 호출

        // int를 전달받는 생성자 호출.
        // (△) 비권장. T(2) 로 개체 생성후 obj의 복사 생성자 호출하는 느낌을 줌. T obj3(2); 로 호출하는게 더 좋음.
        T obj3 = T(2); // T obj3(2); 와 동일
    }
    // ----
    // 복사 초기화
    // ----   
    T other;
    {
        // T obj1 = 1; // (X) 컴파일 오류. explicit 가 아니라면 int를 전달받는 값 생성자가 호출됨
        T obj1 = other; // (O) 타입이 같다면 복사 생성자가 호출됨
        T obj2(other); // (O) 명시적으로 복사 생성자 호출됨
    }
    // ----
    // 생성 후 대입 : 하지 마라.
    // ----
    {
        T obj; // 기본 생성자
        obj = other; // (△) 비권장. 생성하고 대입하지 말고, 완전하게 생성하세요. T obj(other); 가 낫습니다.
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
