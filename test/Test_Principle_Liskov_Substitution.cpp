#include "gtest/gtest.h" 

namespace {
    // 크기 변경이 가능한 개체입니다.
    class IResizeable {
    protected : 
        ~IResizeable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.
    public:
        // Get/Set을 구현해야 합니다.
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;

        virtual void SetWidth(int val) = 0;
        virtual void SetHeight(int val) = 0;   
    };

    // IResizeable 구현한 클래스입니다. 다른 클래스에서 상속받아 기능을 포함합니다.
    class ResizeableImpl : 
        public IResizeable {
    private:
        int m_Width;
        int m_Height;    
    protected:
        ResizeableImpl(int w, int h) :
            m_Width(w), 
            m_Height(h) {}
        ~ResizeableImpl() {} // has-a 관계로 사용되기 때문에, 단독으로 생성되지 않도록 protected입니다.
    public:    
        // Get/Set 함수
        virtual int GetWidth() const override {return m_Width;}
        virtual int GetHeight() const override {return m_Height;}

        virtual void SetWidth(int val) override {m_Width = val;}
        virtual void SetHeight(int val) override {m_Height = val;}
    };
        
    // 부모 클래스 입니다. m_Width와 m_Height를 뺐습니다.
    class Shape {
    private:
        int m_Left;
        int m_Top;
    public:
        Shape(int l, int t) :
            m_Left(l),
            m_Top(t) {}
        virtual ~Shape() {} // 다형적 소멸
    public:
        // Get/Set 함수
        int GetLeft() const {return m_Left;}
        int GetTop() const {return m_Top;}

        void SetLeft(int val) {m_Left = val;}
        void SetTop(int val) {m_Top = val;}

        // m_Left, m_Top을 이용하여 자식 클래스들이 알아서 그려야 함
        virtual void Draw() const = 0; 
    };

    // 자식 클래스 입니다. ResizeableImpl을 포함하여 m_Width와 m_Height를 제공합니다.
    class Rectangle : 
        public Shape, 
        public ResizeableImpl {
    public:    
        Rectangle(int l, int t, int w, int h) : 
            Shape(l, t), 
            ResizeableImpl(w, h) {}
        virtual ~Rectangle() override {}   
    
        virtual void Draw() const override {
            // m_Left, m_Top, IResizeable::GetWidth(), IResizeable::GetHeight()를 이용하여 사각형을 그립니다.
        }
    };

    // 자식 클래스 입니다. ResizeableImpl을 포함하여 m_Width와 m_Height를 제공합니다.
    class Ellipse : 
        public Shape, 
        public ResizeableImpl {
    public:
        Ellipse(int l, int t, int w, int h) : 
            Shape(l, t), 
            ResizeableImpl(w, h) {}
        virtual ~Ellipse() override {}
    
        virtual void Draw() const override {
            // m_Left, m_Top, IResizeable::GetWidth(), IResizeable::GetHeight()를 이용하여 타원을 그립니다.
        }
    };

    // 자식 클래스입니다. m_Length를 제공합니다.
    class Square : 
        public Shape {
    public:
        int m_Length;
    public:
        // width, height없이 length 하나만 사용합니다.
        Square(int l, int t, int length) : 
            Shape(l, t),
            m_Length(length) {}
        virtual ~Square() override {}

        // Get/Set 함수
        int GetLength() const {return m_Length;}

        void SeLength(int val) {m_Length = val;}

        virtual void Draw() const override {
            // m_Left, m_Top, m_Length를 이용하여 정사각형을 그립니다.
        }
    };
}

TEST(TestPrinciple, LiskovSubstitution) {
    Rectangle rect(0, 0, 10, 20);
    Ellipse ellipse(0, 0, 10, 20);
    Square square(0, 0, 10); // length를 10으로 설정합니다.

    // 생성자에 입력한 값이 잘 전달됐는지 확인합니다.
    EXPECT_TRUE(rect.GetWidth() == 10 && rect.GetHeight() == 20);
    EXPECT_TRUE(ellipse.GetWidth() == 10 && ellipse.GetHeight() == 20);
    EXPECT_TRUE(square.GetLength() == 10);
}
