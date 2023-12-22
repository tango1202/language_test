#include "gtest/gtest.h" 
#include <chrono>
#include <iostream>
#include <format>

namespace {
    template<typename Func, typename... Params>
    std::chrono::microseconds Measure(Func func, Params&&... params) {
        // func 실행전 time_point 측정
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    

        // func 실행
        func(std::forward<Params>(params)...);

        // func 실행 후 time_point 측정
        std::chrono::system_clock::time_point end{std::chrono::system_clock::now()};

        // 두 time_point 간의 차
        std::chrono::microseconds val{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        return val;
    }
    void MyFunc() {}
}

TEST(TestMordern, Chrono) {
    {
        std::chrono::microseconds duration{Measure(MyFunc)};

        std::cout << "MyFunc() : " << duration.count() << std::endl; 
    }
}
TEST(TestMordern, hh_mm_ss) {
    // hh_mm_ss
    {
#if 202002L <= __cplusplus // C++20~  
        using namespace std::chrono; // ""h, ""min, ""s사용을 위해 using namespace를 사용합니다. 
        constexpr std::chrono::hh_mm_ss time{12h + 72min + 3s};
        
        static_assert(time.hours() == 13h); // 12h에서 72min을 더해서 13h 12min이 되었습니다.
        static_assert(time.minutes() == 12min);
        static_assert(time.seconds() == 3s);

        static_assert(std::chrono::is_am(13h) == false);
        static_assert(std::chrono::is_pm(13h) == true);
        static_assert(std::chrono::make12(13h) == 1h);
        static_assert(std::chrono::make24(1h, true) == 13h); // pm 1h를 24시간 서식으로 변경하여 13h가 되었습니다. 
#endif
    }
}
TEST(TestMordern, Calendar) {
#if 202002L <= __cplusplus // C++20~    
    // calendar
    {
        static_assert(std::chrono::Sunday == std::chrono::weekday(0)); // 각 요일은 미리 정의되었습니다.
        static_assert(std::chrono::December == std::chrono::month(12)); // 각월은 미리 정의되었습니다.

        // 년-월-일
        {
            // 년, 월, 일을 전달합니다.
            std::chrono::year_month_day yearMonthDay1{std::chrono::year{2023}, std::chrono::month{12}, std::chrono::day{25}};
            
            // / 표기를 사용하여 년월일을 합성해서 사용할 수 있습니다.
            std::chrono::year_month_day yearMonthDay2{std::chrono::year{2023}/std::chrono::month{12}/std::chrono::day{25}};
            std::chrono::year_month_day yearMonthDay3{std::chrono::day{25}/std::chrono::month{12}/std::chrono::year{2023}};
            std::chrono::year_month_day yearMonthDay4{std::chrono::month{12}/std::chrono::day{25}/std::chrono::year{2023}};
        
            using namespace std::chrono; // 표준 사용자 정의 리터럴인 2023y, 25d를 사용하기 위해서 추가합니다.
            std::chrono::year_month_day yearMonthDay5{2023y/std::chrono::December/25d};
        }
        {
            // 2023년 12월의 마지막 날
            std::chrono::year_month_day date{std::chrono::year{2023}/std::chrono::month{12}/std::chrono::last};
            EXPECT_TRUE(date.day() == std::chrono::day{31});
        }
        {
            // 2023년 11월의 마지막 일요일
            std::chrono::year_month_weekday_last date1{std::chrono::year{2023}/std::chrono::month{11}/std::chrono::Sunday[std::chrono::last]}; 
            std::chrono::year_month_day date2{date1};
            EXPECT_TRUE(date2.day() == std::chrono::day{26}); // 2023년 11월의 마지막 일요일은 26일입니다.
        }
        {
            // 2023년 11월의 2번째 일요일
            std::chrono::year_month_weekday date1{std::chrono::year{2023}/std::chrono::month{11}/std::chrono::Sunday[2]}; 
            std::chrono::year_month_day date2{date1};
            EXPECT_TRUE(date2.day() == std::chrono::day{12}); // 2023년 11월의 두번째 일요일은 12일입니다.
        }
        {
            // 2번째 일요일
            std::chrono::weekday_indexed week{std::chrono::Sunday, 2};

            // 2023년 11월의 2번째 일요일
            std::chrono::year_month_weekday date1{std::chrono::year{2023}/std::chrono::month{11}/week}; 
            std::chrono::year_month_day date2{date1};
            EXPECT_TRUE(date2.day() == std::chrono::day{12}); // 2023년 11월의 두번째 일요일은 12일입니다.            
        }

        {
            // 2023년 11월 21일
            std::chrono::year_month_day date1{std::chrono::year{2023}/std::chrono::month{11}/std::chrono::day{21}};
            std::chrono::year_month_weekday date2{date1};
            EXPECT_TRUE(date2.weekday() == std::chrono::Tuesday); // 목요일입니다.
            EXPECT_TRUE(date2.weekday_indexed() == std::chrono::Tuesday[3]); // 3번째 목요일입니다.
        }             

    }

    // now()로부터 달력 날짜 초기화
    {
        std::chrono::time_point now{std::chrono::system_clock::now()}; // now()는 nanoseconds 단위로 timpoint를 구합니다.
        std::chrono::year_month_day date{std::chrono::floor<std::chrono::days>(now)}; // now를 days단위로 구하여 년-월-일 단위로 저장합니다.
    }
    // local_days(), sys_days() 형변환 연산자
    {
        // 2023년 12월 25일
        std::chrono::year_month_day date1{std::chrono::year{2023}/std::chrono::month{12}/std::chrono::day{25}};

        // 2023년 12월 20일
        std::chrono::year_month_day date2{std::chrono::year{2023}/std::chrono::month{12}/std::chrono::day{20}};

        std::chrono::days duration{std::chrono::sys_days(date1) - std::chrono::sys_days(date2)}; // 두 달력 날짜의 차이를 구하기 위해 time_point로 변환합니다.

        std::cout << duration << std::endl; // 5d 
    } 
    // 유효성 검사
    {
        std::chrono::day day29{29};
        EXPECT_TRUE(day29.ok() == true);
        
        std::chrono::year_month_day date1{std::chrono::year{2023}/std::chrono::month{2}/day29};
        EXPECT_TRUE(date1.ok() == false); // 2023/2/30은 유효하지 않습니다. 28일까지 있습니다.

        std::chrono::year_month_day date2{std::chrono::year{2024}/std::chrono::month{2}/day29};
        EXPECT_TRUE(date2.ok() == true); // 2023/2/29는 유효합니다. 29일까지 있습니다.
    }

#endif     
}
TEST(TestMordern, TimeZone) {
#if 202002L <= __cplusplus // C++20~ 
    {
        auto utcTime{std::chrono::system_clock::now()};
        std::cout << "utcTime : " << std::format("{:%Y/%m/%d %H:%M:%S}", utcTime) << std::endl; // 2023/12/17 13:25:45.131857000

        std::chrono::zoned_time newyork{"America/New_York", utcTime};
        std::cout << "localTime : " << std::format("{:%Y/%m/%d %H:%M:%S}", newyork) << std::endl; // 2023/12/17 08:25:45.131857000

        std::chrono::zoned_time shanghai{"Asia/Shanghai", utcTime};
        std::cout << "localTime : " << std::format("{:%Y/%m/%d %H:%M:%S}", shanghai) << std::endl; // 2023/12/17 21:25:45.131857000

        std::chrono::zoned_time seoul{"Asia/Seoul", utcTime};
        std::cout << "localTime : " << std::format("{:%Y/%m/%d %H:%M:%S}", seoul) << std::endl; // 2023/12/17 22:25:45.131857000
    }
#endif    
}