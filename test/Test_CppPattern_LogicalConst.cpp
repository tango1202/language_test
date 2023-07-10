#include "gtest/gtest.h" 

TEST(TestCppPattern, LogicalConst) {
    // ----
    // 논리적 상수성
    // ----
    // 그때 그때 계산
    {
        class Date {
            int m_Year;
            int m_Month;
            int m_Day;
        public: 
            Date(int year, int month, int day) :
                m_Year(year),
                m_Month(month), 
                m_Day(day) {
            }

            // Getter/Setter
            void SetYear(int val) {m_Year = val;} 
            void SetMonth(int val) {m_Month = val;}
            void SetDay(int val) {m_Day = val;}
        public:
            // (△) 비권장. 호출할 때마다 문자열을 새롭게 만듭니다.
            // 데이터를 yyyy-mm-dd 형식으로 만듬
            std::string MakeText() const { // 멤버 변수를 수정하지 않으므로 const
                char buffer[] = "yyyy-mm-dd";
                snprintf(buffer, 11, "%04d-%02d-%02d", m_Year, m_Month, m_Day); // 널문자 포함 11
                return buffer;
            }
        };
        Date date(20, 2, 10); // 20년 2월 10일
        EXPECT_TRUE(date.MakeText().compare("0020-02-10") == 0);         
    }
    // 값이 수정될때
    {
        class Date {
            int m_Year;
            int m_Month;
            int m_Day;
            std::string m_Text; // yyyy-mm-dd
        public: 
            Date(int year, int month, int day) :
                m_Year(year),
                m_Month(month), 
                m_Day(day),
                m_Text() {
                MakeText(); // (△) 비권장. 값이 설정되었으니 문자열을 만듬
            }

            // Getter/Setter
            void SetYear(int val) {
                m_Year = val; 
                MakeText(); // (△) 비권장. 값이 수정되었으니 문자열을 만듬
            } 
            void SetMonth(int val) {
                m_Month = val;
                MakeText(); // (△) 비권장. 값이 수정되었으니 문자열을 만듬
            }
            void SetDay(int val) {
                m_Day = val;
                MakeText(); // (△) 비권장. 값이 수정되었으니 문자열을 만듬
            }

            const std::string& GetText() const {return m_Text;}

        private:
            // 데이터를 yyyy-mm-dd 형식으로 만듬
            void MakeText() {
                char buffer[] = "yyyy-mm-dd";
                snprintf(buffer, 11, "%04d-%02d-%02d", m_Year, m_Month, m_Day); // 널문자 포함 11
                m_Text = buffer;
            }
        };
        Date date(20, 2, 10); // 20년 2월 10일
        EXPECT_TRUE(date.GetText().compare("0020-02-10") == 0); 
    }
    // 캐쉬
    {
        class Date {
            int m_Year;
            int m_Month;
            int m_Day;
            std::string m_Cached; // 문자열을 캐쉬해 둡니다.
        public: 
            Date(int year, int month, int day) :
                m_Year(year),
                m_Month(month), 
                m_Day(day),
                m_Cached() {
            }

            // Getter/Setter
            void SetYear(int val) {
                m_Year = val;
                m_Cached.clear(); // 캐쉬를 초기화
            } 
            void SetMonth(int val) {
                m_Month = val;
                m_Cached.clear(); // 캐쉬를 초기화
            }
            void SetDay(int val) {
                m_Day = val;
                m_Cached.clear(); // 캐쉬를 초기화
            }
        public:
            // (O) 한번 호출되면 데이터를 캐쉬해 둡니다.
            // (O) 멤버 변수를 리턴하므로 const std::string& 리턴
            // (△) 비권장. 멤버 변수의 값을 리턴하는데 상수 멤버 함수가 아닙니다.
            // 데이터를 yyyy-mm-dd 형식으로 만듬
            const std::string& MakeText() { 

                // 캐쉬된게 있다면 그대로 사용
                if (m_Cached.empty() == false) {
                    return m_Cached;
                }

                // 문자열을 만들어 캐쉬에 저장
                char buffer[] = "yyyy-mm-dd";
                snprintf(buffer, 11, "%04d-%02d-%02d", m_Year, m_Month, m_Day); // 널문자 포함 11
                m_Cached = buffer;

                return m_Cached;
            }
        };
        Date date(20, 2, 10); // 20년 2월 10일
        EXPECT_TRUE(date.MakeText().compare("0020-02-10") == 0); 
        EXPECT_TRUE(date.MakeText().compare("0020-02-10") == 0); // 기존 캐쉬를 사용

        date.SetYear(21); // 년도를 21년으로 변경. 캐쉬가 초기화됨
        EXPECT_TRUE(date.MakeText().compare("0021-02-10") == 0); // 문자열 생성
    }
    // mutable
    {
        class Date {
            int m_Year;
            int m_Month;
            int m_Day;
            mutable std::string m_Cached; // (O) 상수 멤버 함수에서 수정 가능합니다. 문자열을 캐쉬해 둡니다.
        public: 
            Date(int year, int month, int day) :
                m_Year(year),
                m_Month(month), 
                m_Day(day),
                m_Cached() {
            }

            // Getter/Setter
            void SetYear(int val) {
                m_Year = val;
                m_Cached.clear(); // 캐쉬를 초기화
            } 
            void SetMonth(int val) {
                m_Month = val;
                m_Cached.clear(); // 캐쉬를 초기화
            }
            void SetDay(int val) {
                m_Day = val;
                m_Cached.clear(); // 캐쉬를 초기화
            }

            // (O) 한번 호출되면 데이터를 캐쉬해 둡니다.
            // (O) 멤버 변수를 리턴하므로 const std::string& 리턴
            // (O) mutable 개체를 수정합니다.
            // 데이터를 yyyy-mm-dd 형식으로 만듬
            const std::string& GetText() const { 

                // 캐쉬된게 있다면 그대로 사용
                if (m_Cached.empty() == false) {
                    return m_Cached;
                }

                // 문자열을 만들어 캐쉬에 저장
                char buffer[] = "yyyy-mm-dd";
                snprintf(buffer, 11, "%04d-%02d-%02d", m_Year, m_Month, m_Day); // 널문자 포함 11
                m_Cached = buffer;

                return m_Cached;
            }
        };

        Date date(20, 2, 10); // 20년 2월 10일
        EXPECT_TRUE(date.GetText().compare("0020-02-10") == 0); 
        EXPECT_TRUE(date.GetText().compare("0020-02-10") == 0); // 기존 캐쉬를 사용

        date.SetYear(21); // 년도를 21년으로 변경. 캐쉬가 초기화됨
        EXPECT_TRUE(date.GetText().compare("0021-02-10") == 0); // 문자열 생성        
    }
}

