#include "gtest/gtest.h" 

#include <regex>
TEST(TestMordern, RegularExpressions) {

    // regex_match
    {
        std::vector<std::string> v{"abcd", "ab,cd", "ab:cd", "ab cd"}; 
        std::vector<std::string> result;

        // ab 다음에 `,` 또는 `:` 가 있는 문자열 검색하되, 모든 문자(.)가 0부터 무한대로 반복(*)됨.
        std::regex regex{"ab[,:].*"}; 

        for(auto str : v) {
            if (std::regex_match(str, regex)) {
                result.push_back(str);
            }    
        }

        EXPECT_TRUE(result.size() == 2 && result[0] == "ab,cd" && result[1] == "ab:cd");
    }

    // regex_search
    {
        std::string str{
            R"(Tango's phone number is 010-1234-5678. 
His home phone number is 02-123-4567.)"
        }; 
        std::vector<std::string> result;

        // 010- 또는 02-로 시작하고, 숫자가 3~4개 반복, -, 숫자가 4개 반복
        std::regex regex{R"((010|02)-[0-9]{3,4}-[0-9]{4})"}; // 특수 기호가 있으므로, Raw String 리터럴로 표기하는게 좋습니다.
        std::smatch matchResult;

        while(std::regex_search(str, matchResult, regex)) {
            result.push_back(matchResult.str()); 

            str = matchResult.suffix(); // matchResult 다음 위치의 문자열입니다. str에 대입하여 다음번 매칭을 찾습니다.
        }

        EXPECT_TRUE(result.size() == 2 && result[0] == "010-1234-5678" && result[1] == "02-123-4567");
    }    
    // sregex_iterator
    {
        std::string str{
            R"(Tango's phone number is 010-1234-5678. 
His home phone number is 02-123-4567.)"
        }; 
        std::vector<std::string> result;

        std::regex regex{R"((010|02)-[0-9]{3,4}-[0-9]{4})"};

        // 문자열 전체의 regex_search() 결과를 이터레이터로 리턴합니다. 
        auto itr{std::sregex_iterator(str.begin(), str.end(), regex)};
        auto endItr{std::sregex_iterator()};

        for (; itr != endItr; ++itr) {
            result.push_back(itr->str());
        }

        EXPECT_TRUE(result.size() == 2 && result[0] == "010-1234-5678" && result[1] == "02-123-4567");
    }

    // regex_replace
    {
        std::string str{
            R"(Tango's phone number is 010-1234-5678. 
His home phone number is 02-123-4567.)"
        }; 

        std::regex regex{R"((010|02)-[0-9]{3,4}-[0-9]{4})"};

        // 매칭된 결과를 수정합니다.
        std::string result{std::regex_replace(str, regex, "xxx-xxxx-xxxx")};

        EXPECT_TRUE(result == R"(Tango's phone number is xxx-xxxx-xxxx. 
His home phone number is xxx-xxxx-xxxx.)");
    }   
    {
        std::string str{R"(file_2020_01_01.txt, file_2020_01_02.txt)"}; 

        // ([0-9]{4}_[0-9]{2}_[0-9]{2})을 캡쳐합니다.
        std::regex regex{R"(file_([0-9]{4}_[0-9]{2}_[0-9]{2})[.]txt)"};

        // 매칭된 결과에서 캡쳐된 내용인 $1을 출력합니다.
        std::string result{std::regex_replace(str, regex, "$1_file.txt")};

        EXPECT_TRUE(result == R"(2020_01_01_file.txt, 2020_01_02_file.txt)");        
    } 

}