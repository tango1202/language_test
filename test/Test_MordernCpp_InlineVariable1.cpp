#include "Test_MordernCpp_InlineVariable.h"

#if 201703L <= __cplusplus // C++17~
// C++17 인라인 변수
namespace InlineVariable {
    void IncreaseVal1_17() {
        ++g_Val_17;
    }
}
#endif