#include "gtest/gtest.h" 

namespace ExceptionMechanism_1 {
    // unsigned char ToChar(int val) {
    //     return static_cast<char>(val); // 0~255 의 범위를 벗어나면 오동작합니다.
    // }
    unsigned char ToChar(int val) {
        assert(0 <= val && val < 256); // 인자가 유효한지 진단합니다.

        if (!(0 <= val && val < 256)) {
            throw std::out_of_range("Fail : val range"); // 예외를 발생시킵니다.
        }
        return static_cast<char>(val);
    }

    int f(int val) {
        throw std::out_of_range("Fail : f()"); // 예외를 발생시킵니다.
        return 0;
    }
    int g(int val) {
        throw std::out_of_range("Fail : g()"); // 예외를 발생시킵니다.
        return 0;
    }
    void h() {
        try {
            int a = 10;
            int b = f(a); // f() 함수에서 예외가 발생하여 g()는 호출되지 않습니다. 스택 풀기를 하고 catch문으로 이동합니다.
            int c = g(a); // f()에서 예외 발생시 호출되지 않습니다.
        }
        catch (std::out_of_range& e) {
            // 예외 처리를 합니다.
        }
    }

    void f() {}
}

namespace ExceptionMechanism_2 {
    unsigned char ToChar(int val) {
        assert(0 <= val && val < 256); // 인자가 유효한지 진단합니다.

        if (!(0 <= val && val < 256)) {
            throw std::out_of_range("Fail : val range"); // 예외를 발생시킵니다.
        }
        return static_cast<char>(val);
    }

    unsigned char f(int val) {
        // (△) 비권장. 매번 진단과 예외 발생 코드를 넣으면 눈을 어지럽힙니다.
        // assert(0 <= val && val < 256); // 인자가 유효한지 진단합니다.
        // if (!(0 <= val && val < 256)) {
        //     throw std::out_of_range("Fail : val range"); // 예외를 발생시킵니다.
        // }
        return ToChar(val);
    }
    unsigned char g(int val) {
        // (△) 비권장. 매번 진단과 예외 발생 코드를 넣으면 눈을 어지럽힙니다.
        // assert(0 <= val && val < 256); // 인자가 유효한지 진단합니다.
        // if (!(0 <= val && val < 256)) {
        //     throw std::out_of_range("Fail : val range"); // 예외를 발생시킵니다.
        // }
        return f(val);
    }
    unsigned char h(int val) {
        // (△) 비권장. 매번 진단과 예외 발생 코드를 넣으면 눈을 어지럽힙니다.
        // assert(0 <= val && val < 256); // 인자가 유효한지 진단합니다.
        // if (!(0 <= val && val < 256)) {
        //     throw std::out_of_range("Fail : val range"); // 예외를 발생시킵니다.
        // }
        return g(val);
    }    
}
namespace ExceptionMechanism_3 {
#if 201103L <= __cplusplus // C++11~    
#else
    void f() throw(int, double) { 
        throw 'e'; // (△) 비권장. 예외 명세에 없는 char 형 예외를 발생시킵니다. 
    }
#endif
}
namespace ExceptionMechanism_4 {
#if 201103L <= __cplusplus // C++11~    
#else    
    void MyUnexpectedHandler() {
        throw; // #3. 예외를 전파합니다 
    }

    void f() throw(int, double, std::bad_exception) { // #1
        throw 'e'; // (△) 비권장. 예외 명세에 없는 char 형 예외를 발생시킵니다. 
    }
#endif
}

TEST(TestLegacyCpp, ExceptionMechanism) {
    // 예외 발생과 탐지(try-catch(), throw)
    {
        using namespace ExceptionMechanism_1;

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
        using namespace ExceptionMechanism_2;
        // 가장 상위 개체에서 try-catch 합니다.
        try {
            int val = 'A';
            unsigned char ch = h(val); 
        }
        catch (std::out_of_range& e) { // 참조자를 사용합니다.
            // 예외가 발생했을때 뭔가 처리하는 코드
        }    
    }

    {
        using namespace ExceptionMechanism_1;
        class MyError {};
        class YourError {};

        try {
            f();
        }
        catch (const MyError&) {} 
        catch (const YourError&) {}
    } 
    {
        using namespace ExceptionMechanism_1;
        class Base : public std::exception {};
        class Derived : public Base {};

        try {
            f();
        }
        catch (const std::exception&) {} // (△) 비권장. std::exception에서 Derived와 Base를 모두 탐지합니다.
        // catch (const Derived&) {} // (X) 컴파일 경고 
        // catch (const Base&) {}       
    }   
    {
        using namespace ExceptionMechanism_1;
        class Base : public std::exception {};
        class Derived : public Base {};
        try {
            f();
        }
        catch (const Derived&) {} // (O) 자식 개체를 먼저 탐지해야 합니다.
        catch (const Base&) {}    
        catch (const std::exception&) {} 
    } 
    
    {
        using namespace ExceptionMechanism_1;

        class Base : public std::exception {};
        class Derived : public Base {};
        try {
            f();
        }
        catch (const Derived&) {} // 자식 개체 타입을 먼저 사용
        catch (const Base&) {}
        catch (const std::exception&) {}
        catch (...) {} // 그외 모든 예외 타입
    }
    // 예외 전파
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
    // 스택 풀기(예외 복원)
    {
        using namespace ExceptionMechanism_1;
        h();
    }
    {
        try {
            using namespace ExceptionMechanism_1;
#if 201103L <= __cplusplus // C++11~        
            std::unique_ptr<int> a(new int(10));    
#else
            std::auto_ptr<int> a(new int(10)); // (O) 
#endif
            int b = f(*a);
            int c = g(*a);
        }
        catch (std::out_of_range& e) {
            // 예외 처리를 합니다.
        }        
    }
    // 동적 예외 사양
    {
        using namespace ExceptionMechanism_3;  
#if 201103L <= __cplusplus // C++11~    
#else        
        try {
            // f(); // (△) 비권장.  int와 double 예외 명세가 있지만, char 예외를 발생시킵니다.
        }
        catch (char) {
            std::cout << "throw : char" << std::endl;
        }       
        catch (int) {
            std::cout << "throw : int" << std::endl;
        }
        catch (double) {
            std::cout << "throw : double" << std::endl;
        } 
        catch (...) {
            std::cout << "throw : ..." << std::endl; // 예외를 탐지하지 못합니다.
        } 
#endif                       
    }
    {
#if 201103L <= __cplusplus // C++11~    
#else        
        using namespace ExceptionMechanism_4;  
        std::unexpected_handler old = std::set_unexpected(MyUnexpectedHandler); // #2. unexpected 핸들러를 설정합니다.
        try {
            f(); // (△) 비권장.  int와 double 예외 명세가 있지만, char 예외를 발생시킵니다.
        }
        catch (char) {
            std::cout << "throw : char" << std::endl;
        }       
        catch (int) {
            std::cout << "throw : int" << std::endl;
        }
        catch (double) {
            std::cout << "throw : double" << std::endl;
        } 
        catch (std::bad_exception&) {
            std::cout << "throw : bad_exception" << std::endl; // #4. bad_exception으로 예외를 탐지합니다.
        } 
        catch (...) {
            std::cout << "throw : ..." << std::endl; 
        } 
        
         std::set_unexpected(old); // #2
#endif         
    }
}

