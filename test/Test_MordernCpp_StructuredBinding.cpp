#include "gtest/gtest.h" 

TEST(TestMordern, StructuredBinding) {
    // C++17~ 구조화된 바인딩 - 배열
    {
        int arr[]{1, 2, 3};
        
        auto [a, b, c]{arr}; // auto [a, b, c] = arr;
        EXPECT_TRUE(a == 1 && b == 2 && c == 3); // arr을 임시 개체에 복제하고, 복제본에 a = &temp[0], b = &temp[1], c = &temp[2] 바인딩

        auto& [x, y, z]{arr};
        EXPECT_TRUE(x == 1 && y == 2 && z == 3); // x = &a[0], y = &a[1], z = &a[2] 에 바인딩

        arr[0] = 10; // 원본 수정
        arr[1] = 20;
        arr[2] = 30;

        EXPECT_TRUE(a == 1 && b == 2 && c == 3); // 복제본이므로 원본 내용이 반영되지 않습니다.
        EXPECT_TRUE(x == 10 && y == 20 && z == 30); // 참조본이므로 원본 수정시 값이 동기화 됩니다.
    }
    // C++17~ 구조화된 바인딩 - tuple
    {
        auto data{std::make_tuple(10, "John")};
       
        auto [id, name]{data};
        EXPECT_TRUE(id == 10 && name == "John");

        auto& [idRef, nameRef]{data};
        EXPECT_TRUE(idRef == 10 && nameRef == "John");

        std::get<0>(data) = 20;

        EXPECT_TRUE(id == 10 && name == "John");
        EXPECT_TRUE(idRef == 20 && nameRef == "John");
    }

     // C++17~ 구조화된 바인딩 - 클래스
    {
        class T {
        public:
            int m_Id{10};
            std::string m_Name{"John"};    
        };
        T data;

        auto [id, name]{data};
        EXPECT_TRUE(id == 10 && name == "John");

        auto& [idRef, nameRef]{data};
        EXPECT_TRUE(idRef == 10 && nameRef == "John");

        data.m_Id = 20;

        EXPECT_TRUE(id == 10 && name == "John");
        EXPECT_TRUE(idRef == 20 && nameRef == "John");
    }  
    // / C++17~ 구조화된 바인딩 - 컨테이너 활용 
    {
        // 고전 C++ 스타일

        // 초기 데이터 입력
        std::map<int, std::string> m;
        m.insert(std::make_pair(0, "data0"));
        m.insert(std::make_pair(1, "data1"));

        // 요소 이터레이팅
        std::map<int, std::string>::iterator itr = m.begin();
        std::map<int, std::string>::iterator endItr = m.end();
        for (; itr != endItr; ++itr) {
            std::cout << (*itr).first << (*itr).second << std::endl; // pair여서 first, second로 접근
        }

        // 삽입 실패 검사
        std::pair<std::map<int, std::string>::iterator, bool> result = m.insert(std::make_pair(0, "data0"));
        if (result.second != true) {
            std::cout << "insert fail" << std::endl;
        }
    }
    {
        // C++11 스타일

        // 초기 데이터 입력 - 중괄호 초기화로 단순화 됐습니다.
        std::map<int, std::string> m_11{
            {0, "data0"}, 
            {1, "data1"}
        }; 

        // 요소 이터레이팅 - 범위 기반 for로 단순화 됐습니다.
        for (const auto& item : m_11) {
            std::cout << item.first << item.second << std::endl; // pair여서 first, second로 접근
        }

        // 삽입 실패 검사 - auto 사용으로 단순화 됐습니다.
        auto result{m_11.insert(std::make_pair(0, "data0"))};
        if (result.second != true) {
            std::cout << "insert fail" << std::endl;
        }        
    }
    {
        // C++17 스타일

        // 초기 데이터 입력 - 중괄호 초기화로 단순화 됐습니다.
        std::map<int, std::string> m_17{
            {0, "data0"}, 
            {1, "data1"}
        }; 

        // 요소 이터레이팅 - 범위 기반 for와 구조화된 바인딩으로 단순화 됐습니다.
        for (const auto& [key, value]: m_17) {
            std::cout << key << value << std::endl; 
        }

        // 삽입 실패 검사 -  초기식을 포함하는 if()와 구조화된 바인딩으로 단순화 됐습니다.
        if (auto [itr, result]{m_17.insert(std::make_pair(0, "data0"))}; result != true) {
            std::cout << "insert fail" << std::endl;
        } 
    }
 
}