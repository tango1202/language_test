#include "gtest/gtest.h" 

#include <thread>
#include <shared_mutex>

namespace {
    void Message1() {
        for(int i{0};i < 100; ++i) {
            std::cout << "Message1 : " << i << std::endl;
        }        
    }
     void Message2() {
        for(int i{0};i < 100; ++i) {
            std::cout << "Message2 : " << i << std::endl;
        }        
    }   
    void Sum(std::vector<int>::iterator itr, std::vector<int>::iterator endItr, int& result) {
        result = 0;

        for(;itr != endItr; ++itr) {
            result += *itr;
            //!! 테스트 속도 때문에 임시 주석
            //!! std::this_thread::sleep_for(std::chrono::milliseconds{1}); // 1 밀리초 만큼 쉽니다.
        }
    }

    void ThreadSum(std::vector<int>::iterator itr, std::vector<int>::iterator endItr, int& result) {

        std::vector<int>::iterator middleItr{itr};
        std::advance(middleItr, std::distance(itr, endItr) / 2); // 중간 지점 이터레이터를 구함

        int result1{0};
        std::thread worker1{ // [시작 ~ 중간) 까지의 Sum
            Sum, 
            itr, middleItr, 
            std::ref(result1) // 인수의 참조성 유지
        }; 

        int result2{0};
        std::thread worker2{ // [중간 ~ 끝) 까지의 Sum
            Sum, 
            middleItr, endItr, 
            std::ref(result2) // 인수의 참조성 유지
        }; 

        worker1.join(); // worker1이 종료될때까지 기다립니다.
        worker2.join(); // worker2가 종료될때까지 기다립니다.

        // worker1, worker2 결과를 더합니다.
        result = result1 + result2;
    }    

    template<typename Func, typename... Params>
    std::chrono::microseconds Measure(Func func, Params... params) {
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    
 
        func(params...);

        std::chrono::system_clock::time_point end{std::chrono::system_clock::now()};
        std::chrono::microseconds val{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        return val;
    }

    void UniqueRead(std::vector<int>::iterator itr, std::vector<int>::iterator endItr, std::mutex& mutex) {
        for(int i = 0; itr != endItr; ++itr, ++i) {
            std::unique_lock<std::mutex> lock(mutex); // mutex를 독점합니다.
            *itr = 1;
            //!! 테스트 속도 때문에 임시 주석
            //!! std::this_thread::sleep_for(std::chrono::milliseconds{1}); 
            lock.unlock();
        }
    }
    void UniqueFunc(std::vector<int>::iterator itr, std::vector<int>::iterator endItr) {


        std::mutex mutex; // mutex 개체
        std::thread worker1{UniqueRead, itr, endItr, std::ref(mutex)};
        std::thread worker2{UniqueRead, itr, endItr, std::ref(mutex)};
        std::thread worker3{UniqueRead, itr, endItr, std::ref(mutex)};
        std::thread worker4{UniqueRead, itr, endItr, std::ref(mutex)};
        worker1.join(); 
        worker2.join(); 
        worker3.join();
        worker4.join();
    }
    void SharedWrite_14(std::vector<int>::iterator itr, std::vector<int>::iterator endItr, std::shared_timed_mutex& mutex) {
        for(int i = 0; itr != endItr; ++itr, i++) {
            std::shared_lock<std::shared_timed_mutex> lock(mutex); // mutex를 공유합니다.
            *itr = 1;
            //!! 테스트 속도 때문에 임시 주석
            //!! std::this_thread::sleep_for(std::chrono::milliseconds{1}); 
            lock.unlock();
        }
    }
    void SharedFunc_14(std::vector<int>::iterator itr, std::vector<int>::iterator endItr) {

        std::shared_timed_mutex mutex; // mutex 개체
        std::thread worker1{SharedWrite_14, itr, endItr, std::ref(mutex)};
        std::thread worker2{SharedWrite_14, itr, endItr, std::ref(mutex)};
        std::thread worker3{SharedWrite_14, itr, endItr, std::ref(mutex)};
        std::thread worker4{SharedWrite_14, itr, endItr, std::ref(mutex)};
        worker1.join(); 
        worker2.join(); 
        worker3.join();
        worker4.join();
    }    

};


TEST(TestMordern, Thread) {
    {
        std::thread worker1{Message1};
        std::thread worker2{Message2};

        worker1.join(); // worker1이 종료될때까지 기다립니다.
        worker2.join(); // worker2가 종료될때까지 기다립니다.
    }

    {
        std::vector<int> v;

        for (int i{0}; i < 100; ++i) {
            v.push_back(i);
        }

        int result{0};
        std::chrono::microseconds duration{Measure(
            Sum, 
            v.begin(), v.end(), 
            std::ref(result) // 인수의 참조성 유지
        )};
        std::cout << "Sum : " << result << " Duration : " << duration.count() << std::endl; 

        int threadResult{0};
        std::chrono::microseconds threadDuration{Measure(
            ThreadSum, 
            v.begin(), v.end(), 
            std::ref(threadResult) // 인수의 참조성 유지
        )};
        std::cout << "ThreadSum : " << threadResult << " Duration : " << threadDuration.count() << std::endl; 
    }
    // mutex
    {
        class A {
            int m_Val{0};
        public:
            int GetVal() const {return m_Val;}
            
            // i를 m_Val을 100번 증가시킵니다.
            void Increase() {
                for (int i{0}; i < 100; ++i) {
                    m_Val = m_Val + 1;
                    //!! 테스트 속도 때문에 임시 주석
                    //!! std::this_thread::sleep_for(std::chrono::milliseconds{1}); // 1 밀리초 만큼 쉽니다.
                }
            }
            
        };

        A a{};
        std::thread worker1{std::mem_fn(&A::Increase), std::ref(a)};
        std::thread worker2{std::mem_fn(&A::Increase), std::ref(a)};

        worker1.join(); 
        worker2.join(); 

        std::cout << "Non Mutex : " << a.GetVal() << std::endl;
    }
    {
        class A {
            int m_Val{0};
            
        public:
            int GetVal() const {return m_Val;}
            
            void Increase(std::mutex& mutex) {
                for (int i{0}; i < 100; ++i) {
                    mutex.lock(); // 값을 읽고 쓰기 전에 다른 쓰레드를 기다리게 합니다.
                    m_Val = m_Val + 1;
                    mutex.unlock(); // lock을 해제 합니다.
                    //!! 테스트 속도 때문에 임시 주석
                    //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
                }
            }
        };

        A a{};
        std::mutex mutex; // mutex 개체
        std::thread worker1{std::mem_fn(&A::Increase), std::ref(a), std::ref(mutex)};
        std::thread worker2{std::mem_fn(&A::Increase), std::ref(a), std::ref(mutex)};

        worker1.join(); 
        worker2.join(); 

        std::cout << "Mutex : " << a.GetVal() << std::endl;
        EXPECT_TRUE(a.GetVal() == 200);
    }
    {
        // 데드락 예제
        // class A {
        // public:
        //     void Func(std::mutex& mutex) {
        //         for (int i{0}; i < 100; ++i) {
        //             mutex.lock(); 

        //             // Todo

        //             // mutex.unlock(); // (X) 오동작. 실수로 빼먹으면 다른 쓰레드가 무한 대기합니다.
        //             std::this_thread::sleep_for(std::chrono::milliseconds{1});
        //         }
        //     }
        // };

        // A a{};
        // std::mutex mutex; // mutex 개체

        // std::thread worker1{std::mem_fn(&A::Func), std::ref(a), std::ref(mutex)};
        // std::thread worker2{std::mem_fn(&A::Func), std::ref(a), std::ref(mutex)};
    
        // worker1.join();
        // worker2.join(); 
    }
    {
        // 데드락 예제
        // class A {
        // public:
        //     void Init(std::mutex& myMutex, std::mutex& yourMutex) {
        //         for (int i{0}; i < 100; ++i) {
        //             std::lock_guard<std::mutex> lock1{myMutex}; // myMutex 후 yourMutex 를 lock 합니다.
        //             std::lock_guard<std::mutex> lock2{yourMutex}; 
        //             // Todo

        //             std::this_thread::sleep_for(std::chrono::milliseconds{1});
        //         }
        //     }
        //     void Reset(std::mutex& myMutex, std::mutex& yourMutex) {

        //         for (int i{0}; i < 100; ++i) {
        //             std::lock_guard<std::mutex> lock1{yourMutex}; // yourMutex후 myMutex 를 lock 합니다. 
        //             std::lock_guard<std::mutex> lock2{myMutex}; 
        //             // Todo
        //             std::this_thread::sleep_for(std::chrono::milliseconds{1});
        //         }
        //     }
        // };

        // A a{};
        // std::mutex myMutex; // mutex 개체
        // std::mutex yourMutex; 

        // std::thread worker1{std::mem_fn(&A::Init), std::ref(a), std::ref(myMutex), std::ref(yourMutex)};
        // std::thread worker2{std::mem_fn(&A::Reset), std::ref(a), std::ref(myMutex), std::ref(yourMutex)};
    
        // worker1.join(); 
        // worker2.join(); 
    }

    {
        class A {
        public:
            void Init(std::mutex& myMutex, std::mutex& yourMutex) {
                for (int i{0}; i < 100; ++i) {
                    myMutex.lock(); // myMutex 후 yourMutex 를 lock 합니다.
                    yourMutex.lock();
                    // Todo
                    yourMutex.unlock();
                    myMutex.unlock();
                    //!! 테스트 속도 때문에 임시 주석
                    //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
                }
            }
            void Reset(std::mutex& myMutex, std::mutex& yourMutex) {

                for (int i{0}; i < 100; ++i) {
                    yourMutex.lock(); // yourMutex 후  myMutex를 lock 합니다.
                    if (!myMutex.try_lock()) { // myMutex가 다른곳에서 벌써 사용중이라면, yourMutex lock을 다른 쓰레드가 사용하도록 양보합니다.
                        yourMutex.unlock();
                        continue;
                    }
                    // Todo
                    myMutex.unlock();
                    yourMutex.unlock();
                    //!! 테스트 속도 때문에 임시 주석
                    //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
                }
            }
        };

        A a{};
        std::mutex myMutex; // mutex 개체
        std::mutex yourMutex; 

        std::thread worker1{std::mem_fn(&A::Init), std::ref(a), std::ref(myMutex), std::ref(yourMutex)};
        std::thread worker2{std::mem_fn(&A::Reset), std::ref(a), std::ref(myMutex), std::ref(yourMutex)};
    
        worker1.join(); 
        worker2.join(); 
    }
    {
        class A {
            std::once_flag m_OnceFlag; // call_once() 시에 사용되는 개체입니다. 이 개체에 호출 정보가 담겨 있어 다음번 호출시 비교됩니다.
        public:
            void OnceFunc() {
                // this->Func() 함수를 여러 쓰레드에서 호출해도 한번만 호출합니다.
                std::call_once(m_OnceFlag, std::mem_fn(&A::Func), std::ref(*this));
            } 
            void Func() {
                std::cout << "A : Func()" << std::endl;
            }
        };

        A a;
        std::thread worker1{std::mem_fn(&A::OnceFunc), std::ref(a)};
        std::thread worker2{std::mem_fn(&A::OnceFunc), std::ref(a)};

        worker1.join(); 
        worker2.join();

        a.OnceFunc(); // 더이상 Func()을 호출하지 않습니다.
    }
    // C++14 shared_timed_mutex 와 shared_lock
    {
        std::vector<int> v;

        for (int i{0}; i < 100; ++i) {
            v.push_back(i);
        }

        std::chrono::microseconds uniqueDuration{Measure(
            UniqueFunc, 
            v.begin(), v.end()
        )};
        std::cout << "UniqueFunc : " << uniqueDuration.count() << std::endl; 
        for (int i = 0; i < 100; ++i) {
            EXPECT_TRUE(v[i] == 1);
            v[i] = i; // 다시 값을 초기화 해둡니다.
        }

        std::chrono::microseconds sharedDuration{Measure(
            SharedFunc_14, 
            v.begin(), v.end() 
        )};
        std::cout << "SharedFunc : " << sharedDuration.count() << std::endl;
        for (int i = 0; i < 100; ++i) {
            EXPECT_TRUE(v[i] == 1);
        } 
    }
    // (C++17~) scoped_lock
    {
        class A {
        public:
            void Init(std::mutex& myMutex, std::mutex& yourMutex) {
                for (int i{0}; i < 100; ++i) {
                   std::scoped_lock{myMutex, yourMutex}; // 내부적으로 데드락을 방지합니다. 
                    // Todo
                    //!! 테스트 속도 때문에 임시 주석
                    //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
                }
            }
            void Reset(std::mutex& myMutex, std::mutex& yourMutex) {

                for (int i{0}; i < 100; ++i) {
                    std::scoped_lock{myMutex, yourMutex}; // 내부적으로 데드락을 방지합니다. 
                    // Todo
                    //!! 테스트 속도 때문에 임시 주석
                    //!! std::this_thread::sleep_for(std::chrono::milliseconds{1});
                }
            }
        };

        A a{};
        std::mutex myMutex; // mutex 개체
        std::mutex yourMutex; 

        std::thread worker1{std::mem_fn(&A::Init), std::ref(a), std::ref(myMutex), std::ref(yourMutex)};
        std::thread worker2{std::mem_fn(&A::Reset), std::ref(a), std::ref(myMutex), std::ref(yourMutex)};
    
        worker1.join(); 
        worker2.join(); 
    }



}