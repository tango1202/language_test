#include "gtest/gtest.h" 

namespace {
    class Degree {};
    class View {
    public:
        float GetCenterX() const { return 0.F; }
        float GetCenterY() const { return 0.F; }
    };

    class Rectangle {
    private:
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;
    public:
        // 생성시에 필요한 값을 모두 전달해야 합니다.
        Rectangle(int l, int t, int w, int h) : 
            m_Left(l), m_Top(t), m_Width(w), m_Height(h) {}
    
        // 함수 호출시에 필요한 인수를 모두 전달해야 합니다.
        void RotateAt(const Degree& delta, float x, float y) {}
    };    
}

TEST(TestPrinciple, ExpicitDependencies) {
    Rectangle rect(0, 0, 10, 20);
    View view;
    Degree delta;

    // view 중심점을 구합니다.
    float x = view.GetCenterX();
    float y = view.GetCenterY();
        
    // view 중심점에 맞춰 회전시킵니다.
    rect.RotateAt(delta, x, y); 
}