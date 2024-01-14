 #ifndef Test_ModernCpp_InlineVariable_h
 #define Test_ModernCpp_InlineVariable_h


#if 201703L <= __cplusplus // C++17~
// C++17 인라인 변수
namespace InlineVariable {
    inline int g_Val_17{10};  
}
#endif
#endif // Test_ModernCpp_InlineVariable_h