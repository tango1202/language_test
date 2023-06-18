#include "gtest/gtest.h" 

namespace {
   
}

TEST(TestClassicCpp, Array) {
    {
        // 자료형 재정의
        typedef int MyArray[5]; 
        MyArray arr; // int arr[5]; 와 동일 
        arr[0] = 10;// 첫번째 요소에 값 대입
        EXPECT_TRUE(arr[0] == 10);
    }
    {
        int* p = new int[5]; // int[5] 배열 생성
        delete[] p; // 삭제. delete가 아님에 유의. delete p;를 하면 int[0] 만 소멸됨
    }
}