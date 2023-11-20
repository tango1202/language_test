export module MyModule_20; // 내보내기할 모듈 선언

// 개별 내보내기
export void Func1() {}

// 그룹 내보내기
export {
    void Func2() {}
    void Func3() {}
}

// 네임스페이스 내보내기
export namespace MyLib {
    void Func4() {}
    void Func5() {}
}

// 템플릿 함수 내보내기
export template<typename T>
void Func6(T val) {}

// 클래스 내보내기
export class T {};

