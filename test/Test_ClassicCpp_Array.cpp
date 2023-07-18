#include "gtest/gtest.h" 

TEST(TestClassicCpp, Array) {
    // ---- 
    // 개요
    // ----
    {
        // int arr1[]; // (X) 컴파일 오류. 갯수가 지정되지 않음. 오류
        int arr1[3]; // (△) 비권장. int 형 3개 정의. 초기화 되지 않아 비권장 
        int arr2[] = {0, 1, 2}; // (O) 갯수만큼 초기화
        int arr3[3] = {}; // (O) 3개 모두 0으로 초기화
        int arr4[3] = {1, }; // (O) 갯수가 적거나 같아야 함. 모자라면 0

        EXPECT_TRUE(sizeof(arr1) == sizeof(int) * 3); // 배열 요소의 갯수와 같음
        EXPECT_TRUE(arr2[2] == 2);
        EXPECT_TRUE(arr3[0] == 0 && arr3[1] == 0 && arr3[2] == 0);
        EXPECT_TRUE(arr4[0] == 1 && arr4[1] == 0 && arr4[2] == 0);        
    }
    // ----
    // typedef로 타입 재정의
    // ----
    {
        // 타입 재정의
        typedef int MyArray[5]; 
        MyArray arr; // int arr[5]; 와 동일 
        arr[0] = 10; // 첫번째 요소에 값 대입
        EXPECT_TRUE(arr[0] == 10);
    }
    // ----
    // 배열 동적 생성과 소멸
    // ----
    {
        int* p = new int[5]; // int[5] 배열 생성
        delete[] p; // 소멸. delete가 아님에 유의. delete p;를 하면 배열 요소 갯수만큼 소멸자를 호출하지 않고 한번만 호출합니다.
    }
}