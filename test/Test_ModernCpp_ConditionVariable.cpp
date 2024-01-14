#include "gtest/gtest.h" 

#include <thread>

TEST(TestModern, ConditionVariable) {
    {
        class A {
            std::string m_Str;
            std::condition_variable m_CV;
            bool m_IsRunBuild{false}; // true면 Build가 wait를 멈춥니다.
            bool m_IsRun1{false}; // true면 Part1이 wait를 멈춥니다.
            bool m_IsRun2{false}; // true면 Part2가 wait를 멈춥니다.
        public:
            const std::string& GetStr() const {return m_Str;}

            void Part1(std::mutex& mutex) {

                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsRun1;}); // 누군가가 m_IsRun1 == true로 하고 m_CV에서 notify할때까지 대기

                std::cout << "#2. Part1()" << std::endl;

                m_Str += "Hello";

                m_IsRunBuild = true; // Build를 깨움
                m_IsRun1 = false;
                lock.unlock();
                m_CV.notify_all(); 

                // 아직 할일이 더 있어 wait
                lock.lock();
                m_CV.wait(lock, [&]() -> bool {return m_IsRun1;}); // 누군가가 m_IsRun1 == true로 하고 m_CV에서 notify할때까지 대기
                
                std::cout << "#6. Part1()" << std::endl;
                m_Str += "!!";

                m_IsRunBuild = true; // Build를 깨움
                m_IsRun1 = false;
                lock.unlock();
                m_CV.notify_all(); 
            }
            void Part2(std::mutex& mutex) {
                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsRun2;});// 누군가가 m_IsRun2 == true로 하고 m_CV에서 notify할때까지 대기

                std::cout << "#4. Part2()" << std::endl;

                m_Str += "World";

                m_IsRunBuild = true; // Build를 깨움
                m_IsRun2 = false;
                lock.unlock();
                m_CV.notify_all(); 
            } 

            void Build() {
                std::mutex mutex; // mutex 개체

                std::thread worker1{std::mem_fn(&A::Part1), std::ref(*this), std::ref(mutex)};
                std::thread worker2{std::mem_fn(&A::Part2), std::ref(*this), std::ref(mutex)};

                std::cout << "#1. Build Start" << std::endl;
                m_IsRun1 = true; // Part1을 깨움
                m_CV.notify_all(); 

                std::unique_lock<std::mutex> lock{mutex};
                m_CV.wait(lock, [&]() -> bool {return m_IsRunBuild;});

                std::cout << "#3. Build Wake Up" << std::endl;
                m_IsRunBuild = false;
                m_IsRun1 = false;
                m_IsRun2 = true; // Part2를 깨움
                lock.unlock();
                m_CV.notify_all(); 

                lock.lock();
                m_CV.wait(lock, [&]() -> bool {return m_IsRunBuild;});

                std::cout << "#5. Build Wake Up" << std::endl;
                m_IsRunBuild = false;
                m_IsRun1 = true; // Part1를 깨움
                m_IsRun2 = false; 
                lock.unlock();
                m_CV.notify_all(); 

                lock.lock();
                m_CV.wait(lock, [&]() -> bool {return m_IsRunBuild;});

                std::cout << "#7. Build Completed" << std::endl;

                worker1.join(); 
                worker2.join();
            }           
        };

        A a{};
        a.Build();
        std::cout << "Make String : " << a.GetStr() << std::endl; // HelloWorld!!를 출력함

    }

}