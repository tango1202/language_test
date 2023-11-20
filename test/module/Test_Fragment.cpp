module; // 전역 모듈 조각(Global module fragment)

#include<iostream> // 모듈에서 사용하는 헤더 파일

export module MyModule_20; // 모듈 선언
export void Func(); // 인터페이스만 작성합니다.

//?? GCC 12.3.0 에서 아직 구현 안됨
// module : private; // 구현을 작성합니다.
void Print() {
    std::cout << "MyModule_20 : Print()" << std::endl; 
}
void Func() {
   Print();
} 
