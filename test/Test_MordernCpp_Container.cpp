#include "gtest/gtest.h" 

#include <algorithm>
TEST(TestMordern, Container) {
    // # (C++11~) const_iterator 지원
    {
        std::vector<int> v;
        v.push_back(1);
        v.push_back(2);

        std::vector<int>::const_iterator const_itr = std::find(v.cbegin(), v.cend(), 1); // v를 수정하지 않으므로 const_iterator로 받습니다.
        v.insert(const_itr, 0); // const_itr 앞에 0을 삽입합니다.

        EXPECT_TRUE(v[0] == 0 && v[1] == 1 && v[2] == 2);
    }
    // # (C++11~) 컨테이너의 initializer_list 초기화
    {
        std::vector<int> v;
        v.push_back(1); // 이전에는 push_back으로 데이터를 삽입했습니다.
        v.push_back(2);
        v.push_back(3);
    }
    {
        std::vector<int> v{1, 2, 3}; // C++11. 중괄호 초기화로 초기값을 넣을 수 있습니다.        
    }
    {
        std::vector<int> v1(2); // 요소 갯수가 2개인 vector 생성
        EXPECT_TRUE(v1.size() == 2 && v1[0] == 0 && v1[1] == 0);

        std::vector<int> v2{2}; // 요소값이 2인 vector 생성
        EXPECT_TRUE(v2.size() == 1 && v2[0] == 2);          
    }
    // # (C++11~) emplace(), emplace_back(), emplace_front(), emplace_hint() 삽입
    {
        class A {
            int m_X;
            int m_Y;
        public:
            A(int x, int y) {std::cout << "A::Value Constructor" << std::endl;}
            A(const A& other) {std::cout << "A::Copy Constructor" << std::endl;}
            A(A&& other) noexcept {std::cout << "A::Move Constructor" << std::endl;}

            A& operator =(const A& other) = delete;
            A& operator =(A&& other) noexcept = delete;
        };

        std::vector<A> v;
        v.reserve(10); // push_back이나 emplace_back 중 재할당이 없게끔 공간을 널널하게 만듭니다.

        A a{1, 2}; 
        v.push_back(a); // (△) 비권장. 복제본을 생성하면서 좌측값이므로 복사 생성 1회.
        v.push_back(A{1, 2}); // (△) 비권장. 복제본을 생성하면서 임시개체인 우측값이므로 이동 생성 1회.
        v.push_back({1, 2}); // (△) 비권장. 중괄호 복사 초기화로 v.push_back(A{1, 2}); 과 동일

        v.emplace_back(1, 2); // (O) A개체 생성을 위한 데이터를 전달합니다. 개체 생성 1회
        // v.emplace_back({1, 2}); // (X) 컴파일 오류. A는 {1, 2} 를 전달받는 생성자가 없습니다.
    }

}

namespace HeterogeneousLookup_1 {
    class Data {
    public:
        std::string m_Name;
        int m_Val;
        bool operator <(const Data& other) const {
            if (m_Val < other.m_Val) return true;
            if (other.m_Val < m_Val) return false;

            return m_Name < other.m_Name;
        }
    };
    // 이종 타입에 대해 비교할 수 있게 정적 비교 함수를 제공합니다.
    bool operator <(const Data& left, const char* right) {
        return left.m_Name < right;
    }
    bool operator <(const char* left, const Data& right) {
        return left < right.m_Name;
    }
}

TEST(TestMordern, HeterogeneousLookup) {

    // ~C++14 연관 컨테이너의 이종 탐색
    {
        using namespace HeterogeneousLookup_1;
        std::set<Data> dataSet{
            Data{"Lee", 10},
            Data{"Park", 20}
        };
        auto result{
            dataSet.find(
                Data{"Lee", 10} // 같은 타입만 탐색이 가능하므로 "Lee"를 탐색하고 싶어도 Data 개체를 동일하게 만들어 탐색해야 합니다.
            )
        }; 

        EXPECT_TRUE((*result).m_Name == "Lee" && (*result).m_Val == 10);
    }
 #if 201402L <= __cplusplus // C++14~       
    // 연관 컨테이너의 이종 탐색
    {
        using namespace HeterogeneousLookup_1;
        // find() 에서 키 타입외에 다른 것을 사용하려면 std::less<>를 사용합니다.
        std::set<Data, std::less<>> dataSet{
            Data{"Lee", 10},
            Data{"Park", 20}
        };
        auto result{
            dataSet.find(
                "Lee" // find() 함수에 탐색하고 싶은 이종 타입의 개체를 전달합니다.
            )
        }; 

        EXPECT_TRUE((*result).m_Name == "Lee" && (*result).m_Val == 10);
    }
#endif    
}