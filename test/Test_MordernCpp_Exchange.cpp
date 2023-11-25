#include "gtest/gtest.h" 

TEST(TestMordern, Exchange) {
    // C++11 exchange
    {
        class Big {
            size_t m_Size;
            char* m_Ptr; // 크기가 큰 데이터

            Big(Big&& other) noexcept : 
                m_Size(other.m_Size),
                m_Ptr(other.m_Ptr) {

                other.m_Size = 0; // other는 0, nullptr로 초기화 됩니다.
                other.m_Ptr = nullptr;
            } 
            // 이동 대입 연산자. 우측값(rvalue) 이동
            Big& operator =(Big&& other) noexcept {
                delete[] m_Ptr; // 기존 것은 삭제하고,
                m_Size = other.m_Size; // other 것을 저장한뒤
                m_Ptr = other.m_Ptr;

                other.m_Size = 0; // other는 초기화 합니다.
                other.m_Ptr = nullptr;
                return *this;
            }   
        };
    }
#if 201402L <= __cplusplus // C++14~        
    // C++14 exchange
    {
        class Big_14 {
            size_t m_Size;
            char* m_Ptr; // 크기가 큰 데이터

            Big_14(Big_14&& other) noexcept : 
                m_Size{std::exchange(other.m_Size, 0)}, // other는 0, nullptr로 초기화 됩니다.
                m_Ptr{std::exchange(other.m_Ptr, nullptr)} {} 
            // 이동 대입 연산자. 우측값(rvalue) 이동
            Big_14& operator =(Big_14&& other) noexcept {
                delete[] m_Ptr; // 기존 것은 삭제하고,

                m_Size = std::exchange(other.m_Size, 0);  // other는 0, nullptr로 초기화 됩니다.
                m_Ptr = std::exchange(other.m_Ptr, nullptr);

                return *this;
            }   
        };
    }
#endif    
}