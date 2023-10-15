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


TEST(TestMordern, Future) {
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