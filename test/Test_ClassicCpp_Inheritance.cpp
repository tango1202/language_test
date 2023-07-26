#include "gtest/gtest.h" 

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
}
