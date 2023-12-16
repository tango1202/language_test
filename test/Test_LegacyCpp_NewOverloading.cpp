#include "gtest/gtest.h" 


#define MY_DEBUG

#ifdef MY_DEBUG
// MY_DEBUG 가 정의된 경우만 사용하는 new 관련 정보입니다.
class NewInfo {
public:
    std::string m_Filename; // 파일명
    int m_Line; // 라인 번호
    NewInfo(const char* filename, int line) : m_Filename(filename), m_Line(line) {}
};
std::map<void*, NewInfo> g_NewInfoMap; // #1

#endif 

TEST(TestLegacyCpp, NewOverloading) { 
    {
        class T {
        public:
#ifdef MY_DEBUG // MY_DEBUG 모드에서는 new 할때 g_NewInfoMap에 파일명과 라인 번호를 저장 delete시 지웁니다. 
            // 눈을 어지럽힐 수 있으니, 예외처리 부분은 생략했습니다.
            static void* operator new(size_t sz, const char* filename, int line) {
                void* ptr = ::operator new(sz);

                g_NewInfoMap.insert( // #2
                    std::make_pair(
                        ptr, // 할당한 영역을 키로하여
                        NewInfo(filename, line) // new 시의 파일명, 라인 번호 저장
                    )
                );
                return ptr;
            }
            static void operator delete(void* ptr) {
              
                std::map<void*, NewInfo>::iterator result = g_NewInfoMap.find(ptr); // #3
                if (result != g_NewInfoMap.end()) {
                    g_NewInfoMap.erase(result); // 찾았다면 삭제
                }
                ::operator delete(ptr);
            }
#endif
        };

#ifdef MY_DEBUG
    #define new new(__FILE__, __LINE__) // #4. MY_DEBUG 모드에서는 new를 new(__FILE__, __LINE__)로 대체합니다.
#endif

        T* ptr1 = new T; // MY_DEBUG 모드에서는 new(__FILE__, __LINE__) 입니다.
        delete ptr1;

        // T* ptr2 = new T; // MY_DEBUG 모드에서는 new(__FILE__, __LINE__) 입니다.
        // delete ptr2; // 테스트를 위해 의도적으로 delete를 생략합니다.
    }
#ifdef MY_DEBUG
    // EXPECT_TRUE(g_NewInfoMap.size() == 1); // #5. 무언가가 남아 있다면 메모리 릭입니다.
#endif
}
#undef MY_DEBUG