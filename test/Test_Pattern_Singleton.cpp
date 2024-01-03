#include "gtest/gtest.h" 


namespace {

    enum class ErrorCode {Unknown, InvalidArgument, OutOfRange};

    class ErrorCodeManager {
        std::map<ErrorCode, const char*> m_Map;
    private:
        ErrorCodeManager() { // #2. 외부에서는 생성지 못하게 합니다.
            m_Map.insert(std::make_pair(ErrorCode::Unknown, "Unknown"));
        }
        ErrorCodeManager(const ErrorCodeManager&) = delete; 
        ErrorCodeManager(ErrorCodeManager&&) = delete; 
        ErrorCodeManager& operator =(const ErrorCodeManager&) = delete; 
        ErrorCodeManager& operator =(ErrorCodeManager&&) = delete;   

    public:
        // ----
        // Singleton
        // ----
        static ErrorCodeManager& GetInstance() {
            static ErrorCodeManager s_ErrorCodeManager; // #1. 함수내 정적 지역 변수여서 함수 호출시 1회만 생성됩니다.

            return s_ErrorCodeManager;
        } 

        // #3. 동일한 ErrorCode가 있으면 수정하고, 없으면 추가합니다.
        void SetMessage(ErrorCode errorCode, const char* message) {
            auto result = m_Map.insert(std::make_pair(errorCode, message));
            
            // 동일한 ErrorCode가 있어서 insert에 실패하면 message를 변경합니다.
            if (result.second == false) {
                (*(result.first)).second = message;
            }
        } 
        // #3. errorCode인 문자열을 리턴하고, 없으면 nullptr을 리턴합니다.
        const char* GetMessage(ErrorCode errorCode) const {
            auto result = m_Map.find(errorCode);
            if (result == m_Map.end()) {
                return nullptr;
            } 
            return (*result).second;
        }         
    };
}

TEST(TestPattern, Singleton) {

    {
        // #4. 등록하지 않은 것들을 등록합니다.
        ErrorCodeManager::GetInstance().SetMessage(ErrorCode::InvalidArgument, "Invalid Argument");
        ErrorCodeManager::GetInstance().SetMessage(ErrorCode::OutOfRange, "Out Of Range");
    
        // #4. 원래 입력했던 Unknown 과 새롭게 등록한 Invalid Argument, Out Of Range가 있습니다.
        EXPECT_TRUE(ErrorCodeManager::GetInstance().GetMessage(ErrorCode::Unknown) == "Unknown");
        EXPECT_TRUE(ErrorCodeManager::GetInstance().GetMessage(ErrorCode::InvalidArgument) == "Invalid Argument");
        EXPECT_TRUE(ErrorCodeManager::GetInstance().GetMessage(ErrorCode::OutOfRange) == "Out Of Range");
    }
}