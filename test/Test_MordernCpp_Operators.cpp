#include "gtest/gtest.h" 

namespace ThreeWayComparison_1 {
 
    class T {
        int m_Val;
    public:
        explicit T(int val) : m_Val{val} {} 

        // T op T 의 형태로 비교
        bool operator ==(const T& other) const {return m_Val == other.m_Val;} // < 로부터 구현하면, !(*this < other || other < *this)로 할 수 있습니다. 단 < 을 2회 하므로 비효율적입니다.
        bool operator !=(const T& other) const {return !(*this == other);} // == 로부터 구현
        bool operator <(const T& other) const {return m_Val < other.m_Val;}
        bool operator >(const T& other) const {return other < *this;} // < 로부터 구현
        bool operator <=(const T& other) const {return !(other < *this);} // < 로부터 구현
        bool operator >=(const T& other) const {return !(*this < other);} // < 로부터 구현

        // T op int 의 형태로 비교
        bool operator ==(int val) const {return m_Val == val;} 
        bool operator !=(int val) const {return !(m_Val == val);} 
        bool operator <(int val) const {return m_Val < val;}
        bool operator >(int val) const {return val < m_Val;} 
        bool operator <=(int val) const {return !(val < m_Val);} 
        bool operator >=(int val) const {return !(m_Val < val);} 
    }; 
    // int op T의 형태로 비교
    bool operator ==(int left, const T& right) {return T{left} == right;}
    bool operator !=(int left, const T& right) {return T{left} != right;}
    bool operator <(int left, const T& right) {return T{left} < right;}
    bool operator >(int left, const T& right) {return T{left} > right;}
    bool operator <=(int left, const T& right) {return T{left} <= right;}
    bool operator >=(int left, const T& right) {return T{left} >= right;}
}
TEST(TestMordern, 3WayComparison) {

    {
        class T {
            int m_Val;
        public:
            explicit T(int val) : m_Val{val} {} 
            bool operator ==(const T& other) const {return m_Val == other.m_Val;} // < 로부터 구현하면, !(*this < other || other < *this)로 할 수 있습니다. 단 < 을 2회 하므로 비효율적입니다.
            bool operator !=(const T& other) const {return !(*this == other);} // == 로부터 구현
            bool operator <(const T& other) const {return m_Val < other.m_Val;}
            bool operator >(const T& other) const {return other < *this;} // < 로부터 구현
            bool operator <=(const T& other) const {return !(other < *this);} // < 로부터 구현
            bool operator >=(const T& other) const {return !(*this < other);} // < 로부터 구현
        }; 

        EXPECT_TRUE(T{10} == T{10});  
        EXPECT_TRUE(T{10} != T{20}); 
        EXPECT_TRUE(T{10} < T{20}); 
        EXPECT_TRUE(T{20} > T{10}); 
        EXPECT_TRUE(T{10} <= T{20} && T{10} <= T{10}); 
        EXPECT_TRUE(T{20} >= T{10} && T{10} >= T{10}); 
    }
    // 상등 비교, 동등 비교
    {
        class Area {
            int m_Width;
            int m_Height;
        public:
            Area(int width, int height) : m_Width{width}, m_Height{height} {}
            int GetData() const {return m_Width * m_Height;}

            // 상등 비교. 가로와 세로가 같은지 검사합니다.
            bool IsEqual(const Area& other) const {
                if (m_Width != other.m_Width) return false;
                return m_Height == other.m_Height;
            }

            // 동등 비교 : 개념적으로 같은지, 즉 면적이 같은지 검사합니다.
            bool IsEquivalence(const Area& other) const {
                return GetData() == other.GetData();
            }
        };
        Area a{2, 3};
        Area b{3, 2};
        EXPECT_TRUE(a.IsEqual(b) == false); // 2 X 3 과 3 X 2 는 상등하지 않습니다. 
        EXPECT_TRUE(a.IsEquivalence(b) == true); // 2 X 3 과 3 X 2 는 동등합니다. 
    }
#if 202002L <= __cplusplus // C++20~ 
    {
        class T_20 {
            int m_Val;
        public:
            explicit T_20(int val) : m_Val{val} {}
            
            std::strong_ordering operator <=>(const T_20& other) const {return m_Val <=> other.m_Val;}
            bool operator ==(const T_20& other) const {return m_Val == other.m_Val;}
        };  

        EXPECT_TRUE(T_20{10} == T_20{10});  
        EXPECT_TRUE(T_20{10} != T_20{20}); 
        EXPECT_TRUE(T_20{10} < T_20{20}); 
        EXPECT_TRUE(T_20{20} > T_20{10}); 
        EXPECT_TRUE(T_20{10} <= T_20{20} && T_20{10} <= T_20{10}); 
        EXPECT_TRUE(T_20{20} >= T_20{10} && T_20{10} >= T_20{10}); 
    }

    // 3중 비교 연산자 비교
    {
        class T_20 {
            int m_Val;
        public:
            explicit T_20(int val) : m_Val{val} {}
            
            std::strong_ordering operator <=>(const T_20& other) const {return m_Val <=> other.m_Val;}
            bool operator ==(const T_20& other) const {return m_Val == other.m_Val;}
        };  

        EXPECT_TRUE((T_20{10} <=> T_20{10}) == 0);  // left == right
        EXPECT_TRUE((T_20{10} <=> T_20{20}) != 0); // left != right
        EXPECT_TRUE((T_20{10} <=> T_20{20}) < 0); // left < right
        EXPECT_TRUE((T_20{20} <=> T_20{10}) > 0);  // left > right
        EXPECT_TRUE((T_20{10} <=> T_20{20}) == 0 || (T_20{10} <=> T_20{10}) <= 0); // left <= right
        EXPECT_TRUE((T_20{20} <=> T_20{10}) == 0 || (T_20{10} <=> T_20{10}) >= 0); // left >= right
    }
    {
        using namespace ThreeWayComparison_1;
        EXPECT_TRUE(T{10} < T{20}); // T op T
        EXPECT_TRUE(T{10} < 20); // T op int  
        EXPECT_TRUE(10 < T{20}); // int op T
    }
    {
        class T_20 {
            int m_Val;
        public:
            explicit T_20(int val) : m_Val{val} {}
            
            // T_20 op T_20
            std::strong_ordering operator <=>(const T_20& other) const {return m_Val <=> other.m_Val;}
            bool operator ==(const T_20& other) const {return m_Val == other.m_Val;}

            // T_20 op int 또는 int op T_20
            std::strong_ordering operator <=>(int val) const {return m_Val <=> val;}
            bool operator ==(int val) const {return m_Val == val;}
        };  

        EXPECT_TRUE((T_20{10} <=> T_20{20}) < 0); // T_20 op T_20
        EXPECT_TRUE((T_20{10} <=> 20) < 0); // T_20 op int  
        EXPECT_TRUE((10 <=> T_20{20}) < 0); // int op T_20. (T_20 <=> 10) > 0으로 변경합니다.

        EXPECT_TRUE(T_20{10} < T_20{20}); // T_20 op T_20
        EXPECT_TRUE(T_20{10} < 20); // T_20 op int  
        EXPECT_TRUE(10 < T_20{20}); // int op T_20. T_20 > 10으로 변경합니다.     
    }
    // 암시적 형변환
    {
        class T_20 {
            int m_Val;
        public:
            T_20(int val) : m_Val{val} {} // (△) 비권장. explicit 가 없습니다. 암시적 형변환 됩니다.
            
            // T_20 op T_20
            std::strong_ordering operator <=>(const T_20& other) const {return m_Val <=> other.m_Val;}
            bool operator ==(const T_20& other) const {return m_Val == other.m_Val;}
        };  

        EXPECT_TRUE((T_20{10} <=> T_20{20}) < 0); // T_20 op T_20
        EXPECT_TRUE((T_20{10} <=> 20) < 0); // int를 암시적으로 T_20으로 형변환  
        EXPECT_TRUE((10 <=> T_20{20}) < 0); // int op T_20. (T_20 <=> 10) > 0으로 변경후 int를 암시적으로 T_20으로 형변환

        EXPECT_TRUE(T_20{10} < T_20{20}); // T_20 op T_20
        EXPECT_TRUE(T_20{10} < 20); // int를 암시적으로 형변환.
        EXPECT_TRUE(10 < T_20{20}); // int op T_20. T_20 > 10으로 변경후 int를 암시적으로 T_20으로 형변환
    }
    // 3중 비교 연산자 default 정의
    {
        class T_20 {
            int m_Val;
        public:
            explicit T_20(int val) : m_Val{val} {}
            
            auto operator <=>(const T_20& other) const = default; 
            // bool operator ==(const T_20& other) const {return m_Val == other.m_Val;} // default 로 정의하면, ==를 정의할 필요가 없습니다.
        };  

        EXPECT_TRUE(T_20{10} == T_20{10});  
        EXPECT_TRUE(T_20{10} != T_20{20}); 
        EXPECT_TRUE(T_20{10} < T_20{20}); 
        EXPECT_TRUE(T_20{20} > T_20{10}); 
        EXPECT_TRUE(T_20{10} <= T_20{20} && T_20{10} <= T_20{10}); 
        EXPECT_TRUE(T_20{20} >= T_20{10} && T_20{10} >= T_20{10});
    }

    {
        class T_20 {
            int m_Val;
        public:
            explicit T_20(int val) : m_Val{val} {}
            
            auto operator <=>(const T_20& other) const = default; 
            bool operator <(const T_20& other) const { // 사용자 정의 버전입니다.
                std::cout << "T_20::operator <" << std::endl;
                return m_Val < other.m_Val;
            } 
            bool operator ==(const T_20& other) const { // 사용자 정의 버전입니다.
                std::cout << "T_20::operator ==" << std::endl;
                return m_Val == other.m_Val;
            }             
        };  

        EXPECT_TRUE(T_20{10} < T_20{20}); // < 을 사용합니다. 
        EXPECT_TRUE(T_20{20} > T_20{10}); // > 이 없으므로 <=> 을 사용합니다.
        EXPECT_TRUE((T_20{10} <=> T_20{20}) < 0); //<=> 을 사용합니다.

        EXPECT_TRUE(T_20{10} == T_20{10}); // == 을 사용합니다.
        EXPECT_TRUE(T_20{10} != T_20{20}); // !=이 없으나 사용자 정의한 ==이 있으므로 == 을 사용합니다. 
        EXPECT_TRUE((T_20{10} <=> T_20{10}) == 0); //<=> 을 사용합니다.
    }
    // 비교 카테고리와 3중 비교 연산자의 리턴 타입
    {
        class Strong_20 {
            int m_Val;
        public:
            explicit Strong_20(int val) : m_Val{val} {}
            std::strong_ordering operator <=>(const Strong_20& other) const = default; 
        };
        class Weak_20 {
            int m_Val;
        public:
            explicit Weak_20(int val) : m_Val{val} {}
            std::weak_ordering operator <=>(const Weak_20& other) const = default; 
        };
        class Partial_20 {
            int m_Val;
        public:
            explicit Partial_20(int val) : m_Val{val} {}
            std::partial_ordering operator <=>(const Partial_20& other) const = default; 
        };

        class Mix_20 {
            Strong_20 m_Strong; // strong_ordering
            Weak_20 m_Weak; // weak_ordering
            Partial_20 m_Partial; // partial_ordering

        public:
            explicit Mix_20(int strong, int weak, int partial) : m_Strong{strong}, m_Weak(weak), m_Partial(partial) {}
            auto operator <=>(const Mix_20& other) const = default; // partial ordering
        };

        std::partial_ordering result{Mix_20{0, 0, 0} <=> Mix_20{1, 1, 1}}; // partial_ordering을 리턴합니다.
    }


#endif
}
