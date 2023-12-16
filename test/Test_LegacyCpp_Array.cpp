#include "gtest/gtest.h" 

TEST(TestLegacyCpp, Array) {
    // ----
    // 배열 초기화
    // ----   
    {
        int arr1[3]; // (△) 비권장. int 형 3개 정의. 초기화 되지 않아 비권장 
        // int arr1[]; // (X) 컴파일 오류. 갯수가 지정되지 않음. 오류
        // int arr1[] = {}; // (X) 컴파일 오류. 갯수가 지정되지 않았고, 중괄호 집합초기화도 비었음. 오류
        int arr2[] = {0, 1, 2}; // (O) 중괄호 집합 갯수만큼 초기화
        int arr3[3] = {}; // (O) 3개 모두 0으로 초기화
        int arr4[3] = {0, 1, }; // (O) 갯수가 적거나 같아야 함. 모자라면 0

        EXPECT_TRUE(arr2[2] == 2);
        EXPECT_TRUE(arr3[0] == 0 && arr3[1] == 0 && arr3[2] == 0);
        EXPECT_TRUE(arr4[2] == 0);
    }
    {
        char str1[] = "abc"; // (O) {'a', `b`, 'c', '\0'};
        EXPECT_TRUE(str1[0] == 'a');
        EXPECT_TRUE(str1[1] == 'b');
        EXPECT_TRUE(str1[2] == 'c');
        EXPECT_TRUE(str1[3] == '\0'); // 널문자가 추가됨
        EXPECT_TRUE(sizeof(str1) / sizeof(char) == 4); // 배열 갯수는 널문자를 포함하여 4
        EXPECT_TRUE(strlen(str1) == 3); // 문자열의 길이는 3

        wchar_t str2[] = L"abc"; // (O) {L'a', L`b`, L'c', L'\0'};
        EXPECT_TRUE(str2[0] == L'a');
        EXPECT_TRUE(str2[1] == L'b');
        EXPECT_TRUE(str2[2] == L'c');
        EXPECT_TRUE(str2[3] == L'\0'); // 널문자가 추가됨
        EXPECT_TRUE(sizeof(str2) / sizeof(wchar_t) == 4); // 배열 갯수는 널문자를 포함하여 4
        EXPECT_TRUE(wcslen(str2) == 3); // 문자열의 길이는 3
    }
    // 배열 크기
    {
        int arr[3];
        EXPECT_TRUE(sizeof(arr) / sizeof(int) == 3); // 배열의 전체 크기 / 배열 요소 크기
    }
    // ----
    // 배열 대입
    // ----
    {
        int arr1[] = {1, 2, 3};
        
        // int arr2[] = arr1; // (X) 컴파일 오류. 배열끼리는 대입되지 않습니다.
        int arr2[3];
        for (int i = 0; i < sizeof(arr1) / sizeof(int); ++i) { // 배열의 각 요소를 복사합니다.
            arr2[i] = arr1[i];
        } 
        EXPECT_TRUE(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3);

        int* ptr = arr1; // 포인터형 변수가 배열을 가리킵니다.

        ptr[1] = 20;
        EXPECT_TRUE(arr1[1] == 20);
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