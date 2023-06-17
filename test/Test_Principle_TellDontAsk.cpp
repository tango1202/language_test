#include "gtest/gtest.h" 

namespace {
    class Rectangle {
    private:
        int m_CenterX; // 중심점을 멤버로 사용합니다.
        int m_CenterY;
        int m_Width;
        int m_Height;
    public:
        Rectangle(int l, int t, int w, int h) : 
            m_CenterX(l + w / 2), 
            m_CenterY(t + h / 2),
            m_Width(w), 
            m_Height(h) {}
    public:
        std::pair<int, int> GetCenter() const { 
            return std::make_pair(m_CenterX, m_CenterY);
        }
    };  
}

TEST(TestPrinciple, TellDontAsk) {
    Rectangle rect(0, 0, 10, 20);
    std::pair<int, int> center = rect.GetCenter();
}