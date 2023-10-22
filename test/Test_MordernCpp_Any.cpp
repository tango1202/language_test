#include "gtest/gtest.h" 

TEST(TestMordern, Any) {
    {
        void* data{new int}; // int 타입을 사용합니다.
        *reinterpret_cast<int*>(data) = 10;
        delete reinterpret_cast<int*>(data);

        class T {};
        data = new T; // T 타입을 사용합니다.
        delete reinterpret_cast<T*>(data);        
    }
#if 201703L <= __cplusplus // C++17~    
    // C++17 any
    {
        std::any data{10}; // 정수 타입을 사용합니다.
        EXPECT_TRUE(std::any_cast<int>(data) == 10); // 데이터를 사용할때 any_cast를 이용합니다.
    
        class T {
        public:
            int m_Val{1};
        };

        data = T{}; // T 타입을 사용합니다.
        EXPECT_TRUE(std::any_cast<T>(data).m_Val == 1); 

        data.reset(); // 데이터를 초기화합니다.

        EXPECT_TRUE(data.has_value() == false);
    }
#endif    

}