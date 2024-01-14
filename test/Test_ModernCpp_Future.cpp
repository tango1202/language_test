#include "gtest/gtest.h" 

#include <thread>
#include <future>

namespace Future_1 {
    void Async(std::promise<int>& promise) {
        promise.set_value(1); // 데이터를 설정합니다.

    }

    int Sync() {

        std::promise<int> promise;
        std::future<int> data{promise.get_future()}; // 미래에 data를 주겠다고 약속합니다.

        std::thread worker{&Async, std::ref(promise)};

        data.wait(); // 데이터를 줄때까지 기다립니다.
        worker.join(); 

        return data.get(); // get() 하면 future 개체에서 관리하던 값은 사라집니다.
    } 
}

namespace Future_2 {
    int Async(int val) {
        return val + 1; // promise에 설정될 데이터를 리턴합니다.
    }

    int Sync() {

        std::packaged_task<int(int)> task(&Async);

        std::future<int> data{task.get_future()}; // 미래에 data를 주겠다고 약속합니다.

        std::thread worker{std::move(task), 10}; // thread에 대입시 task는 복사가 안되므로, 이동 생성합니다. Async 함수의 인자를 전달합니다.

        worker.join(); 

        return data.get(); // get() 하면 future 개체에서 관리하던 값은 사라집니다.
    } 
}
namespace Future_3 {
    int Async(int val) {
        // return val + 1; // promise에 설정될 데이터를 리턴합니다.
        throw "MyError"; // 예외 발생
    }

    int Sync() {

        std::packaged_task<int(int)> task(&Async);

        std::future<int> data{task.get_future()}; // 미래에 data를 주겠다고 약속합니다.

        std::thread worker{std::move(task), 10}; // thread에 대입시 task는 복사가 안되므로, 이동 생성합니다. Async 함수의 인자를 전달합니다.

        worker.join(); 

        return data.get(); // get() 하면 future 개체에서 관리하던 값은 사라집니다.
    } 
}
namespace Future_4 {
    int Async(int val) {
        return val + 1; // promise에 설정될 데이터를 리턴합니다.
    }

    int Sync() {

        std::future<int> data{std::async(std::launch::async, &Async, 10)}; // 미래에 data를 주겠다고 약속합니다. 내부적으로 쓰레드를 가동하고 join() 합니다.

        return data.get(); // get() 하면 future 개체에서 관리하던 값은 사라집니다.
    } 
}


TEST(TestModern, Future) {
    {
        class A {
            int m_Data{0};
            std::condition_variable m_CV;
            bool m_IsAsyncCompleted{false}; // true면 Sync가 wait를 멈춥니다.
        public:
            void Async(std::mutex& mutex) {

                { // lock
                    std::unique_lock<std::mutex> lock{mutex};
                    m_Data = 1; // 데이터를 설정합니다.
                    m_IsAsyncCompleted = true; 
                } // unlock
                m_CV.notify_all(); 
            }

            void Sync() {
                std::mutex mutex; 

                std::thread worker{std::mem_fn(&A::Async), std::ref(*this), std::ref(mutex)};

                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsAsyncCompleted;}); // Async가 끝날때까지 기다립니다.
                lock.unlock();

                worker.join(); 

                EXPECT_TRUE(m_Data == 1);
            }           
        };

        A a{};
        a.Sync();
    }

    // future, promise
    {
        using namespace Future_1;
        EXPECT_TRUE(Sync() == 1); 
        
    } 
    // packaged_task
    {
        using namespace Future_2;
        EXPECT_TRUE(Sync() == 11); 
    } 
    {
        using namespace Future_3;
        try {
            EXPECT_TRUE(Sync() == 11); 
        }
        catch(const char* err) {
            std::cout << "Task Exception : " << err << std::endl;
            EXPECT_TRUE(err == "MyError");
        }
    } 
    // async
    {
        using namespace Future_4;
        EXPECT_TRUE(Sync() == 11); 
    }        

}

#if 202002L <= __cplusplus // C++20~
namespace AtomicFlag_1 {

    std::atomic_flag g_IsCompleted{}; // 기본 생성자는 false로 세팅됩니다.

    void Async(int& data) {
        data = 1; // 데이터를 설정합니다.

        g_IsCompleted.test_and_set(); // true로 설정합니다.
        g_IsCompleted.notify_all(); // 값이 수정되었음을 통지합니다.
    }

    int Sync() {

        int data{0};
        std::thread worker{&Async, std::ref(data)};

        g_IsCompleted.wait(false); // 값이 false 이면 대기합니다.
        worker.join(); 

        return data; 
    }     
}
TEST(TestModern, AtomicFlag) {
        // atomic_flag를 이용한 쓰레드 동기화
    {
        using namespace AtomicFlag_1;

        EXPECT_TRUE(Sync() == 1);

    }
}
#endif
#if 202002L <= __cplusplus // C++20~
namespace Semaphore_1 {
    std::binary_semaphore g_IsCompleted{0}; // 0입니다. acquire()에서 대기합니다.

    void Async(int& data) {
        data = 1; // 데이터를 설정합니다.

        g_IsCompleted.release(); // 1로 증가시킵니다.
    }

    int Sync() {

        int data{0};
        std::thread worker{&Async, std::ref(data)};

        g_IsCompleted.acquire(); // 카운트가 0보다 커질때까지 대기합니다.
        worker.join(); 

        return data; 
    }     
}
TEST(TestModern, Semaphore) {

    using namespace Semaphore_1;

    EXPECT_TRUE(Sync() == 1);

}
#endif

#if 202002L <= __cplusplus // C++20~
#include <latch>
#include <format>

namespace Latch_1 {
    std::latch g_IsAsyncComplated{5}; // 최대 5개가 동시에 진행됩니다.
    std::mutex messageMutex;   

    // std::cout 시 쓰레드 경쟁에 출력이 뒤섞이지 않도록 뮤텍스를 사용합니다.
    void SyncMessage(int i, const char* str) {
        std::lock_guard<std::mutex> lock(messageMutex); // 유효 범위를 벗어나면 unlock을 호출합니다.

        std::cout << std::format("Work {} : {}", i, str) << std::endl;
    }
    void Async(int i) {
        SyncMessage(i, "First");

        g_IsAsyncComplated.arrive_and_wait(); // #1. 숫자를 감소시키고 0이 될때까지 대기합니다.

        SyncMessage(i, "Last"); // #2
    }
}

TEST(TestModern, Latch) {
    using namespace Latch_1;

    std::vector<std::thread> v;

    for (int i{0}; i < 5; ++i) {
        v.emplace_back(&Async, i);
    }

    for (auto& work : v) {
        work.join();
    }
}

#include <barrier>

namespace Barrier_1 {
    std::barrier g_IsAsyncComplated{5}; // 최대 5개가 동시에 진행됩니다.
    std::mutex messageMutex;   

    // std::cout 시 쓰레드 경쟁에 출력이 뒤섞이지 않도록 뮤텍스를 사용합니다.
    void SyncMessage(int i, const char* str) {
        std::lock_guard<std::mutex> lock(messageMutex); // 유효 범위를 벗어나면 unlock을 호출합니다.

        std::cout << std::format("Work {} : {}", i, str) << std::endl;
    }
    void Async(int i) {
        SyncMessage(i, "First");
        g_IsAsyncComplated.arrive_and_wait(); // #1. 숫자를 감소시키고 현 단계가 끝날때까지 대기합니다.

        SyncMessage(i, "Second");
        g_IsAsyncComplated.arrive_and_wait(); // #2. 숫자를 감소시키고 현 단계가 끝날때까지 대기합니다.
 
        SyncMessage(i, "Third");
        g_IsAsyncComplated.arrive_and_wait(); // #3. 숫자를 감소시키고 현 단계가 끝날때까지 대기합니다.
 
        SyncMessage(i, "Last"); // #4
    }
  
}

TEST(TestModern, Barrier) {
    using namespace Barrier_1;

    std::vector<std::thread> v;

    for (int i{0}; i < 5; ++i) {
        v.emplace_back(&Async, i);
    }

    for (auto& work : v) {
        work.join();
    }
}
#endif


