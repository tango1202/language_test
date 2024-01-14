#include "gtest/gtest.h" 

TEST(TestPattern, Visitor) {
    class Shape; // 상호 참조가 되어 전방 선언이 필요합니다.
    class Rectangle;
    class Circle;

    // ----
    // #1. 개체를 방문합니다. 방문한 개체가 호출할 수 있도록 VisitRectangle()과 VisitCircle()을 제공합니다.
    // ----
    class IVisitor {
    protected:
        IVisitor() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~IVisitor() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        IVisitor(const IVisitor&) = delete;
        IVisitor(IVisitor&&) = delete;
        IVisitor& operator =(const IVisitor&) = delete;
        IVisitor& operator =(IVisitor&&) = delete;
    public:
        virtual void VisitRectangle(Rectangle& rectangle) = 0;
        virtual void VisitCircle(Circle& circle) = 0;
    };
    // ----
    // #2. IVisitor가 방문할 수 있도록 Accept()를 제공합니다.
    // ----
    class Shape {
    protected:
        Shape() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
        Shape(const Shape&) = default; 
    public:
        virtual ~Shape() = default; // 다형 소멸 하도록 public virtual 
    private:
        Shape(Shape&&) = delete; 
        Shape& operator =(const Shape&) = delete; 
        Shape& operator =(Shape&&) = delete;   
    public:
        virtual void Accept(IVisitor& visitor) = 0;
    };
    // ----
    // #3. IVisitor가 방문하면, 현 개체에 해당하는 함수를 실행합니다.
    // ----
    class Rectangle : public Shape {
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;
    public:
        Rectangle(int l, int t, int w, int h) : m_Left{l}, m_Top{t}, m_Width{w}, m_Height{h} {}
    public:
        int GetLeft() const {return m_Left;}
        int GetTop() const {return m_Top;}
        int GetWidth() const {return m_Width;}
        int GetHeight() const {return m_Height;}

        void SetLeft(int val) {m_Left = val;}
        void SetTop(int val) {m_Top = val;}
        void SetWidth(int val) {m_Width = val;}
        void SetHeight(int val) {m_Height = val;}   

        virtual void Accept(IVisitor& visitor) override {
            visitor.VisitRectangle(*this); // #3
        }
    };
    class Circle : public Shape {
        int m_CenterX;
        int m_CenterY;
        int m_Diameter;
    public:
        Circle(int centerX, int centerY, int diameter) : m_CenterX(centerX), m_CenterY(centerY), m_Diameter(diameter) {}
    public:
        int GetCenterX() const {return m_CenterX;}
        int GetCenterY() const {return m_CenterY;}
        int GetDiameter() const {return m_Diameter;}

        void SetCenterX(int val) {m_CenterX = val;}
        void SetCenterY(int val) {m_CenterY = val;}
        void SetDiameter(int val) {m_Diameter = val;}

        virtual void Accept(IVisitor& visitor) override {
            visitor.VisitCircle(*this); // #3
        }
    };

    // ----
    // #4. 각 개체에서 호출하면, 해당 기능을 실행합니다.
    // ----
    class XmlWriter : public IVisitor {
    public:
        XmlWriter() = default;

        virtual void VisitRectangle(Rectangle& rectangle) override {
            std::cout << "XmlWriter Rectangle" << std::endl; 
        };
        virtual void VisitCircle(Circle& rectangle) override {
            std::cout << "XmlWriter Circle" << std::endl; 
        }
    };

    class JsonWriter : public IVisitor {
    public:
        JsonWriter() = default;

        virtual void VisitRectangle(Rectangle& rectangle) override {
            std::cout << "JsonWriter Rectangle" << std::endl; 
        };
        virtual void VisitCircle(Circle& rectangle) override {
            std::cout << "JsonWriter Circle" << std::endl; 
        }
    };

    // ----
    // 테스트 코드
    // ----
    Rectangle rectangle{0, 0, 10, 20};
    Circle circle{0, 0, 10};

    XmlWriter xmlWriter;
    rectangle.Accept(xmlWriter);
    circle.Accept(xmlWriter);

    JsonWriter jsonWriter;
    rectangle.Accept(jsonWriter);
    circle.Accept(jsonWriter);  


    class ScaleVisitor : public IVisitor {
    public:
        virtual void VisitRectangle(Rectangle& rectangle) override {
            rectangle.SetWidth(rectangle.GetWidth() * 2); // 2배로 크게합니다.
        };
        virtual void VisitCircle(Circle& rectangle) override {
            rectangle.SetDiameter(rectangle.GetDiameter() * 2); // 2배로 크게 합니다.
        }
    };

    ScaleVisitor scaleVisitor; 
    rectangle.Accept(scaleVisitor);
    circle.Accept(scaleVisitor);

    EXPECT_TRUE(rectangle.GetWidth() == 10 * 2 && rectangle.GetHeight() == 20 * 2);
    EXPECT_TRUE(circle.GetDiameter() == 10 * 2);  
}