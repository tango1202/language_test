#include "gtest/gtest.h" 


namespace {

    class Shape {
    protected:
        Shape() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        Shape(const Shape&) = delete; 
        Shape(Shape&&) = delete; 
        Shape& operator =(const Shape&) = delete; 
        Shape& operator =(Shape&&) = delete;   
    public:
        virtual ~Shape() = default; // 다형 소멸 하도록 public virtual    

        virtual void Draw() const = 0; // #1
    };
    class Rectangle : public Shape {
    public:
        Rectangle() = default;

        virtual void Draw() const override { // #2
            std::cout << "Rectangle::Draw()" << std::endl;
        }
    };
    class Ellipse : public Shape {
    public:
        Ellipse() = default;

        virtual void Draw() const override { // #2
            std::cout << "Ellipse::Draw()" << std::endl;
        }
    };

    class Circle { // Shape을 상속하지 않았습니다.
    public:
        Circle() = default;

        void Render() const { // #3
            std::cout << "Circle::Render()" << std::endl;            
        }
    };  

    // ----
    // 클래스 Adapter. Circle 클래스를 Shape 처럼 사용할 수 있게 해줍니다.
    // ----
    class CircleClassAdapter : public Shape, private Circle {
    public:
        CircleClassAdapter() = default;

        virtual void Draw() const override { // #4
            Render();
        }   
    };

    // ----
    // 개체 Adapter. Circle 개체를 Shape처럼 사용할 수 있게 해줍니다.
    // ----
    class CircleObjectAdapter : public Shape {
    private:
        std::unique_ptr<Circle> m_Circle;
    public:
        explicit CircleObjectAdapter(std::unique_ptr<Circle> circle) : m_Circle(std::move(circle)) {}
        virtual void Draw() const override {  // #4
            m_Circle->Render();
        } 
    };
}

TEST(TestPattern, Adapter) {

    {
        std::vector<std::unique_ptr<Shape>> v;
        v.emplace_back(new Rectangle{}); 
        v.emplace_back(new Ellipse{});
        // v.emplace_back(new Circle{}); // (X) #5. 컴파일 오류. Circle은 Shape이 아닙니다.
        v.emplace_back(new CircleClassAdapter{}); // #6
        v.emplace_back(new CircleObjectAdapter{std::unique_ptr<Circle>{new Circle{}}}); // #6

        for (auto& shape : v) {
            shape->Draw();
        }
    }
}