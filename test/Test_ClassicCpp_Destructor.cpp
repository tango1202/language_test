#include "gtest/gtest.h" 

namespace {
    // ---- 
    // 순가상 소멸자
    // ----    
    class T {
    public:
        virtual ~T() = 0; // 순가상 소멸자
    };
    T::~T() {} // 실제 구현 정의가 있어야 함
}

TEST(TestClassicCpp, Destructor) {

    // ---- 
    // 소멸자 호출 순서
    // ----
    {
        class BaseMemberObj {
        public:
            BaseMemberObj() {std::cout<<"1. BaseMemberObj::BaseMemberObj()"<<std::endl;}
            ~BaseMemberObj() {std::cout<<"10. BaseMemberObj::~BaseMemberObj()"<<std::endl;}
        };
        class BaseLocalObj {
        public:
            ~BaseLocalObj() {std::cout<<"9. BaseLocalObj::~BaseLocalObj()"<<std::endl;}
        };
        class Base {
            BaseMemberObj m_BaseMemberObj;
        public:
            Base() {std::cout<<"2. Base::Base()"<<std::endl;}
            ~Base() {
                BaseLocalObj baseLocalObj;
                std::cout<<"8. Base::~Base()"<<std::endl;
            }
        };
        class DerivedMemberObj {
        public:
            DerivedMemberObj() {std::cout<<"3. DerivedMemberObj::DerivedMemberObj()"<<std::endl;}
            ~DerivedMemberObj() {std::cout<<"7. DerivedMemberObj::~DerivedMemberObj()"<<std::endl;}
        };
        class DerivedLocalObj {
        public:
            ~DerivedLocalObj() {std::cout<<"6. DerivedLocalObj::~DerivedLocalObj()"<<std::endl;}
        };        
        class Derived : public Base {
            DerivedMemberObj m_DerivedMemberObj;
        public:
            Derived() {std::cout<<"4. Derived::Derived()"<<std::endl;}
            ~Derived() {
                DerivedLocalObj derivedLocalObj;
                std::cout<<"5. Derived::~Derived()"<<std::endl;
            }
        };

        Derived d;
    }
    // ----
    // public Non-Virtual 소멸자
    // ----
    {
        class T {
        public:
            ~T() {}
        };
        class U : public T { // (△) 비권장. has-a 관계가 명확하지 않음. is-a 관계에서는 사용 금지
        };
        T t;
        U u;
    }
    // ----
    // protected Non-Virtual 소멸자
    // ----
    {
        class IEatable {
        protected:
            ~IEatable() {} // 상속받지만, 다형적으로 사용하지 않아 non-virtual 입니다.

        public:
            virtual void Eat() = 0;
        };
        class IWalkable {
        protected:
            ~IWalkable() {} // 상속받지만, 다형적으로 사용하지 않아 non-virtual 입니다.

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
        // delete* p; // (△) 비권장. 단위 전략 인터페이스를 이용하여 다형 소멸 하지 마세요. 메모리 릭이 발생합니다. 다형 소멸 참고.
    }
    {
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
    // ----
    // public Virtual 소멸자
    // ----
    {
        class Base {
        public:
            // ~Base() {std::cout<<"2. Base::~Base()"<<std::endl;} // (△) 비권장. 그냥 public non-virtual 이어서 다형 소멸을 지원하지 않습니다.
            virtual ~Base() {std::cout<<"2. Base::~Base()"<<std::endl;} // (O) 다형 소멸을 지원함
        };

        class Derived : public Base {
        public:
            ~Derived() {std::cout<<"1. Derived::~Derived()"<<std::endl;}
        };

        Derived* d = new Derived;
        //delete d; // 1, 2 호출됨
        Base* b = d;
        // delete b; // (X) 오동작. 2만 호출됨. Derived 소멸자가 호출되지 않음. 메모리 릭.
        delete b; // (O) 1, 2 호출됨. 다형 소멸 지원.
    }
    {
        class Error {
        private:
            std::string m_Message;
        public:
            virtual ~Error() {} // (O) 가상 함수가 없어 억지로 만듬
            // 모든 자식 개체들에서 사용합니다. 가상 함수는 없습니다.
            void SetMessage(const char* msg) {m_Message = msg;}
            const std::string& GetMessage() const {return m_Message;}
        };

        // 모든 자식 개체들이 타입명만 다릅니다.
        class FileError : public Error {};
        class NetworkError : public Error {};
        class PrintError : public Error {};

        std::vector<Error*> errors;
        errors.push_back(new FileError());
        errors.push_back(new NetworkError());
        errors.push_back(new PrintError());

        // 다형 소멸
        for(std::vector<Error*>::iterator itr = errors.begin(); itr != errors.end(); ++itr) {
            delete *itr;
        }
        errors.clear();        
    }
    // ----
    // 순가상 소멸자
    // ----  
    {
        class U : public T {};
        // T t; // (X) 컴파일 오류. 순가상 소멸자가 있어 개체 정의(인스턴스화) 안됨
        U u; // 상속하면 개체 정의(인스턴스화) 가능
    }
    // ----
    // 소멸자에서 가상 함수 호출 금지
    // ----
    {
        class BaseMemberObj {
        public:
            ~BaseMemberObj() {std::cout<<"4. BaseMemberObj::~BaseMemberObj()"<<std::endl;}
        };
        class Base {
            BaseMemberObj m_BaseMemberObj;
        public:
            virtual ~Base() {
                std::cout<<"3. Base::~Base()"<<std::endl;

                f(); // (△) 비권장. 소멸자에서 가상 함수 호출. Derived는 이미 소멸되었고, Base::f()가 호출됨
            }
            virtual void f() {std::cout<<"*** Base::f() ***"<<std::endl;}
        };
        class DerivedMemberObj {
        public:
            ~DerivedMemberObj() {std::cout<<"2. DerivedMemberObj::~DerivedMemberObj()"<<std::endl;}
        };
    
        class Derived : public Base {
            DerivedMemberObj m_DerivedMemberObj;
        public:
            ~Derived() { std::cout<<"1. Derived::~Derived()"<<std::endl;}
            virtual void f() {std::cout<<"*** Derived::f() ***"<<std::endl;}
        };

        Derived d; // 소멸자에서 가상 함수 호출
    }
    // ----
    // 소멸자에서 예외 발생 금지
    // ----
    {
        class T {
        public:
            void Release() {} // 예외 발생할 수 있는 경우, 메모리나 리소스 해제
        };     

        T t;
        t.Release(); // 예외 발생할 수 있는 경우, 메모리나 리소스 해제
    }
}
