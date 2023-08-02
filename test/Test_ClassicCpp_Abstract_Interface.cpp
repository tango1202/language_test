#include "gtest/gtest.h" 

namespace {
    // ----
    // 마땅한 순가상 함수가 없는 추상 클래스
    // ----    
    class Abstract {
    public:
        virtual ~Abstract() = 0; // 다형 소멸 하도록 public virtual, 마땅히 할게 없어 순가상
    };
    Abstract::~Abstract() {} // 실제 구현 정의가 있어야 함   
}

TEST(TestClassicCpp, Abstract) {
    // ----
    // 추상 클래스
    // ----
    {
        class Shape {
            // 모든 도형은 왼쪽 상단 좌표와 크기를 가집니다.
            int m_Left;
            int m_Top;
            int m_Width;
            int m_Height;
            Shape(const Shape& other) {} // 추상 클래스여서 외부에서 사용 못하게 복사 생성자 막음
            Shape& operator =(const Shape& other) {return *this;} // 추상 클래스여서 외부에서 사용 못하게 대입 연산자 막음    
        protected:
            Shape() {} // 추상 클래스여서 상속한 개체에서만 생성할 수 있게함               
        public:
            virtual ~Shape() {} // 다형 소멸 하도록 public virtual
            virtual void Draw() const = 0; // 순가상 함수입니다. 자식 클래스에서 구체화 해야 합니다.
        };

        // Shape을 구체화한 클래스 입니다. Draw()에서 사각형을 그립니다.
        class Rectangle : public Shape {
        public:
            virtual void Draw() const {
                std::cout<<"Rectangle::Draw()"<<std::endl;
            }
        };
        // Shape을 구체화한 클래스 입니다. Draw()에서 타원을 그립니다.
        class Ellipse : public Shape {
        public:
            virtual void Draw() const {
                std::cout<<"Ellipse::Draw()"<<std::endl;
            }
        };

        // Shape shape; // (X) 컴파일 오류. 추상 클래스는 순가상 함수가 있어 구체화 할 수 없습니다.
        // Shape* p = new Shape; // (X) 컴파일 오류. 추상 클래스는 순가상 함수가 있어 구체화 할 수 없습니다.

        Shape* shapes[2] = { // 도형들을 Shape* 로 관리합니다.
            new Rectangle(), 
            new Ellipse()
        };

        for(int i = 0; i < 2; ++i) {
            shapes[i]->Draw(); // 다형적으로 그립니다.
        }
        for(int i = 0; i < 2; ++i) {
            delete shapes[i]; // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse을 소멸합니다.
        }         
    }
    // ----
    // 마땅한 순가상 함수가 없는 추상 클래스
    // ----  
    {
        class Abstract {
        public:
            virtual ~Abstract() {} // 다형 소멸 하도록 public virtual
        };

        Abstract abstract; // (X) 오동작. 순가상 함수가 없어 인스턴스화 됩니다. 추상 클래스여서 인스턴스화 되면 안됩니다.
    }
    {
        class Abstract {
        protected:
            Abstract() {} // 상속을 강제합니다.
        public:
            virtual ~Abstract() {} // 다형 소멸 하도록 public virtual
        };

        // Abstract abstract; // (X) 정상 코딩 계약. 추상 클래스를 인스턴스화 하지 못합니다.    
    }
    {
        class Concrete : public Abstract {
        };    

        // Abstract abstract; // (X) 정상 코딩 계약. 추상 클래스는 순가상 함수가 있어 인스턴스화 할 수 없습니다.
        Concrete concrete; // (O) 상속하면 인스턴스화 가능
    }  
}
TEST(TestClassicCpp, Interface) {
    // ----
    // 인터페이스
    // ----
    // 인터페이스
    class IDrawable {
    private:    
        IDrawable(const IDrawable& other) {} // 인터페이스여서 외부에서 사용 못하게 복사 생성자 막음
        IDrawable& operator =(const IDrawable& other) {return *this;} // 인터페이스여서 외부에서 사용 못하게 대입 연산자 막음     
    protected:
        IDrawable() {} // 인터페이스여서 상속한 개체에서만 생성할 수 있게함
        ~IDrawable() {} // 인터페이스여서 protected non-virtual(상속해서 사용하고, 다형 소멸 안함) 입니다. 
    public:
        virtual void Draw() const = 0; // 순가상 함수입니다. 자식 클래스에서 구체화 해야 합니다.
    };

    // 추상 클래스
    class Shape : 
        public IDrawable { // Shape은 IDrawable 인터페이스를 제공합니다.
        // 모든 도형은 왼쪽 상단 좌표와 크기를 가집니다.
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;
    public:
        virtual ~Shape() {} // 다형 소멸 하도록 public virtual
    };

    // Shape을 구체화한 클래스 입니다. Draw()에서 사각형을 그립니다.
    class Rectangle : public Shape {
    public:
        virtual void Draw() const {
            std::cout<<"Rectangle::Draw()"<<std::endl;
        }
    };
    // Shape을 구체화한 클래스 입니다. Draw()에서 타원을 그립니다.
    class Ellipse : public Shape { 
    public:
        virtual void Draw() const {
            std::cout<<"Ellipse::Draw()"<<std::endl;
        }
    };

    // Shape shape; // (X) 컴파일 오류. 추상 클래스는 순가상 함수가 있어 구체화 할 수 없습니다.
    // Shape* p = new Shape; // (X) 컴파일 오류. 추상 클래스는 순가상 함수가 있어 구체화 할 수 없습니다.

    // IDrawable drawable; // (X) 컴파일 오류. 인터페이스는 순가상 함수가 있어 구체화 할 수 없습니다.
    // IDrawable* p = new IDrawable; // (X) 컴파일 오류. 인터페이스는 순가상 함수가 있어 구체화 할 수 없습니다. 

    // (O) Shape으로 다형 소멸 합니다.
    {
        Shape* shapes[2] = { // 도형들을 Shape* 로 관리합니다.
            new Rectangle(), 
            new Ellipse()
        };
        
        // (O) Shape이 IDrawable을 상속했으므로 Draw() 할 수 있습니다.
        for(int i = 0; i < 2; ++i) {
            shapes[i]->Draw(); // 다형적으로 그립니다.
        }

        for(int i = 0; i < 2; ++i) {
            delete shapes[i]; // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse을 소멸합니다.
        } 
    }
    // (O) IDrawable 인터페이스로 Draw() 할 수 있습니다.
    {
        class DrawUtil {
        public:
            static void Draw(const IDrawable& drawable) {
                drawable.Draw();
            }    
        };
        
        Shape* shapes[2] = { // 도형들을 Shape* 로 관리합니다.
            new Rectangle(), 
            new Ellipse()
        };

        // (O) IDrawable 인터페이스로 다형적으로 그립니다. 
        for(int i = 0; i < 2; ++i) {
            DrawUtil::Draw(*shapes[i]); 
        }
        for(int i = 0; i < 2; ++i) {
            delete shapes[i]; // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse을 소멸합니다.
        } 
    }
    // (X) 컴파일 오류. protected 소멸자 이므로, IDrawable로 다형 소멸 되지 않습니다.
    {
        // IDrawable* drawables[2] = {
        //     new Rectangle(), 
        //     new Ellipse()
        // };

        // for(int i = 0; i < 2; ++i) {
        //     drawables[i]->Draw(); // 다형적으로 그립니다.
        // }

        // for(int i = 0; i < 2; ++i) {
        //     delete drawables[i]; // (X) 컴파일 오류. 인터페이스는 다형 소멸을 제공하지 않습니다.
        // }     
    }    
}
