#include "gtest/gtest.h" 

#if 201703L <= __cplusplus // C++17~
    #include <memory_resource>
#endif

TEST(TestMordern, PolymorphicAllocator) {
#if 201703L <= __cplusplus // C++17~
    // C++17 polymorphic_allocator
    {
        unsigned char data[100]; // 100 byte 공간을 할당합니다.
        std::pmr::monotonic_buffer_resource pool{data, sizeof(data)}; // 모든 요소가 소멸되면 메모리를 한번에 해제합니다.
        std::pmr::polymorphic_allocator<int> pa{&pool};

        std::vector<int, std::pmr::polymorphic_allocator<int>> v(pa); 

        v.push_back(1); 
        v.push_back(2);
        EXPECT_TRUE(v[0] == 1 && v[1] == 2); 
    }
#endif    
}