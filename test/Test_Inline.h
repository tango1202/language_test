#ifndef Test_Inline_h 
#define Test_Inline_h

// ----
// 여러 cpp에서 사용하는 인라인 함수 정의
// ----
namespace TestInline { // 이름 충돌을 피하기 위해 네임스페이스에서 정의

    // 헤더파일에 함수를 정의하면, 이를 include한 여러 cpp에서 중복 정의되므로 
    // 컴파일 오류가 발생합니다. 
    // 그래서 보통 헤더파일에서는 함수 선언만 합니다.
    // inline을 이용해 헤더파일에 함수를 정의하면, 동일한 함수 정의를 사용하기 때문에
    // 컴파일 오류가 발생하지 않습니다.
    inline int Plus(int a, int b) {
        return a + b;
    }
}

#endif // Test_Inline_h