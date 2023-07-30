#include "gtest/gtest.h" 

namespace {
    // ---- 
    // 상속 강제 - 순가상 소멸자
    // ----    
    class T {
    public:
        virtual ~T() = 0; // 순가상 소멸자
    };
    T::~T() {} // (△) 비권장. 실제 구현 정의가 있어야 함
}

TEST(TestClassicCpp, Inheritance) {
    // ----
    // 개요
    // ----
    {
        class Base {
        private: 
            int m_Private;
        protected:
            int m_Protected;
        public:
            int m_Public;    
        };
        class Derived : public Base {
            void f() {
                // m_Private; // (X) 컴파일 오류. private 접근 불가
                m_Protected; // (O) 상속한 개체에서는 사용 가능
                m_Public; // (O)
            }
        };
        Derived d;
        // d.m_Private; // (X) 컴파일 오류. private 접근 불가
        // d.m_Protected; // (X) 컴파일 오류. 외부 노출 불가. 상속한 개체내에서는 가능
        d.m_Public; // (O)
    } 
    // ----
    // public 상속, protected 상속, private 상속
    // ----
    {
        class Base {
        private:
            int m_Private;
        protected:
            int m_Protected;
        public:
            int m_Public;
        };

        // 외부 노출이 안되도록 Base protected->private, Base public->private
        class PrivateDerived : private Base { 
            void f() {
                // m_Private; // (X) 컴파일 오류. private 접근 불가
                m_Protected; // (O)
                m_Public; // (O)
            }
        };
        // PrivateDeirved obj1;
        // obj1.m_Private; // (X) 컴파일 오류. private 접근 불가
        // obj1.m_Protected; // (X) 컴파일 오류. private 접근 불가
        // obj1.m_Public; // (X) 컴파일 오류. private 접근 불가

        // 외부 노출이 안되지만 상속은 되도록 Base protected->protected, Base public->protected
        class ProtectedDerived : protected Base { 
            void f() {
                // m_Private; // (X) 컴파일 오류. private 접근 불가
                m_Protected; // (O)
                m_Public; // (O)
            }
        };
        class ProtectedDerived2 : public ProtectedDerived { 
            void g() {
                // m_Private; // (X) 컴파일 오류. private 접근 불가
                m_Protected; // (O)
                m_Public; // (O)
            }
        };

        ProtectedDerived obj2;
        // obj2.m_Private; // (X) 컴파일 오류. private 접근 불가
        // obj2.m_Protected; // (X) 컴파일 오류. protected 접근 불가
        // obj2.m_Public; // (X) 컴파일 오류. protected 접근 불가

        // 외부 노출과 상속은 되도록 Base protected->protected, Base public->public
        class PublicDerived : public Base { 
            void f() {
                // m_Private; // (X) 컴파일 오류. private 접근 불가
                m_Protected; // (O)
                m_Public; // (O)
            }
        };

        PublicDerived obj3;
        // obj3.m_Private; // (X) 컴파일 오류. private 접근 불가
        // obj3.m_Protected; // (X) 컴파일 오류. protected 접근 불가
        obj3.m_Public; // (O) 
    }
    // ----
    // 부모 개체 멤버 접근
    // ----
    {
        class Base {
        public:
            int f() {return 10;} 
        };

        class Derived : public Base {
        public:
            int f() {return 20;} // (△) 비권장. 부모 개체의 이름을 가립니다.
            int f(int) {return 30;} // (△) 비권장. 부모 개체의 함수 오버로딩. 오버로딩 후보군에서 제외됨
        };

        Derived d;
        Base* b = &d;

        EXPECT_TRUE(b->f() == 10); // (△) 비권장. Base 개체를 이용하면 Base::f()가 호출됨   
        EXPECT_TRUE(d.f() == 20);

        EXPECT_TRUE(b->Base::f() == 10); // 부모 개체에 명시적으로 접근합니다.
        EXPECT_TRUE(d.Base::f() == 10); // 부모 개체에 명시적으로 접근합니다.
    }
    // ----
    // 부모 개체의 멤버 함수 오버로딩
    // ----
    {
        class Base {
        public:
            int f() {return 10;} 
        };

        class Derived : public Base {
        public:
            int f(int) {return 20;} // (△) 비권장. 부모 개체의 이름을 가립니다.
        };

        Derived d;
        Base* b = &d;

        EXPECT_TRUE(b->f() == 10);  // (△) 비권장. Base 개체를 이용하면 Base::f()가 호출됨 
        // EXPECT_TRUE(d.f() == 10); // (X) 컴파일 오류. 오버로딩 함수 탐색 규칙에서 제외됨
        EXPECT_TRUE(d.f(1) == 20);

        EXPECT_TRUE(b->Base::f() == 10); 
        EXPECT_TRUE(d.Base::f() == 10);      
    }
    // ----
    // 부모 개체의 가상 함수 오버라이딩
    // ----
    {
        class Base {
        public:
            virtual int f() {return 10;} 
        };

        class Derived : public Base {
        public:
            virtual int f() {return 20;} // (O) 다형적으로 동작합니다.
        };
        Derived d;
        Base* b = &d;

        EXPECT_TRUE(b->f() == 20); // (O) 자식 개체의 함수가 호출됩니다.
        EXPECT_TRUE(b->Base::f() == 10); // (△) 비권장. 부모 개체에 명시적으로 접근하면, 부모 개체의 함수가 호출됩니다.
        EXPECT_TRUE(d.f() == 20);        
    }
    // ----
    // 자식 개체의 생성자 재정의
    // ----
    {
        class Base { 
            int m_X;
            int m_Y;
        public: 
            Base(int x, int y) :
                m_X(x),
                m_Y(y) {}  
        };
        class Derived : public Base {
            int m_Z;
        public: 
            Derived(int x, int y, int z) : 
                Base(x, y), // 부모 클래스 생성자 호출 
                m_Z(z) {}
        };

        Derived d(1, 2, 3);
    }
    // 복사 손실
    {
        class Base { 
            int m_X;
            int m_Y;
        public: 
            Base(int x, int y) :
                m_X(x),
                m_Y(y) {}  
        };
        class Derived : public Base {
            int m_Z;
        public: 
            Derived(int x, int y, int z) : 
                Base(x, y), 
                m_Z(z) {}
        };

        Derived d(1, 2, 3);
        Base b = d; // (X) 오동작. 아무런 경고없이 실행됩니다. 
        // d = b; // (X) 컴파일 오류
    }
    // ----
    // 다중 상속
    // ----
    {
        class Singer {
        public:
            int m_Age; // 동일한 멤버 변수
        };
        class Dancer {
        public:
            int m_Age; // 동일한 멤버 변수
        };
        class Idol : 
            public Singer,
            public Dancer {
        };

        Idol obj;
        // obj.m_Age = 10; // (X) 컴파일 오류. Singer::m_Age와 Dancer::m_Age가 모호합니다.
        obj.Singer::m_Age = 20; // (△) 비권장. 이름이 동일한 멤버는 ::(범위 확인 연산자)를 이용해 접근 가능합니다.
        obj.Dancer::m_Age = 30;

        EXPECT_TRUE(obj.Singer::m_Age == 20);
        EXPECT_TRUE(obj.Dancer::m_Age == 30);       
    }   
    {
        class Person {
        public:
            int m_Age; 
        };
        class Singer : public Person {};
        class Dancer : public Person {};
        class Idol : 
            public Singer, // (△) 비권장. Singer::Person::m_Age 존재
            public Dancer { // (△) 비권장. Dancer::Person::m_Age 존재
        };

        Idol obj;
        obj.Singer::m_Age = 20; 
        obj.Dancer::m_Age = 30;
 
        EXPECT_TRUE(obj.Singer::m_Age == 20);
        EXPECT_TRUE(obj.Dancer::m_Age == 30);               
    } 
    {
        class Person {
        public:
            int m_Age; 
        };
        class Singer : public virtual Person {}; // 상속 트리에서 이름이 같은 조상 개체는 1개로 통합시킵니다.
        class Dancer : public virtual Person {};
        class Idol : 
            public Singer, 
            public Dancer { 
        };

        Idol obj;
        obj.m_Age = 10;
        obj.Singer::m_Age = 20; // obj.m_Age와 동일
        obj.Dancer::m_Age = 30; // obj.m_Age와 동일
        
        EXPECT_TRUE(obj.m_Age == 30);
        EXPECT_TRUE(obj.Singer::m_Age == 30);
        EXPECT_TRUE(obj.Dancer::m_Age == 30);   
    } 
    // ----
    // 상속 강제 - public virtual 소멸자는 순가상 함수 없으면 안됨
    // ----  
    {
        class T {
        public:
            virtual ~T() {}
        };

        T t; // (△) 비권장. 순가상 함수가 없으면 개체 정의(인스턴스화) 할 수 있습니다.        
    }
    // ----
    // 상속 강제 - 순가상 소멸자
    // ----  
    {
        class U : public T {};
        // T t; // (X) 컴파일 오류. 순가상 소멸자가 있어 개체 정의(인스턴스화) 안됨
        U u; // 상속하면 개체 정의(인스턴스화) 가능
    }    
    // ----
    // 상속 제한
    // ----
    {
        class T {
        private:
            T() {} // 상속 및 외부에서는 접근 불가
        public:
            static T Create() {return T();} // 내부 static 함수로 생성
            static T* CreatePtr() {return new T;}
        };
        class U : public T {};
        // U u; // (X) 컴파일 오류. 상속해서 생성할 수 없음
        // U* p = new u; // (X) 컴파일 오류  

        // T t; // (X) 컴파일 오류 
        T t(T::Create()); // (O) T를 복사 생성    
        T* p = T::CreatePtr(); // (O) T의 포인터 생성
        delete p;       
    }
    // is-a 관계
    {
        class Shape {
        public:
            virtual ~Shape() {} // 다형 소멸을 하려면 public virtual 소멸자를 사용합니다.
            virtual void Draw() const {}
        };
        class Rectangle : public Shape {
        public:
            virtual void Draw() const {}
        };
        class Ellipse : public Shape {
        public:
            virtual void Draw() const {}
        };
        class Triangle : public Shape {
        public:
            virtual void Draw() const {}
        };

        Shape* shapes[3] = { // 도형들을 Shape* 로 관리합니다.
            new Rectangle(), 
            new Ellipse(), 
            new Triangle()
        };

        for(int i = 0; i < 3; ++i) {
            shapes[i]->Draw(); // 다형적으로 그립니다.
        }
        for(int i = 0; i < 3; ++i) {
            delete shapes[i]; // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse, Triangle을 소멸합니다.
        }        
    }
    // ----
    // has-a 관계
    // ----
    {
        // 너비/높이에 대한 공통 구현
        class ResizeableImpl {
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
            int GetWidth() const {return m_Width;}
            int GetHeight() const {return m_Height;}

            void SetWidth(int val) {m_Width = val;}
            void SetHeight(int val) {m_Height = val;}
        };
        class Rectangle : public ResizeableImpl {
            int m_Left;
            int m_Top;
        public:
            Rectangle(int l, int t, int w, int h) :
                ResizeableImpl(w, h),
                m_Left(l),
                m_Top(t) {}
        };
        class Ellipse : public ResizeableImpl {
            int m_CenterX;
            int m_CenterY;
        public:
            Ellipse(int centerX, int centerY, int w, int h) :
                ResizeableImpl(w, h),
                m_CenterX(centerX),
                m_CenterY(centerY) {}
        };
        Rectangle r(0, 0, 10, 20);
        Ellipse e(5, 10, 10, 20);
    }
    {
        class IEatable {
        protected:
            ~IEatable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

        public:
            virtual void Eat() = 0;
        };
        class IWalkable {
        protected:
            ~IWalkable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

        public:
            virtual void Walk() = 0;
        };

        class Dog :
            public IEatable,
            public IWalkable {
        public:        
            virtual void Eat() {}
            virtual void Walk() {}
        };
        // IEatable eatable; // (X) 컴파일 오류. 소멸자가 protected
        // IWalkable walkable; // (X) 컴파일 오류. 소멸자가 protected
        Dog dog; // (O)
        
        // IEatable* p = &dog:
        // delete* p; // (X) 컴파일 오류. IEatable의 소멸자가 protected
    }  
    // ----
    // 나쁜 상속 - 부모 개체의 무의미한 구현
    // ---- 
    {
        class Base {
        public:
            virtual void Func() {
                // (△) 비권장. 대충 기본 작업을 합니다.
            }
        };
        class Derived {
        public:
            virtual void Func() {
                // 자식 개체에서 제대로 구현합니다.
            }
        };       
    }  
    // 순가상 함수 사용
    {
        class Base {
        public:
            virtual void Func() = 0; // (O) 순가상 함수
        };
        class Derived {
        public:
            virtual void Func() {
                // 자식 개체에서 제대로 구현합니다.
            }
        };        
    } 
    // 기본 구현 제공
    {
        class Base {
        public:
            virtual void Func() {
                // 기본 구현을 해둡니다.
                // (△) 비권장. 누가 상속해서 사용하는지 파악하기 어렵습니다.
            }
        };
        class Derived1 {
        public:
            // Base의 기본 구현을 사용합니다.
        };
        class Derived2 {
        public:
            // Base의 기본 구현을 사용합니다.
        };
        class Derived3 {
        public:
            // 기본 구현이 마음에 들지 않아 다시 구현합니다.
            virtual void Func() {}
        };        
    }  
    // 유틸리티 제공
    {
        class Base {
        public:
            virtual void Func() = 0; // 순가상 함수
        };
        // (O) Base의 기본 구현을 제공하는 유틸리티 함수를 제공합니다.
        class BaseUtil {
        public:
            static void Func() {}
        };
        class Derived1 {
        public:
            // (O) Base의 기본 구현을 사용합니다.
            virtual void Func() {
                BaseUtil::Func();
            }
        };
        class Derived2 {
        public:
            // (O) Base의 기본 구현을 사용합니다.
            virtual void Func() {
                BaseUtil::Func();
            }
        };
        class Derived3 {
        public:
            // 기본 구현이 마음에 들지 않아 다시 구현합니다.
            virtual void Func() {}
        };        
    }
    // ----
    // up casting, down casting sibling casting 형변환
    // ----
    {
        class ISinger {
        protected:
            ~ISinger() {} // 인터페이스여서 protected non-virtual(상속해서 사용하고, 다형 소멸 안함) 입니다.     
        public:
            virtual void Sing() const = 0; // 노래를 부릅니다.
        };
        class IDancer {
        protected:
            ~IDancer() {} // 인터페이스여서 protected non-virtual(상속해서 사용하고, 다형 소멸 안함) 입니다.   
        public:
            virtual void Dance() const = 0; // 춤을 춥니다.
        };
        class Idol : 
            public ISinger,
            public IDancer {
        public:
            virtual void Sing() const {}
            virtual void Dance() const {}
        };

        Idol obj;
        Idol* idol = &obj;
        ISinger *singer = &obj; // (O) Up casting. 자식 개체에서 부모 개체로는 잘 변환 됩니다.
        IDancer *dancer = &obj;

        // idol = singer; // (X) 컴파일 오류. Down casting. 부모 개체에서 자식 개체로는 변환되지 않습니다.
        idol = dynamic_cast<Idol*>(singer); // (O) dynamic_cast로 Down casting. 자식 개체로 변환됩니다.
        dancer = dynamic_cast<IDancer*>(singer); // (O) dynamic_cast로 Sibling casting. 형제 개체로 변환됩니다.        

        EXPECT_TRUE(idol != NULL);
        EXPECT_TRUE(dancer != NULL);
        EXPECT_TRUE(typeid(obj) == typeid(Idol));
        EXPECT_TRUE(typeid(*idol) == typeid(Idol));
        EXPECT_TRUE(typeid(*singer) == typeid(Idol));
        EXPECT_TRUE(typeid(*dancer) == typeid(Idol));
    }
    // ----
    // 가상 복사 생성자
    // ----
    {
        class Shape {
        protected:
            Shape() {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.
        public:
            Shape(const Shape& other) {
                if (typeid(*this) != typeid(other)) {
                    const std::type_info& ti = typeid(other);
                    std::cout<<ti.name()<<std::endl;  
                } 
            }
            
            virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
        };
        class Rectangle : public Shape {};
        class Ellipse : public Shape {};

        Rectangle rect;
        Ellipse ellipse;
        Shape shape(rect); // (X) 오동작. shape은 Rectangle을 알지 못해 복사 생성할 수 없습니다.        
    }
    // Clone
    {
        class Shape {
        protected:
            Shape() {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.    
            Shape(const Shape& other) {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.
        public:
            virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
            virtual Shape* Clone() const = 0; // (O) 부모 개체에서는 Shape* 으로 리턴합니다.
        };

        class Rectangle : public Shape {
        public:
            virtual Rectangle* Clone() const { // (O) 자식 개체에서는 자식 타입으로 리턴합니다.
                return new Rectangle(*this); // Rectangle의 복사 생성자를 이용하며 복제본을 리턴합니다.
            }
        };
        class Ellipse : public Shape {
        public:
            virtual Ellipse* Clone() const { // (O) 자식 개체에서는 자식 타입으로 리턴합니다.
                return new Ellipse(*this); // Ellipse 복사 생성자를 이용하며 복제본을 리턴합니다.
            }
        };
        Shape* shapes[2] = { 
            new Rectangle(), 
            new Ellipse()
        };
        Shape* clones[2];

        for(int i = 0; i < 2; ++i) {
            clones[i] = shapes[i]->Clone(); // 복제본을 만듭니다.
        }

        // (O) 자식 개체의 타입으로 잘 복제 됩니다. 
        EXPECT_TRUE(typeid(*clones[0]) == typeid(Rectangle));
        EXPECT_TRUE(typeid(*clones[1]) == typeid(Ellipse));

        for(int i = 0; i < 2; ++i) {
            delete shapes[i]; 
            delete clones[i];
        }
    }    
    // ----
    // 부모 개체의 대입 연산
    // ----
    {
        class Shape {
        protected:
            Shape() {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.  
            Shape(const Shape& other) {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.  
        public:
            virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
            Shape& operator =(const Shape& other) { //  (△) 비권장. 자식 개체가 대입될 수 있습니다.
                if (typeid(*this) != typeid(other)) {
                    const std::type_info& ti = typeid(other);
                    std::cout<<ti.name()<<std::endl;  
                } 
                return *this; 
            }
            virtual Shape* Clone() const = 0; 
        };

        class Rectangle : public Shape {
        public:
            virtual Rectangle* Clone() const { 
                return new Rectangle(*this); 
            }
        };
        class Ellipse : public Shape {
        public:
            virtual Ellipse* Clone() const { 
                return new Ellipse(*this); 
            }
        };

        Rectangle rect1;
        Rectangle rect2;
        Ellipse ellipse;
        Shape* shape = &rect1;

        rect1 = rect2; // (O) 메시지 표시 안됨
        *shape = ellipse; // (X) 오동작. 동일한 Shape 타입이어서 실행됩니다. Rectangle에 Ellipse를 대입합니다.
    }
    {
        class Shape {
        protected:    
            Shape& operator =(const Shape& other) {return *this;} // (O) 부모 개체는 사용 못하게 막아 버립니다.
        protected:
            Shape() {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.  
            Shape(const Shape& other) {} // (O) 상속해서 생성할 수 있게끔 protected 입니다.  
        public:
            virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
            virtual Shape* Clone() const = 0; 
        };

        class Rectangle : public Shape {
        public:
            Rectangle& operator =(const Rectangle& other) { // (O) 자식 개체는 사용할 수 있게 합니다.
                return *this;
            }   
            virtual Rectangle* Clone() const { 
                return new Rectangle(*this); 
            }
        };
        class Ellipse : public Shape {
        public:
            Ellipse& operator =(const Ellipse& other) { // (O) 자식 개체는 사용할 수 있게 합니다.
                return *this;
            } 
            virtual Ellipse* Clone() const { 
                return new Ellipse(*this); 
            }
        };

        Rectangle rect1;
        Rectangle rect2;
        Ellipse ellipse;
        Shape* shape = &rect1;

        rect1 = rect2; // (O) 메시지 표시 안됨
        // *shape = rect2; // (X) 컴파일 오류. 대입 연산은 private임         
    }
}
