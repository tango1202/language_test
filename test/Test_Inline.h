#ifndef Test_Inline_h 
#define Test_Inline_h

namespace TestInline { // 이름 충돌을 피하기 위해 네임스페이스에서 정의

    // 헤더파일에 함수를 정의하면, 헤더파일을 include한 여러 cpp에서 중복 정의되어 컴파일 오류가 납니다.
    // inline은 여러 컴파일 단위에서 동일한 함수 정의를 사용할 수 있게 합니다.
    inline int Plus(int a, int b) {
        return a + b;
    }
}

#endif // Test_Inline_h