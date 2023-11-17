#include "gtest/gtest.h" 

TEST(TestMordern, StructuredBinding) {
#if 201703L <= __cplusplus // C++17~    
    // C++17~ 구조화된 바인딩 - 배열
    {
        int arr[]{1, 2, 3};
        
        auto [a_17, b_17, c_17]{arr}; // auto [a_17, b_17, c_17] = arr; 과 동일
        EXPECT_TRUE(a_17 == 1 && b_17 == 2 && c_17 == 3); // arr을 임시 개체에 복제하고, 복제본에 a_17 = &temp[0], b_17 = &temp[1], c_17 = &temp[2] 바인딩

        auto& [x_17, y_17, z_17]{arr};
        EXPECT_TRUE(x_17 == 1 && y_17 == 2 && z_17 == 3); // x_17 = &a[0], y_17 = &a[1], z_17 = &a[2] 에 바인딩

        arr[0] = 10; // 원본 수정
        arr[1] = 20;
        arr[2] = 30;

        EXPECT_TRUE(a_17 == 1 && b_17 == 2 && c_17 == 3); // 복제본이므로 원본 내용이 반영되지 않습니다.
        EXPECT_TRUE(x_17 == 10 && y_17 == 20 && z_17 == 30); // 참조본이므로 원본 수정시 값이 동기화 됩니다.
    }
    // C++17~ 구조화된 바인딩 - tuple
    {
        auto data_11{std::make_tuple(10, "John")};
        auto [id_17, name_17]{data_11}; // 복제본에 바인딩
        EXPECT_TRUE(id_17 == 10 && name_17 == "John");

        auto& [idRef_17, nameRef_17]{data_11}; // 원본에 바인딩
        EXPECT_TRUE(idRef_17 == 10 && nameRef_17 == "John");

        std::get<0>(data_11) = 20;

        EXPECT_TRUE(id_17 == 10 && name_17 == "John");
        EXPECT_TRUE(idRef_17 == 20 && nameRef_17 == "John");



    }

     // C++17~ 구조화된 바인딩 - 클래스
    {
        class T {
        public:
            int m_Id_11{10}; // C++11의 멤버 선언부 초기화
            std::string m_Name_11{"John"};    
        };

        {
            T data_11;

            auto [id_17, name_17]{data_11}; // 복제본에 바인딩
            EXPECT_TRUE(id_17 == 10 && name_17 == "John");

            auto& [idRef_17, nameRef_17]{data_11}; // 원본에 바인딩
            EXPECT_TRUE(idRef_17 == 10 && nameRef_17 == "John");

            data_11.m_Id_11 = 20;

            EXPECT_TRUE(id_17 == 10 && name_17 == "John");
            EXPECT_TRUE(idRef_17 == 20 && nameRef_17 == "John");
        }
        {
            // 임시 개체의 수명 연장
            const auto& [id_17, name_17]{
                T{30, "Sam"}  // 임시 개체를 생성했지만 const auto&로 받아 수명이 연장됩니다.
            };
            EXPECT_TRUE(id_17 == 30 && name_17 == "Sam");
        }
    }  
#endif    
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
#if 201703L <= __cplusplus // C++17~    
    {
        // C++17 스타일

        // 초기 데이터 입력 - 중괄호 초기화로 단순화 됐습니다.
        std::map<int, std::string> m_11{
            {0, "data0"}, 
            {1, "data1"}
        }; 

        // 요소 이터레이팅 - 범위 기반 for와 구조화된 바인딩으로 단순화 됐습니다.
        for (const auto& [key_17, value_17]: m_11) {
            std::cout << key_17 << value_17 << std::endl; 
        }

        // 삽입 실패 검사 -  초기식을 포함하는 if()와 구조화된 바인딩으로 단순화 됐습니다.
        if (auto [itr_17, result_17]{m_11.insert(std::make_pair(0, "data0"))}; result_17 != true) {
            std::cout << "insert fail" << std::endl;
        } 
    }
#endif    
 
}