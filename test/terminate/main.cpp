#include <iostream>
#include <exception>

void myTerminate () {
    std::cout << "terminate handler 호출"<< std::endl;
    std::abort(); // 프로그램 종료
}
int main() {
    std::set_terminate(myTerminate); // terminate 핸들러를 설정합니다.
    throw 0; // 예외 발생
    return 0;
}