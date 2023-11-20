module; // 전역 모듈 조각

#include <iostream> // 모듈에서 사용하는 헤더 파일

export module MyModule_20; // 내보내기할 모듈 선언

void Print() {
    std::cout << "MyModule_20 : Print()" << std::endl;
}
export void Func() { // 내보내기 선언
    Print();
}