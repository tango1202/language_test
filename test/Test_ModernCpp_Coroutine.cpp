#include "gtest/gtest.h" 

#if 202002L <= __cplusplus // C++20~  
#include <coroutine>

namespace Coroutine_1 {
    // 코루틴 개체
    class MyCoroutine {  // #2
    public:
        struct MyPromise;
        using promise_type = MyPromise; // #2. 명칭이 promise_type인 종속 타입이 있어야 합니다.
    private:
        std::coroutine_handle<promise_type> m_Handler; // #3. 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy(); // #3. 코루틴 개체에서 소멸해야 합니다.  
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        void Resume() const { // #2. 일시 정지된 코루틴을 재개합니다.
            if (!m_Handler.done()) { // 코루틴이 종료되지 않았다면
                m_Handler.resume(); // 재개합니다.
            }
        } 

        // Promise 개체
        struct MyPromise { // #4
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            MyCoroutine get_return_object() { // #4. 코루틴 개체를 리턴합니다.
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} // #5. 코루틴 함수 실행 초기의 일시 정지 상태입니다.
            auto final_suspend() noexcept {return std::suspend_always{};} //#6. 코루틴 함수 종료시 일시 정지 상태입니다.
            void unhandled_exception() {} // 예외 발생시 호출됩니다.
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() {  // MyCoroutine 개체를 사용하는 코루틴 함수 
        std::cout << "CoroutineFunc #1" << std::endl;
        co_await std::suspend_always{}; // #1
        std::cout << "CoroutineFunc #2" << std::endl;
        co_await std::suspend_always{}; // #1
        std::cout << "CoroutineFunc #3" << std::endl;
    }
}
namespace Coroutine_2 {
    // 코루틴 개체
    class MyCoroutine { 
    public:
        struct MyPromise;
        using promise_type = MyPromise;
    private:
        std::coroutine_handle<promise_type> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy();   
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        void Resume() const { 
            if (!m_Handler.done()) { 
                m_Handler.resume(); 
            }
        } 

        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_never{};} // 코루틴 함수 실행 초기의 일시 정지 상태입니다.
            auto final_suspend() noexcept {return std::suspend_always{};} 
            void unhandled_exception() {} 
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() { 
        std::cout << "CoroutineFunc #1" << std::endl; // #1. 최초 실행시 일시 정지 하지 않고 실행됩니다.
        co_await std::suspend_always{}; // #2. 일시 정지 합니다.
        std::cout << "CoroutineFunc #2" << std::endl;
        co_await std::suspend_always{}; // 일시 정지 합니다.
        std::cout << "CoroutineFunc #3" << std::endl;
    }
}
namespace Coroutine_3 {
    // 코루틴 개체
    class MyCoroutine { 
    public:
        struct MyPromise;
        using promise_type = MyPromise;
    private:
        std::coroutine_handle<promise_type> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy();   
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        void Resume() const { 
            if (!m_Handler.done()) { // 예외가 발생되면 코루틴은 종료됩니다.  
                m_Handler.resume(); // 따라서 m_Handler.resume(); 이 호출되지 않도록 합니다.
            }
        } 

        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} 
            auto final_suspend() noexcept {return std::suspend_always{};} 
            void unhandled_exception() {std::cout << "Coroutine : unhandled_exception" << std::endl;} // 예외 발생시 호출됩니다. 
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() { 
        std::cout << "CoroutineFunc #1" << std::endl; 
        throw 1; // 억지로 예외를 발생시킵니다.
        co_await std::suspend_always{}; // 예외가 발생하여 더이상 실행되지 않습니다.
        std::cout << "CoroutineFunc #2" << std::endl; // 예외가 발생하여 더이상 실행되지 않습니다.
        co_await std::suspend_always{}; // 예외가 발생하여 더이상 실행되지 않습니다.
        std::cout << "CoroutineFunc #3" << std::endl; // 예외가 발생하여 더이상 실행되지 않습니다.
    }
}
namespace Coroutine_3_1 {
    // 코루틴 개체
    class MyCoroutine { 
    public:
        struct MyPromise;
        using promise_type = MyPromise;
    private:
        std::coroutine_handle<promise_type> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy();   
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        void Resume() const { 
            if (!m_Handler.done()) { // 예외가 발생되면 코루틴은 종료됩니다.  
                m_Handler.resume(); // 따라서 m_Handler.resume(); 이 호출되지 않도록 합니다.
            }
        } 

        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} 
            auto final_suspend() noexcept {return std::suspend_always{};} 
            void unhandled_exception() {throw;} // 예외를 전파 합니다.
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() { 
        std::cout << "CoroutineFunc #1" << std::endl; 
        throw 1; // 억지로 예외를 발생시킵니다.
        co_await std::suspend_always{}; // 예외가 발생하여 더이상 실행되지 않습니다.
        std::cout << "CoroutineFunc #2" << std::endl; // 예외가 발생하여 더이상 실행되지 않습니다.
        co_await std::suspend_always{}; // 예외가 발생하여 더이상 실행되지 않습니다.
        std::cout << "CoroutineFunc #3" << std::endl; // 예외가 발생하여 더이상 실행되지 않습니다.
    }
}
namespace Coroutine_4 {
    // 코루틴 개체
    class MyCoroutine { 
    public:
        struct MyPromise;
        using promise_type = MyPromise;
    private:
        std::coroutine_handle<promise_type> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy();   
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        int GetVal() const {return m_Handler.promise().m_Val;} // #4. promise 개체의 멤버 변수 값을 리턴합니다.
        void Resume() const { 
            if (!m_Handler.done()) { 
                m_Handler.resume(); 
            }
        } 

        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            int m_Val{0}; // #3. return_value()에서 설정된 값을 저장합니다.
            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} 
            auto final_suspend() noexcept {return std::suspend_always{};} 
            void return_value(int val) {m_Val = val;} // #2. co_return val; 시 호출됩니다.
            // void return_void() {} // co_return; 시 호출
            void unhandled_exception() {} 
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() {  
        std::cout << "CoroutineFunc #1" << std::endl;
        co_await std::suspend_always{};
        std::cout << "CoroutineFunc #2" << std::endl;
        co_await std::suspend_always{};
        std::cout << "CoroutineFunc #3" << std::endl;
        co_return 100; // #1. 코루틴 함수에서 값을 리턴하고 종료합니다.
    }
}

namespace Coroutine_5 {
    // 코루틴 개체
    class MyCoroutine { 
    public:
        struct MyPromise;
        using promise_type = MyPromise;
    private:
        std::coroutine_handle<promise_type> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy();   
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        int GetVal() const {return m_Handler.promise().m_Val;} // #4. promise 개체의 멤버 변수 값을 리턴합니다.
        void Resume() const { 
            if (!m_Handler.done()) { 
                m_Handler.resume(); 
            }
        } 

        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            int m_Val{0}; // #3. return_value()에서 설정된 값을 저장합니다.

            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} 
            auto final_suspend() noexcept {return std::suspend_always{};} 
            auto yield_value(int val) { // #2. co_yield 시 호출됩니다.
                m_Val = val;
                return std::suspend_always{}; // co_yield 후 일시 정지 상태로 만듭니다.
            } 
                        
            void unhandled_exception() {} 
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() {
        int result = 0;
        while (true) {
            co_yield result; // #1. MyPromise.yield_value()를 호출하고 일시 정지 합니다.
            ++result;         
        }
    }
}
namespace Coroutine_6 {
    // 코루틴 개체
    class MyCoroutine {  
    public:
        struct MyPromise;
        using promise_type = MyPromise; // 명칭이 promise_type인 종속 타입이 있어야 합니다.
    private:
        std::coroutine_handle<promise_type> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<promise_type> handler) : m_Handler(handler) {}
        ~MyCoroutine() {
            if (m_Handler) {
                m_Handler.destroy(); 
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        void Resume() const { 
            if (!m_Handler.done()) { 
                m_Handler.resume(); 
            }
        } 

        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} 
            auto final_suspend() noexcept {return std::suspend_always{};} 
            void unhandled_exception() {throw;} 
        };
    };

    // 코루틴 함수
    MyCoroutine CoroutineFunc() {  
        std::cout << "CoroutineFunc #1" << std::endl;
        co_await std::suspend_always{}; 
        std::cout << "CoroutineFunc #2" << std::endl;
    }    
}
namespace Coroutine_7 {

    // 코루틴 개체
    class MyCoroutine {  
    public:
        struct MyPromise;  
        // using promise_type = MyPromise; // #1. promise_type을 선언하지 않았습니다.      
    private:
        std::coroutine_handle<MyPromise> m_Handler; // 코루틴 핸들
    public:
        MyCoroutine(std::coroutine_handle<MyPromise> handler) : m_Handler(handler) {}
        ~MyCoroutine() {     
           if (m_Handler) {
                m_Handler.destroy();  
            }
        }
        MyCoroutine(const MyCoroutine&) = delete;
        MyCoroutine(MyCoroutine&&) = delete;
        MyCoroutine& operator =(const MyCoroutine&) = delete;
        MyCoroutine& operator =(MyCoroutine&&) = delete;

        void Resume() const { 
        if (!m_Handler.done()) { 
                m_Handler.resume(); 
            }
        } 
        
        // Promise 개체
        struct MyPromise { 
            MyPromise() = default;
            ~MyPromise() = default;
            MyPromise(const MyPromise&) = delete;
            MyPromise(MyPromise&&) = delete;
            MyPromise& operator =(const MyPromise&) = delete;
            MyPromise& operator =(MyPromise&&) = delete;

            MyCoroutine get_return_object() { 
                return MyCoroutine{std::coroutine_handle<MyPromise>::from_promise(*this)};
            }
            auto initial_suspend() {return std::suspend_always{};} 
            auto final_suspend() noexcept {return std::suspend_always{};} 
            void unhandled_exception() {throw;} 
        };
    };  
}
namespace std {
    using Coroutine_7::MyCoroutine;
    // #2. MyCoroutine::MyPromise를 promise_type으로 만듭니다.
    template<typename... ARGS>
    struct coroutine_traits<MyCoroutine, ARGS...> { 
        using promise_type = MyCoroutine::MyPromise;
    };
}

namespace Coroutine_7 {
    // 코루틴 함수
    MyCoroutine CoroutineFunc() {  
        std::cout << "coroutine_traits #1" << std::endl;
        co_await std::suspend_always{}; 
        std::cout << "coroutine_traits #2" << std::endl;
    } 
}

#endif

TEST(TestModern, Coroutine) {

#if 202002L <= __cplusplus // C++20~  
    // 개요
    {
        using namespace Coroutine_1;

        // Call #1
        // Call #2
        std::cout << "Call #1" << std::endl;
        MyCoroutine object = CoroutineFunc();
        std::cout << "Call #2" << std::endl;
    }
    {
        using namespace Coroutine_1;

        MyCoroutine obj = CoroutineFunc();
        // Call #1
        // CoroutineFunc #1
        // Call #2
        // CoroutineFunc #2
        // Call #3
        // CoroutineFunc #3
        std::cout << "Call #1" << std::endl;
        obj.Resume(); // CoroutineFunc #1
        
        std::cout << "Call #2" << std::endl;
        obj.Resume(); // CoroutineFunc #2
   
        std::cout << "Call #3" << std::endl;
        obj.Resume(); // CoroutineFunc #3
    }
    {
    using namespace Coroutine_2;   
        MyCoroutine obj = CoroutineFunc(); // CoroutineFunc #1

        std::cout << "Call #1" << std::endl;
        obj.Resume(); // CoroutineFunc #2

        std::cout << "Call #2" << std::endl;
        obj.Resume(); // CoroutineFunc #3

        std::cout << "Call #3" << std::endl;
    }
    // promise 개체의 예외 처리
    {
    using namespace Coroutine_3;   
        MyCoroutine obj = CoroutineFunc(); 

        std::cout << "Call #1" << std::endl;
        obj.Resume(); // CoroutineFunc #1 출력 후 예외를 발생합니다.
        
        std::cout << "Call #2" << std::endl;
        obj.Resume(); // CoroutineFunc #2 -> 실행되지 않습니다.
   
        std::cout << "Call #3" << std::endl;
        obj.Resume(); // CoroutineFunc #3 -> 실행되지 않습니다.
    } 
    {
    using namespace Coroutine_3_1;   
        try {
            MyCoroutine obj = CoroutineFunc(); 

            std::cout << "Call #1" << std::endl;
            obj.Resume(); // CoroutineFunc #1 출력 후 예외를 발생합니다.

            std::cout << "Call #2" << std::endl; // 예외가 발생하여 더이상 실행되지 않습니다.
            obj.Resume(); // 예외가 발생하여 더이상 실행되지 않습니다.

            std::cout << "Call #3" << std::endl; // 예외가 발생하여 더이상 실행되지 않습니다.
            obj.Resume(); // 예외가 발생하여 더이상 실행되지 않습니다.
        }
        catch (const int&) {
            std::cout << "Coroutine : throw 1;" << std::endl;    
        }
    }       
    {
        using namespace Coroutine_4;

        MyCoroutine obj = CoroutineFunc();
        EXPECT_TRUE(obj.GetVal() == 0); // #5
        
        obj.Resume(); // CoroutineFunc #1
        EXPECT_TRUE(obj.GetVal() == 0); // #5
        
        obj.Resume(); // CoroutineFunc #2
        EXPECT_TRUE(obj.GetVal() == 0); // #5

        obj.Resume(); // CoroutineFunc #3
        EXPECT_TRUE(obj.GetVal() == 100); // #5
    }
    // co_yield
    {
        using namespace Coroutine_5;

        std::vector<int> v;
        auto func = CoroutineFunc();
        for (int i = 0; i < 3; ++i) {
            func.Resume();
            v.push_back(func.GetVal()); // #5
        }
        EXPECT_TRUE(v.size() == 3 && v[0] == 0 && v[1] == 1 && v[2] == 2);
    }
    // coroutine_traits
    {
        using namespace Coroutine_6;
        
        MyCoroutine obj = CoroutineFunc();
        obj.Resume(); // 시작하자 마자 대기한 함수 실행
        obj.Resume(); // co_await std::suspend_always{}; 이후 재개 
    }
    {
        using namespace Coroutine_7;
        
        MyCoroutine obj = CoroutineFunc();
        obj.Resume(); // 시작하자 마자 대기한 함수 실행
        obj.Resume(); // co_await std::suspend_always{}; 이후 재개 
    }
#endif    

}