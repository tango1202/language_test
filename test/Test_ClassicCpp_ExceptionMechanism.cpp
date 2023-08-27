#include "gtest/gtest.h" 

namespace {
    // unsigned char ToChar(int val) {
    //     return static_cast<char>(val); // 0~255 의 범위를 벗어나면 오동작합니다.
    // }
    unsigned char ToChar(int val) {
        assert(0 <= val && val < 256); // 인자가 유효한지 검사합니다.

        if (!(0 <= val && val < 256)) {
            throw std::out_of_range("범위 오류"); // 예외를 발생시킵니다.
        }
        return static_cast<char>(val);
    }

    int f(int val) {
        throw std::out_of_range("f() 범위 오류"); // 예외를 발생시킵니다.
        return 0;
    }
    int g(int val) {
        throw std::out_of_range("g() 범위 오류"); // 예외를 발생시킵니다.
        return 0;
    }
    void h() {
        try {
            int a = 10;
            int b = f(a); // f() 함수에서 예외가 발생하여 g()는 호출되지 않습니다. 스택 풀기를 하고 catch문으로 이동합니다.
            int c = g(a);
        }
        catch (std::out_of_range& e) {
            // 아무 작업도 안합니다.
        }
    }
    void myTerminate () {
        std::cerr << "#########################################terminate handler called\n";
        //std::abort();  // forces abnormal termination
    }
}

TEST(TestClassicCpp, ExceptionMechanism) {

    {
        int val = 'A';
        unsigned char ch = ToChar(val + 1); 
        EXPECT_TRUE(ch == 'B');
        
        // (X) 예외 발생. 예외를 처리하는 곳이 없으면 프로그램이 종료됩니다.
        // int val = 256;
        // ch = ToChar(val); 

        // (O) 호출전에 조건을 검사하여 실행할 수 있습니다.
        if (0 <= val && val < 256) {
            ch = ToChar(val); 
        }

        // (O) 그냥 실행하고 예외가 발생하면 이를 탐지하여 뭔가를 처리할 수 있습니다.
        try {
            ch = ToChar(val); 
        }
        catch (std::out_of_range& e) { // 참조자를 사용합니다.
            // 예외가 발생했을때 뭔가 처리하는 코드
        }
    }  
    {
        try {
            std::string("abc").substr(10); // (X) 예외 발생. std::length_error
        }
        catch (const std::exception& e) {
            std::cout << e.what() << '\n';
            // throw e; // (△) 비권장. std::exception 을 복제하여 리턴
            // throw; // (O) std::length_error 를 재전파
        }        
    }  
    {
        h();
    }
    {
        try {
            std::auto_ptr<int> a(new int(10)); // (O) 
            int b = f(*a);
            int c = g(*a);
        }
        catch (std::out_of_range& e) {
            // 아무 작업도 안합니다.
        }        
    }
    // ---
    // terminate
    // ---
    {
        std::set_terminate(myTerminate); // terminate 핸들러를 설정합니다.

        std::terminate(); // 예외 발생
    }
}
