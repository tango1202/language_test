#include "Test_MordernCpp_InlineVariable.h" // g_Val_17은 inline 변수여서 중복 정의되지 않습니다.

#if 201703L <= __cplusplus // C++17~
// C++17 인라인 변수
namespace InlineVariable {
    void IncreaseVal2_17() {
        ++g_Val_17;
    }
}
#endif