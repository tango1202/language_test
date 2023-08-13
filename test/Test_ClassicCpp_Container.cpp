#include "gtest/gtest.h" 

#include <list>

namespace {

    // Iterator가 가르키는 곳부터 n개를 value로 채웁니다. 
    // Iterator는 ++, *, = 연산자를 구현해야 합니다.
    template<typename Iterator>
    void Fill(Iterator first, size_t n, const int& value) {
        for (size_t i = n; 0 < n; --n, ++first) {
            *first = value;
        }
    }

    template<typename ContainerT>
    class BackInsertIterator {
        ContainerT& m_Container;
    public:
        // 값 생성자에서는 Container를 전달받습니다.
        explicit BackInsertIterator(ContainerT& container) : // #1
            m_Container(container) {}

        // 복사 생성자에서는 컨테이너 참조자를 복사합니다.
        BackInsertIterator(const BackInsertIterator& other) : // #2
            m_Container(other.m_Container) {}
    private:   
        // 대입 연산자는 사용하지 않습니다.
        BackInsertIterator& operator =(const BackInsertIterator& other) {return *this;} // #3

    public:
        // = 에서 컨테이너 값 타입을 전달하면, push_back()을 하여 추가합니다.
        BackInsertIterator& operator =(const typename ContainerT::value_type& value) { // #4
            m_Container.push_back(value);
            return *this;
        }

        // 무조건 끝에 추가하므로 ++는 별다른 동작하지 않습니다.
        BackInsertIterator& operator ++() { // #5
            return *this;
        }

        // * 시 자기 자신을 리턴하여 *first = value;시 #4가 호출되게 합니다.
        BackInsertIterator& operator *() { //# 6
            return *this;
        }
    };
}

TEST(TestClassicCpp, Container) {
    {
        std::vector<int> v;
   
        // 뒤에 추가
        v.push_back(0);
        v.push_back(1);

        // 지정한 위치 앞에 삽입
        std::vector<int>::iterator result = v.insert(
            v.begin() + 1, // 1번째 요소 앞에 삽입
            10
        );
        EXPECT_TRUE(*(v.begin()) == 0 && *(++v.begin()) == 10 && *(++(++v.begin())) == 1);
        EXPECT_TRUE(v[0] == 0 && v[1] == 10 && v[2] == 1); // [] 연산자를 이용하여 value 값 조회 지원
        EXPECT_TRUE(v.size() == 3); // 요소 갯수 3개
        EXPECT_TRUE(*result == 10); // 삽입한 요소를 리턴함

        // 지정 요소 삭제
        result = v.erase(result);
        EXPECT_TRUE(v[0] == 0 && v[2] == 1);
        EXPECT_TRUE(v.size() == 2); // 요소 갯수 2개
        EXPECT_TRUE(*result == 1); // 삭제한 요소의 다음 요소를 리턴함
    }
    {
        std::list<int> l;
        l.push_back(0);
        l.push_back(1);

        // 지정한 위치 앞에 삽입
        std::list<int>::iterator result = l.insert(
            ++l.begin(), // 1번째 요소 앞에 삽입, 임의 접근 불가하여 ++ 사용
            10
        );
        EXPECT_TRUE(*(l.begin()) == 0 && *(++l.begin()) == 10 && *(++(++l.begin())) == 1);
        EXPECT_TRUE(l.size() == 3); // 요소 갯수 3개
        EXPECT_TRUE(*result == 10); // 삽입한 요소를 리턴함
      
        // 지정 요소 삭제
        result = l.erase(result);
        EXPECT_TRUE(*(l.begin()) == 0 && *(++l.begin()) == 1);
        EXPECT_TRUE(l.size() == 2); // 요소 갯수 2개
        EXPECT_TRUE(*result == 1); // 삭제한 요소의 다음 요소를 리턴함
    }
    {
        std::map<int, std::string> m;

        // (X) 컴파일 오류. map은 정렬하는 것이라 뒤에 추가를 제공하지 않음
        // m.push_back(std::make_pair(0, "data1")); 

        // 삽입. 삽입시 key 값으로 정렬됨
        std::pair<std::map<int, std::string>::iterator, bool> result = m.insert(std::make_pair(1, "data1"));
        result = m.insert(std::make_pair(0, "data0")); 
 
        EXPECT_TRUE((*(m.begin())).first == 0 && (*(m.begin())).second == "data0"); // key 값으로 정렬되어 있음
        EXPECT_TRUE((*(++m.begin())).first == 1 && (*(++m.begin())).second == "data1"); 
        EXPECT_TRUE(m[0] == "data0" && m[1] == "data1"); // [] 연산자를 이용하여 value 값 조회 지원

        EXPECT_TRUE(m.size() == 2); // 요소 갯수 2개
        EXPECT_TRUE((*result.first).first == 0 && (*result.first).second == "data0"); // 삽입한 요소를 리턴함
        EXPECT_TRUE(result.second == true); // 삽입 성공

        // 동일 key 삽입
        result = m.insert(std::make_pair(0, "data2")); 
        EXPECT_TRUE((*result.first).first == 0 && (*result.first).second == "data0"); // 기존 동일키 요소를 리턴함
        EXPECT_TRUE(result.second == false); // 삽입 실패

        // 지정 요소 삭제
        std::map<int, std::string>::iterator eraseResult = m.erase(result.first);
        EXPECT_TRUE((*(m.begin())).first == 1 && (*(m.begin())).second == "data1"); 
        EXPECT_TRUE(m.size() == 1); // 요소 갯수 1개
        EXPECT_TRUE((*eraseResult).first == 1); // 삭제한 요소의 다음 요소를 리턴함
    }

    // 이터레이터
    {
        std::vector<int> v(5); // 5개의 요소 생성(클래스면 생성자를 호출함)
        EXPECT_TRUE(v[0] == 0 && v[1] == 0 && v[2] == 0 && v[3] == 0 && v[4] == 0);

        std::vector<int>::iterator itr = v.begin(); // 요소의 시작
        std::vector<int>::iterator endItr = v.end(); // 마지막 요소의 다음
        for (int i = 0; itr != endItr; ++itr, ++i) { // 이터레이터는 복사 부하가 없도록 전위 증가 연산 사용
            *itr = i; // 요소에 값 할당
        }

        itr = v.begin();
        for (int i = 0; itr != endItr; ++itr, ++i) { 
            EXPECT_TRUE(*itr == i);
        }       

        EXPECT_TRUE(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3 && v[4] == 4);
        
        // 랜덤 접근이 가능하다면, 포인터 연산이랑 비슷한 형태로 접근      
        *(v.begin() + 1) = 10; 
        EXPECT_TRUE(v[1] == 10); 
    }
    // 삽입 이터레이터
    {
        std::vector<int> v; 
        // Fill(v.begin(), 5, 7); // v[0] ~ v[4] 에 7 대입. v가 5개 할당되지 않았다면 예외 발생 
        Fill(BackInsertIterator<std::vector<int>>(v), 5, 7); // 현 컨테이너 뒤 5 개에 7 삽입. BackInsertIterator에서 operator = 을 push_back() 으로 구현
        
        EXPECT_TRUE(v[0] == 7 && v[1] == 7 && v[2] == 7 && v[3] == 7 && v[4] == 7);
    }
    // 삽입 이터레이터
    {
        std::vector<int> v; 
        Fill(std::back_inserter(v), 5, 7); // 표준 유틸리티 함수 사용
        
        EXPECT_TRUE(v[0] == 7 && v[1] == 7 && v[2] == 7 && v[3] == 7 && v[4] == 7);
    }
    // 역방향 이터레이터
    {
        std::vector<int> v(5); 
 
        // 순방향
        {
            std::vector<int>::iterator itr = v.begin(); // 요소의 시작
            std::vector<int>::iterator endItr = v.end(); // 요소의 끝
            for (int i = 0; itr != endItr; ++itr, ++i) { 
                *itr = i; 
            }
            EXPECT_TRUE(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3 && v[4] == 4);
        }
        // 역방향
        {
            std::vector<int>::reverse_iterator itr = v.rbegin(); // 요소의 끝
            std::vector<int>::reverse_iterator endItr = v.rend(); // 요소의 시작
            for (int i = 0; itr != endItr; ++itr, ++i) { 
                *itr = i; 
            }
            EXPECT_TRUE(v[0] == 4 && v[1] == 3 && v[2] == 2 && v[3] == 1 && v[4] == 0);
        }
    }
}

