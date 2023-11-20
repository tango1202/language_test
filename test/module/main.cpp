import MyModule_20; // 가져오기 선언

int main() {
    Func(); // 모듈에서 Export한 함수를 사용합니다.
    // Print(); // (X) 컴파일 오류. export 한 함수가 아닙니다.
    return 0;
}