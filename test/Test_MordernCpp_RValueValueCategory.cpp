#include "gtest/gtest.h" 


TEST(TestCppPattern, RValue) {

    // 왼쪽값(lvalue, left Value)과 우측값(rvalue, right value)
    {
        int a = 0; 
        int* ptr1 = &a; // a는 &로 위치 참조가 되므로 좌측값
        // int* ptr2 = &0; // (X) 컴파일 오류. 상수 0은 & 위치 참조가 안되므로 우측값
        // int b = &(a + 10); // (X) 컴파일 오류. 수식 (a + 10)은 & 위치 참조가 안되므로 우측값
    }
    {
        int a = 0; 
        int& ptr1 = a; // a는 &로 참조되므로 좌측값
        // int& ptr2 = 0; // (X) 컴파일 오류. 상수 0은 &로 참조가 안되므로 우측값
        // int& b = (a + 10); // (X) 컴파일 오류. 수식 (a + 10)은 & 참조가 안되므로 우측값        
    }
}
