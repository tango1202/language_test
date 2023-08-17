#include "gtest/gtest.h" 

#include <list>
#include <algorithm>

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

    // 복사 생성자
    {
        class A {
        public:
            int m_Val;
        public:
            explicit A(int val) : m_Val(val) {}
            // 복사 생성자를 구현해야 합니다.
            A(const A& other) : m_Val(other.m_Val) {}
        private:
            // 대입 연산자를 사용하지 못하도록 private로 정의합니다.
            A& operator =(const A& other) {
                m_Val = other.m_Val;
                return *this;
            }
        };

        std::vector<A> v;
        v.push_back(A(0));
        v.push_back(A(1));
    }
    // sort 등 알고리즘 사용시
    {
        class A {
        public:
            int m_Val;
        public:
            explicit A(int val) : m_Val(val) {}
            // 복사 생성자
            A(const A& other) : m_Val(other.m_Val) {}
            // 대입 연산자
            A& operator =(const A& other) {
                m_Val = other.m_Val;
                return *this;
            }
            // 비교 연산자
            bool operator <(const A& other) const {
                return m_Val < other.m_Val;
            }
        };

        std::vector<A> v;
        v.push_back(A(1));
        v.push_back(A(0));

        // 정렬합니다. 
        // 내부적으로 요소의 대입 연산자와 대소 비교 연산자를 사용합니다.
        std::sort(v.begin(), v.end()); 
        EXPECT_TRUE(v[0].m_Val == 0 && v[1].m_Val == 1);
    }
    // 연관 컨테이너의 key
    {
        class A {
        public:
            int m_Val;
        public:
            explicit A(int val) : m_Val(val) {}
            // 복사 생성자
            A(const A& other) : m_Val(other.m_Val) {}
        private:    
            // 대입 연산자를 사용하지 못하도록 private로 정의합니다.
            A& operator =(const A& other) {
                m_Val = other.m_Val;
                return *this;
            }
        public:
            // 비교 연산자
            bool operator <(const A& other) const {
                return m_Val < other.m_Val;
            }
        };        

        std::map<A, std::string> m;

        // insert시 내부적으로 key를 비교 연산하여 정렬하여 삽입합니다. 
        m.insert(std::make_pair(A(0), "data0"));
        m.insert(std::make_pair(A(1), "data1"));
    }
    // ----
    // vector 의 삽입과 삭제
    // ----
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
    // ----
    // list 의 삽입과 삭제
    // ----
    {
        std::list<int> l;
        l.push_back(0);
        l.push_back(1);

        // 지정한 위치 앞에 삽입
        std::list<int>::iterator result = l.insert(
            ++l.begin(), // 1번째 요소 앞에 삽입, 랜덤 접근 불가하여 ++ 사용
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
    // ----
    // map 의 삽입과 삭제
    // ----
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

    // ----
    // 이터레이터를 이용한 요소 접근
    // ----
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
    // ----
    // 역방향 이터레이터
    // ----
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
    // ----
    // 삽입 이터레이터
    // ----
    {
        std::vector<int> v; 
        // Fill(v.begin(), 5, 7); // v[0] ~ v[4] 에 7 대입. v가 5개 할당되지 않았다면 예외 발생 
        Fill(BackInsertIterator<std::vector<int>>(v), 5, 7); // 현 컨테이너 뒤 5 개에 7 삽입. BackInsertIterator에서 operator = 을 push_back() 으로 구현
        
        EXPECT_TRUE(v[0] == 7 && v[1] == 7 && v[2] == 7 && v[3] == 7 && v[4] == 7);
    }
    {
        std::vector<int> v; 
        Fill(std::back_inserter(v), 5, 7); // 표준 유틸리티 함수 사용
        
        EXPECT_TRUE(v[0] == 7 && v[1] == 7 && v[2] == 7 && v[3] == 7 && v[4] == 7);
    }
    // ----
    // 배열과 vector
    // ----
    {
        std::vector<int> v;

        v.push_back(10); // 요소 생성
        v.push_back(20);
        EXPECT_TRUE(v[0] == 10);
        EXPECT_TRUE(v[1] == 20);

        EXPECT_TRUE(*(&v[0]) == 10);
        EXPECT_TRUE(*((&v[0]) + 1) == 20); // 연속된 메모리여서 포인터 연산으로도 접근 가능합니다.
    }
    // ----
    // size와 capacity
    // ----
    {
        std::vector<int> v(100);
        size_t old = v.capacity();
        v.clear();
        EXPECT_TRUE(v.capacity() == old); // clear해서 요소를 모두 지웠지만, capacity는 그대로 입니다.
    }
    {
        std::vector<int> v;

        for (int i = 0; i < 16; ++i) {
            v.push_back(0);
            std::cout<<"size:"<<v.size()<<" capacity:"<<v.capacity()<<std::endl;
        }
    }
    // ----
    // 저장 공간 예약 
    // ----
    {
        std::vector<int> v;   
        v.reserve(100);
        EXPECT_TRUE(v.capacity() == 100); // 100개를 저장할 수 있는 메모리 공간이 확보됨
        // v[0] = 0; // (X) 컴파일 오류. 아직 요소는 아직 생성된게 아니기에 접근할 수 없습니다.
        v.push_back(0); // (0) 
    }
    // ----
    // clear와 swap
    // ----
    {
        std::vector<int> v(100);
        size_t old = v.capacity();
        v.clear();
        EXPECT_TRUE(v.capacity() == old); // clear를 했지만 메모리 용량은 그대로 입니다.
    }
    {
        std::vector<int> v(100);
        EXPECT_TRUE(v.capacity() == 100);
        std::vector<int> temp;
        v.swap(temp); // vector는 pImpl로 구현되어 swap시 복사 부하가 없습니다.
        EXPECT_TRUE(v.capacity() == 0); // 크기가 0인 vector와 바꿔치기 했습니다.
    }
    // ----
    // vector<bool>
    // ----
    {
        std::vector<bool> v; // 내부적으로는 bool 타입이 아니라 비트 필드를 사용합니다.
        v.push_back(true);
        v.push_back(false);

        EXPECT_TRUE(v[0] == true && v[1] == false);
        //EXPECT_TRUE(*(&v[0]) == true); // (X) 컴파일 오류. &v[0]와 같은 포인터 연산과 호환되지 않습니다.
   }

}

