#include "gtest/gtest.h" 

namespace ExceptionWarranty_1 {
    class A {
    public:
        A() {
            throw "A"; // A 개체를 생성하던 중에 예외가 발생합니다.
        }
    };
    class B {
    public:
        B() {
            std::cout << "Construct B" << std::endl;
        }   
        ~B() {
            std::cout << "Destruct B" << std::endl;           
        } 
    };

    class T {
        A* m_A;
        B* m_B;
    public:
        T(A* a, B* b) : m_A(a), m_B(b) { 
            std::cout << "Construct T" << std::endl;
        } 
        ~T() {
            delete m_A; // 개체를 소멸합니다.
            delete m_B;
        }
        const A* GetA() const {return m_A;}
        const B* GetB() const {return m_B;}
    };
}
namespace ExceptionWarranty_2 {
    class A {
    public:
        A() {
            throw "A"; // A 개체를 생성하던 중에 예외가 발생합니다.
        }
    };
    class B {
    public:
        B() {
            std::cout << "Construct B" << std::endl;
        }   
        ~B() {
            std::cout << "Destruct B" << std::endl;           
        } 
    };

    class T {
#if 201103L <= __cplusplus // C++11~
        std::unique_ptr<A> m_A; // 스마트 포인터
        std::unique_ptr<B> m_B;
#else        
        std::auto_ptr<A> m_A; // 스마트 포인터
        std::auto_ptr<B> m_B;
#endif        
    public:
#if 201103L <= __cplusplus // C++11~
        T(std::unique_ptr<A> a, std::unique_ptr<B> b) : m_A(std::move(a)), m_B(std::move(b)) { // 소유권 이전
 #else   
        T(std::auto_ptr<A> a, std::auto_ptr<B> b) : m_A(a), m_B(b) {  // 소유권 이전
#endif        
            std::cout << "Construct T" << std::endl;
        } 
        ~T() {
        }
        const A* GetA() const {return m_A.get();}
        const B* GetB() const {return m_B.get();}
    };
}
TEST(TestClassicCpp, ExceptionWarranty) {
    {
        using namespace ExceptionWarranty_1;
        try {
            // A 생성에서 예외가 발생하면 T 개체 생성자 호출 자체가 안됩니다.
            // (△) 비권장. B 개체가 이미 생성되었고, A 개체 생성중 예외가 발생했다면, B 개체는 소멸되지 않습니다.
            // 기본 보증 위반입니다.
            // T t(new A, new B); 
            // const A* other = t.GetA();
        }
        catch (...) {
        }        
    }
    {
        using namespace ExceptionWarranty_2;
        try {
            // (O) 인자 전달을 위해 인수 생성시 예외가 발생해도 스마트 포인터여서 예외에 안전합니다.
#if 201103L <= __cplusplus // C++11~
            T t(std::unique_ptr<A>(new A), std::unique_ptr<B>(new B));
#else            
            T t(std::auto_ptr<A>(new A), std::auto_ptr<B>(new B)); 
#endif
            const A* other = t.GetA();
        }
        catch (...) {
        }
    } 
}

