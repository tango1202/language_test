#include "gtest/gtest.h" 

TEST(TestClassicCpp, Destructor) {
    // ----
    // 개요
    // ----
    {
        class T {
            int* m_Ptr;
        public:
            T() : m_Ptr(new int(10)) {} // new로 생성한 것은
            ~T() {delete m_Ptr;} // (O) delete로 소멸
        };
    }
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
        // (△) 비권장. 다형 소멸이 되지 않아, 기능 개선을 하다 보면 나중에 메모리 릭이 발생할 수 있습니다.
        class U : public T { 
        };
        T t;
        U u;
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
    // ----
    // protected Non - Virtual 소멸자
    // ----
    {
        class Base {
        protected:
            ~Base() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.
        public:
            virtual void Func() = 0;
        };
        class Derived : public Base {
        public:        
            virtual void Func() {}
        };
        // Base base; // (X) 컴파일 오류. 소멸자가 protected
        Derived derived; // (O)

        Base* p = &derived;
        // delete p; // (X) 컴파일 오류. Base의 소멸자가 protected        
    }    
    {
        class Error {
        private:
            std::string m_Message;
        public:
            virtual ~Error() {} // (O) 다형 소멸. 소멸자 외에는 별다른 가상 함수가 없습니다.
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

                f(); // (X) 오동작. 가상 함수를 소멸자에서 호출합니다. Derived는 이미 소멸되었고, Base::f()가 호출됩니다.
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

        Derived d; // (X) 오동작. 소멸자에서 가상 함수 호출
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
        t.Release(); // 소멸자 호출전에 명시적으로 메모리나 리소스 해제
    }
}
