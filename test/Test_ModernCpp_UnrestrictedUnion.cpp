#include "gtest/gtest.h" 

namespace {
    // 생성자와 소멸자가 있어서 non-Trivial 타입입니다.
    class A {
        int m_X;
        int m_Y;
    public:
        A(int x, int y) : m_X{x}, m_Y{y} {
            std::cout << "A : Constructor" << std::endl;
        }
        ~A() {
            std::cout << "A : Destructor" << std::endl;    
        }
        int GetX() const {return m_X;}
        int GetY() const {return m_Y;}

        void SetX(int x) {m_X = x;}
        void SetY(int y) {m_Y = y;}
    };

    // 생성자와 소멸자가 있어서 non-Trivial 타입입니다.
    class B {
        std::string m_Str;
    public:
        
        explicit B(const char* str) : m_Str{str} {
            std::cout << "B : Constructor" << std::endl;    
        }
        ~B() {
            std::cout << "B : Destructor" << std::endl;       
        }
        const std::string& GetString() const {return m_Str;}
    };

    class Base {
    public:
        virtual int Func() {return 1;} // #1
    };
    
    // 생성자와 소멸자, 가상 함수가 있어서 non-Trivial 타입입니다.
    class Derived : public Base {
    public:
        Derived() {
            std::cout << "Derived : Constructor" << std::endl;    
        }
        ~Derived() {
            std::cout << "Derived : Destructor" << std::endl;  
        }
        
        virtual int Func() override {return 2;} // #2        
    };

    union MyUnion_11 {
        A m_A; // non-Trivial 타입입니다.
        B m_B; // non-Trivial 타입입니다.
        Derived m_Derived; // non-Trivial 타입입니다.
        MyUnion_11() {
            std::cout << "MyUnion_11 : Constructor" << std::endl;  
        }
        ~MyUnion_11() {
            std::cout << "MyUnion_11 : Destructor" << std::endl;  
        }
    };
}

TEST(TestModern, UnrestrictedUnion) {

    {
        // A, B, Derived 는 non-trival 입니다.
        EXPECT_TRUE(std::is_trivial<A>::value == false);
        EXPECT_TRUE(std::is_trivial<B>::value == false);  
        EXPECT_TRUE(std::is_trivial<Derived>::value == false); 

        // MyUnion 은 A, B, Derived 중 크기가 큰 개체만큼 메모리를 할당합니다.
        EXPECT_TRUE(sizeof(Derived) <= sizeof(A) && sizeof(A) <= sizeof(B) && sizeof(B) == sizeof(MyUnion_11));
        
        // A, B, Derived의 생성자와 소멸자가 호출되지 않습니다.
        MyUnion_11 obj;
    }

    {
        MyUnion_11 obj;

        // A를 사용하기 위해 A의 생성자를 호출합니다.
        new (&obj.m_A) A{10, 20};
        EXPECT_TRUE(obj.m_A.GetX() == 10 && obj.m_A.GetY());

        // A말고 B를 사용하기 위해 기존 A는 소멸시키고 B의 생성자를 호출합니다.
        obj.m_A.~A();
        new (&obj.m_B) B{"Hello"};
        EXPECT_TRUE(obj.m_B.GetString() == "Hello");

        // B 말고 Derived를 사용하기 위해 기존 B는 소멸시키고 Derived의 생성자를 호출합니다.
        obj.m_B.~B();
        new (&obj.m_Derived) Derived;
        EXPECT_TRUE(obj.m_Derived.Func() == 2);

        // obj 사용이 끝나서 Derived 도 소멸합니다.
        obj.m_Derived.~Derived();
    }

}

