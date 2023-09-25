#include "gtest/gtest.h" 

#include <thread>

TEST(TestMordern, Atomic) {
    {
        class A {
            // int m_Val{0}; // 일반 변수는 mutex를 사용해야 합니다.
            std::atomic<int> m_Val{0};
        public:
            int GetVal() const {return m_Val;}
            
            // i를 m_Val을 100번 증가시킵니다.
            void Increase() {
                for (int i{0}; i < 100; ++i) {
                    // m_Val = m_Val + 1; // atomic은 + 를 지원하지 않습니다. 
                    ++m_Val;
                    std::this_thread::sleep_for(std::chrono::milliseconds{1}); // 1 밀리초 만큼 쉽니다.
                }
            }
        };

        A a{};
        std::thread worker1{std::mem_fn(&A::Increase), std::ref(a)};
        std::thread worker2{std::mem_fn(&A::Increase), std::ref(a)};

        worker1.join(); 
        worker2.join(); 

        EXPECT_TRUE(a.GetVal() == 200); // (O) 경쟁 상태에 빠지지 않고 잘 계산합니다.
    }
    {
        class A {
            int m_Val1{0};
            int m_Val2{0};
            int m_Val3{0};

            std::atomic<bool> m_IsCompleted{false};
        public:
            void Producer() {

                m_Val1 = 1; // #1
                m_Val2 = 2; // #2

                // #1, #2가 나중에 실행되지 않도록 보증합니다.
                m_IsCompleted.store(true, std::memory_order_release); // #3

                m_Val3 = 3; // #4
            }

            void Consumer() {

                // m_IsCompleted가 true가 될때까지 while 문을 반복하며 대기합니다.
                // #6, #7, #8 이 먼저 실행되지 않도록 보증합니다.
                while(!m_IsCompleted.load(std::memory_order_acquire)) {} // #5
                
                EXPECT_TRUE(m_Val1 == 1); // #6
                EXPECT_TRUE(m_Val2 == 2); // #7
                // #4가 #3보다 먼저 실행될 수도 있고, 아닐 수도 있으므로 m_Val3은 0 또는 3입니다.
                EXPECT_TRUE(m_Val3 == 0 || m_Val3 == 3); // #8
            }
        };
        A a{};
        std::thread producer{std::mem_fn(&A::Producer), std::ref(a)};
        std::thread consumer{std::mem_fn(&A::Consumer), std::ref(a)};
    }
}