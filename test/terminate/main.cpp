#include <iostream>
#include <exception>

void TerminateHandler() {
    std::cout << "TerminateHandler 호출" << std::endl;
    std::abort(); // 프로그램 종료
}
int main() {
    std::set_terminate(TerminateHandler); // terminate 핸들러를 설정합니다.
    throw 0; // 예외 발생. 내부적으로 매칭되는 catch()가 없어 terminate()를 호출하고, TerminateHandler()가 호출 됩니다. 
    return 0;
}