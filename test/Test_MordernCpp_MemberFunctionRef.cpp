#include "gtest/gtest.h" 

namespace {
    class Big_11 {
    public:    
        Big_11() {}
        ~Big_11() {}

        // 복사 생성자
        Big_11(const Big_11& other) {
            std::cout << "Big_11 : Copy Constructor" << std::endl;
        }
        // 이동 생성자
        Big_11(Big_11&& other) noexcept {
            std::cout << "Big_11 : Move Constructor" << std::endl;
        }  
    };     
}

TEST(TestMordern, MemberFunctionRef) {
    {
        class T {
        public:
            int Func_11() & {return 1;} // #1. 좌측값에서 호출
            int Func_11() && {return 2;} // #2. 우측값에서 호출
        };

        T t;
        EXPECT_TRUE(t.Func_11() == 1); // 좌측값이므로 #1 호출
        EXPECT_TRUE(std::move(t).Func_11() == 2); // move는 우측값이므로 #2 호출
        EXPECT_TRUE(T{}.Func_11() == 2); // T() 는 임시 개체(우측값)이므로 #2 호출   
    }
    {
        class Wrapper_11 {
            Big_11 m_Data;
        public:
            Wrapper_11() : m_Data{} {}
            const Big_11& GetData() const {
                return m_Data;
            }
        };

        {
            Wrapper_11 wrapper;
            const Big_11& big{wrapper.GetData()}; // const Big_11&을 리턴하므로 참조만 합니다.
        }
        {
            Wrapper_11 wrapper;
            Big_11 big{wrapper.GetData()}; // const Big_11& 를 리턴한 것을 Big_11로 받으므로 복사 생성 합니다.         
        }
        {
            // std::move(wrapper)로 우측값으로 만들어 봤자, std::move(a).GetData()는 const Big_11&(좌측값 참조)를 리턴
            Wrapper_11 wrapper;
            Big_11 big{std::move(wrapper).GetData()}; // 복사 생성자를 호출합니다.
        }  
 
        {
            // Wrapper_11()은 임시 개체인 우측값 이지만, 여전히 GetData()는 const Big_11&(좌측값 참조)를 리턴
            Big_11 big{Wrapper_11{}.GetData()}; // 복사 생성자를 호출합니다.
        }    
        {
            // Wrapper_11().GetData()는 const Big_11&를 리턴하고, move() 는 const Big_11&& 을 리턴. 이동 생성자와 인자 타입과 다르므로, 그냥 복사 생성자 호출 
            Big_11 big{
                std::move(
                    Wrapper_11{}.GetData()
                )
            };
        }     
        {
            // Big_11의 이동 생성자 호출
            Big_11 big{ // 3. 이동 생성자 호출
                std::move( // 2. Big&&로 변환하여
                    const_cast<Big_11&>( // 1. 상수성을 떼고
                        Wrapper_11{}.GetData()
                    )
                )
            };            
        }            
    }
    // const 타입 move()
    {
        class Wrapper_11 {
            Big_11 m_Data;
        public:
            Wrapper_11() : m_Data() {}
            const Big_11& GetData() const & { // #1. Wrapper_11가 좌측값일때 호출됩니다.
                return m_Data;
            }

            Big_11&& GetData() && {
                return std::move(m_Data); // #2. Wrapper_11가 우측값일때 호출됩니다.
            }
        };

        {
            // wrapper은 좌측값. #1이 호출되어 const Big_11&을 리턴하고, Big_11의 복사 생성자 호출
            Wrapper_11 wrapper;
            Big_11 big = wrapper.GetData(); 
        }
        {
            // wrapper은 좌측값. std::move(wrapper)는 우측값. #2가 호출되어 Big_11&&을 리턴하고, Big_11의 이동 생성자 호출
            Wrapper_11 wrapper;
            Big_11 big = std::move(wrapper).GetData();            
        }
        {
            // Wrapper_11()은 임시 개체인 우측값. #2가 호출되어 Big_11&&을 리턴하고, Big_11의 이동 생성자 호출
            Big_11 big{Wrapper_11().GetData()}; 
        }        
    } 
    
}

