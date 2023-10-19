#include "gtest/gtest.h" 

#include <unordered_map>
TEST(TestMordern, UnorderedMap) {
    {
        std::unordered_map<int, std::string> m;

        // 예약된 요소 수에 맞게 버킷은 자동 조정 됩니다.
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 1개
        m.reserve(2);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 2개
        m.reserve(3);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 3개
        m.reserve(4);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 5개
        m.reserve(5);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 5개
        m.reserve(6);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 7개
        m.reserve(7);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 7개
        m.reserve(8);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 11개
        m.reserve(9);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 11개
        m.reserve(100);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 103개
        m.reserve(1000);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 1031개
        m.reserve(10000);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 10273개        
        m.reserve(100000);
        std::cout << "bucket count : " << m.bucket_count() << std::endl; // 107897개  
    }
    {
        std::unordered_map<int, std::string> m;
        
        // 기본적인 사용 방식은 map과 동일합니다.
        m.insert(std::make_pair(0, "data0"));
        m.insert(std::make_pair(1, "data1"));
        m[0] = "changed data";
        EXPECT_TRUE(m[0] == "changed data");
    }   
    // unordered_map의 Key
    {
        class A {
        private:
            int m_Val;
            std::string m_Str;
        public:
            A(int val, const char* str) : 
                m_Val{val}, 
                m_Str{str} {}
            A(const A& other) = default; // 복사 생성자 필요
            A& operator =(const A& other) = delete; // 복사 대입 연산자 불필요
        public:
            // == 비교 연산자 필요
            bool operator ==(const A& other) const {
                if (m_Val != other.m_Val) return false;

                return m_Str == other.m_Str;
            }

            // < 비교 연산자 불필요
            // bool operator <(const A& other) const {
            //     if (m_Val < other.m_Val) return true;
            //     if (other.m_Val < m_Val) return false;

            //     return m_Str < other.m_Str;
            // }

            int GetVal() const {return m_Val;}
            const std::string& GetStr() const {return m_Str;}
        }; 

        // 해시 함수. 반드시 const 함수이어야 합니다.
        struct AHash {
            std::size_t operator ()(const A& obj) const {
                size_t h1{std::hash<int>{}(obj.GetVal())};
                size_t h2{std::hash<std::string>{}(obj.GetStr())};

                return h1 ^ (h2 << 1);
            }
        }; 

        // A 개체의 해시 함수를 전달합니다.
        std::unordered_map<A, std::string, AHash> m;

        // insert시 내부적으로 key를 비교 연산하여 정렬하여 삽입합니다. 
        m.insert(std::make_pair(A{0, "a"}, "data0")); 
        m.insert(std::make_pair(A{1, "b"}, "data1"));
    }

}