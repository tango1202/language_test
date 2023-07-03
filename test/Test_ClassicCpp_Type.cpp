#include "gtest/gtest.h" 
#include <cmath>

namespace {
    #define BOOL int
    #define TRUE 1
    #define FALSE 0

    // ----
    // 함수 포인터 타입 재정의
    // ----
    typedef int (*Func)(int, int); // 함수 포인터 typedef

    int f(int a, int b) {return a + b;} // 함수 정의

    // ----
    // template의 타입 재정의
    // ----
    template<class T> 
    struct ClassT { 
        typedef T Type;
        typedef const T ConstType; 
    }; 

    // double 비교
    bool Equals(double a, double b) {
        double diff = fabs(a - b); // 두수의 차

        // (△) 비권장. 두수의 차가 오차 범위보다 작으면 같은 수
        return (diff < std::numeric_limits<double>::epsilon()) ? true : false;
    }
}

TEST(TestClassicCpp, Type) {
    EXPECT_TRUE(sizeof(wchar_t) == 2);  
    EXPECT_TRUE(sizeof(long double) == 16);  
   
    EXPECT_TRUE(sizeof(BOOL) == 4);

    BOOL b = TRUE;
    EXPECT_TRUE(!b == FALSE); // !b == false이고 false를 int로 캐스팅하면 FALSE(0)

    b = FALSE;
    EXPECT_TRUE(!b == TRUE); // !b == true이고 true를 int로 캐스팅하면 TRUE(1)
    
    // b = FALSE;
    // EXPECT_TRUE(~b == FALSE); // (X) ~b 는 0xfffffffe 입니다.
}
TEST(TestClassicCpp, TypeDef) {

    // 배열
    typedef int MyArray[5]; 
    MyArray arr; // int arr[5]; 와 동일 
    arr[0] = 10; // 첫번째 요소에 값 대입
    EXPECT_TRUE(arr[0] == 10);

    // 구조체
    typedef struct {int a; int b;} MyData, *pMyData;
    MyData myData1;
    MyData* myData2;
    pMyData myData3; // MyData* 와 같음
    myData1.a = 10; // a에 값 대입
    EXPECT_TRUE(myData1.a == 10);
   
    // 함수 포인터
    Func func = f; // 함수 포인터 저장

    // template의 타입 재정의
    ClassT<int>::ConstType constVal = 20;  
}

TEST(TestClassicCpp, TypeLimit) {
    EXPECT_TRUE(std::numeric_limits<int>::max() == 2147483647);
    EXPECT_TRUE(std::numeric_limits<int>::min() == -2147483648);
}

TEST(TestClassicCpp, SizeOf) {
    class MyClass {
        int m_X;
        int m_Y;
    };

    MyClass myClass;
    MyClass& myClassRef = myClass;

    EXPECT_TRUE(sizeof(myClass) == 8);
    EXPECT_TRUE(sizeof(myClass) == sizeof(myClassRef)); // sizeof() 시 참조하는 개체와 참조자는 크기가 같습니다.
}

TEST(TestClassicCpp, Float) {
    double a = 10.0;
    double b = a;
    b += 1.0;
    b -= 1.0;
    EXPECT_TRUE(Equals(a, b)); 
}
