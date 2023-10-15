#include "gtest/gtest.h" 

namespace {
    class Shape; // 상호 참조가 되어 전방선언이 필요합니다.

    // 개체를 방문하여 개체별로 작업합니다.
    class IVisitor {
    protected:
        ~IVisitor() {}  // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

    public:
        // object는 Rectangle입니다. 필요하면 dynamic_cast를 합니다.
        virtual void VisitRectangle(Shape* object) = 0;

        // object는 Circle입니다. 필요하면 dynamic_cast를 합니다.
        virtual void VisitCircle(Shape* object) = 0;
    };

    // 개체를 Scale합니다.
    class ScaleVisitor : public IVisitor {
    public:
        virtual void VisitRectangle(Shape* object) override {
            std::cout << "Scale Rectangle" << std::endl; 
        };
        virtual void VisitCircle(Shape* object) override {
            std::cout << "Scale Circle" << std::endl; 
        }
    };

    // 개체를 Rotate합니다.
    class RotateVisitor : public IVisitor {
    public:
        virtual void VisitRectangle(Shape* object) override {
            std::cout << "Rotate Rectangle" << std::endl; 
        };
        virtual void VisitCircle(Shape* object) override {
            std::cout << "Rotate Circle" << std::endl; 
        }
    };

    class Shape {
    private:
        int m_Left;
        int m_Top;
    public:
        Shape(int l, int t) :
            m_Left(l),
            m_Top(t) {
        }
        virtual ~Shape() {} // 다형적 소멸
    public:
        // m_Left, m_Top으로이용하여 자식 클래스들이 알아서 그려야 함
        virtual void Draw() const = 0; 

        // visitor 방문을 허용합니다.
        virtual void Accept(IVisitor& visitor) = 0;
    };

    class Rectangle : public Shape {
    private:
        int m_Width;
        int m_Height;
    public:    
        Rectangle(int l, int t, int w, int h) : 
            Shape(l, t), 
            m_Width(w), m_Height(h) {}
        virtual ~Rectangle() override {}   

        virtual void Draw() const override {}

        // visitor 방문을 허용합니다.
        virtual void Accept(IVisitor& visitor) override {
            visitor.VisitRectangle(this);   
        }
    };

    class Circle : public Shape {
    private:
        int m_Diameter;
    public:    
        Circle(int l, int t, int diameter) : 
            Shape(l, t), 
            m_Diameter(diameter) {}
        virtual ~Circle() override {}   

        virtual void Draw() const override {}

        // visitor 방문을 허용합니다.
        virtual void Accept(IVisitor& visitor) override {
            visitor.VisitCircle(this);
        }
    };    
}

TEST(TestPrinciple, OpenClosed) {
    Rectangle rect(0, 0, 10, 20);  
    Circle circle(0, 0, 10);

    ScaleVisitor scaleVisitor; // 방문하여 개체들을 Scale 합니다.
    rect.Accept(scaleVisitor);
    circle.Accept(scaleVisitor);

    RotateVisitor rotateVisitor; // 방문하여 개체들을 Rotate 합니다.
    rect.Accept(rotateVisitor);
    circle.Accept(rotateVisitor);
}