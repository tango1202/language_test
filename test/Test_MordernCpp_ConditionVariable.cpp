#include "gtest/gtest.h" 

#include <thread>

TEST(TestMordern, ConditionValriable) {
    {
        class A {
            std::string m_Str;
            std::condition_variable m_CV;
            bool m_IsRunProducer{false};
            bool m_IsRun1{false};
            bool m_IsRun2{false};
        public:
            const std::string& GetStr() const {return m_Str;}

            void Customer1(std::mutex& mutex) {

                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsRun1;}); // 누군가가 m_IsRunMake1 == true로 하고 m_CV에서 notify할때까지 대기

                std::cout<<"#2. Customer1()" <<std::endl;

                m_Str += "Hello";

                m_IsRunProducer = true; // Producer를 깨움
                m_IsRun1 = false;
                lock.unlock();
                m_CV.notify_all(); 

                // 아직 할일이 더 있어 wait
                lock.lock();
                m_CV.wait(lock, [&]() -> bool {return m_IsRun1;}); // 누군가가 m_IsRunMake1 == true로 하고 m_CV에서 notify할때까지 대기
                
                std::cout<<"#6. Costomer1()" <<std::endl;
                m_Str += "!!";

                m_IsRunProducer = true; // Producer를 깨움
                m_IsRun1 = false;
                lock.unlock();
                m_CV.notify_all(); 
            }
            void Customer2(std::mutex& mutex) {
                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsRun2;});// 누군가가 m_IsRunMake2 == true로 하고 m_CV에서 notify할때까지 대기

                std::cout<<"#4. Costomer2()" <<std::endl;

                m_Str += "World";

                m_IsRunProducer = true; // Producer를 깨움
                m_IsRun2 = false;
                lock.unlock();
                m_CV.notify_all(); 
            } 

            void Producer() {
                std::mutex mutex; // mutex 개체

                std::thread worker1{std::mem_fn(&A::Customer1), std::ref(*this), std::ref(mutex)};
                std::thread worker2{std::mem_fn(&A::Customer2), std::ref(*this), std::ref(mutex)};

                std::cout<<"#1. Producer Start" <<std::endl;
                m_IsRun1 = true; // Costomer1을 깨움
                m_CV.notify_all(); 

                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsRunProducer;});

                std::cout<<"#3. Producer Wake Up" <<std::endl;
                m_IsRunProducer = false;
                m_IsRun1 = false;
                m_IsRun2 = true; // Costomer2를 깨움
                lock.unlock();
                m_CV.notify_all(); 

                lock.lock();
                m_CV.wait(lock, [&]() -> bool {return m_IsRunProducer;});

                std::cout<<"#5. Producer Wake Up" <<std::endl;
                m_IsRunProducer = false;
                m_IsRun1 = true; // Costomer1를 깨움
                m_IsRun2 = false; 
                lock.unlock();
                m_CV.notify_all(); 

                lock.lock();
                m_CV.wait(lock, [&]() -> bool {return m_IsRunProducer;});

                std::cout<<"#6. Producer Completed" <<std::endl;

                worker1.join(); 
                worker2.join();
            }           
        };

        A a{};
        a.Producer();
        std::cout<<"Make String : "<<a.GetStr()<<std::endl; // Hello World!!를 출력함

    }

}