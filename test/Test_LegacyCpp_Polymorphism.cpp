#include "gtest/gtest.h" 

TEST(TestLegacyCpp, Polymorphism) {
    // ----
    // 가상 생성자
    // ----
    {
        class Shape {
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
    // type_info
    {
        class Base {
        public:
            virtual ~Base() {}
            Base& operator =(const Base& other) {
                if (typeid(*this) != typeid(other)) {
                    const std::type_info& ti = typeid(other);
                    std::cout << ti.name() << std::endl;  
                } 
                return *this; 
            }
        };
        class Derived : public Base {};
        class Other : public Base {};

        Derived d1;
        Derived d2;
        Other other;
        Base* base =&d1;

        d1 = d2; // (O) 메시지 표시 안함
        *base = other; // (X) 오동작. 동일한 Base 타입이어서 실행됨.  

    }
    // up casting, down casting sibling casting 형변환
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
        ISinger *singer = &obj; // (O) Up casting. 자식 개체에서 부보 개체로는 잘 변환 됩니다.
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
}