#include "gtest/gtest.h" 

// ----
// PImpl
// ----
namespace PImpl_1 {
    // ----
    // 선언에서
    // ----
    class T {
        class Impl; // #2
        Impl* m_Impl; // #2. T::Impl 타입의 클래스 포인터를 멤버 변수로 사용합니다. 선언 시점에 구체 구현은 은닉되어 있습니다. 
    public:
        // val1, val2 : new 로 생성된 것을 전달하세요.
        T(int* val1, int* val2);
        // // (△) 비권장 . m_Impl이 포인터 멤버 변수여서, 복사 생성자에서 복제, 소멸자에서 delete, swap을 이용한 복사 대입 연산자를 구현해야 합니다.
        T(const T& other);
        ~T();
        T& operator =(const T& other);
        void Swap(T& other);

        int GetVal1() const;
        int GetVal2() const;
    };

    // ----
    // 정의에서
    // ----

    // 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
    // 복사 대입 연산은 임시 개체 생성 후 swap 합니다.
    class IntPtr {
    private:
        int* m_Ptr; 
    public: 
        explicit IntPtr(int* ptr) :
            m_Ptr(ptr) {}
        IntPtr(const IntPtr& other) :
            m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {}
        ~IntPtr() {delete m_Ptr;}

        IntPtr& operator =(const IntPtr& other) {
            IntPtr temp(other); 
             Swap(temp); 
            return *this;
        }
        void Swap(IntPtr& other) {
            std::swap(this->m_Ptr, other.m_Ptr);  
        }

        const int* operator ->() const {return m_Ptr;}
        int* operator ->() {return m_Ptr;}

        const int& operator *() const {return *m_Ptr;}
        int& operator *() {return *m_Ptr;}

        bool IsValid() const {return m_Ptr != NULL ? true : false;}    
    };

    class T::Impl {
    public: // T 에서 멤버 변수를 자유롭게 쓰도록 public 입니다.
        // 스마트 포인터를 사용합니다. 암시적 복사 생성자에서 복제본을 만들고, 소멸자에서 잘 소멸합니다.
        IntPtr m_Val1; // #1
        IntPtr m_Val2; // #1
        Impl(int* val1, int* val2) : 
            m_Val1(val1),
            m_Val2(val2) {}
    private:        
        // #6. 복사 대입 연산자는 사용하지 않으므로 private로 못쓰게 만듭니다.
        Impl& operator =(const Impl& other) {return *this;}  
    };

    T::T(int* val1, int* val2) :
        m_Impl(new T::Impl(val1, val2)) {}
    T::T(const T& other) : 
        m_Impl(new T::Impl(*other.m_Impl)) {} // #3. T::Impl의 복사 생성자를 호출합니다.
    T::~T() {delete m_Impl;} // #4. T::Impl을 소멸시킵니다.
    
    // #5. Swap으로 복사 대입 연산자를 구현합니다.
    T& T::operator =(const T& other) {
        T temp(other); 
        Swap(temp); 
        return *this;
    } 
    // #5
    void T::Swap(T& other) {
        // PImpl 이디엄을 사용하면 포인터 끼리의 swap이므로 복사 부하가 거의 없습니다.
        std::swap(this->m_Impl, other.m_Impl);
    }

    // T::Impl의 멤버 변수를 이용합니다.
    int T::GetVal1() const {return *(m_Impl->m_Val1);}
    int T::GetVal2() const {return *(m_Impl->m_Val2);}
}
namespace PImpl_2 {
    // ----
    // 선언에서
    // ----

    // ----    
    // #3. T 선언 : 복사 생성자, 소멸자, swap을 이용한 복사 대입 연산자, Swap 불필요
    // ----
    class T {

        class Impl; // #1. 전방 선언
        // ----    
        // #2. ImplPtr 선언
        // ----
        class ImplPtr {
        private:
            Impl* m_Ptr; // 포인터형 변수로 사용합니다.
        public: 
            explicit ImplPtr(Impl* ptr);
            ImplPtr(const ImplPtr& other);
            ~ImplPtr();

            ImplPtr& operator =(const ImplPtr& other);
            void Swap(ImplPtr& other);

            const Impl* operator ->() const;
            Impl* operator ->();

            const Impl& operator *() const;
            Impl& operator *();

            bool IsValid() const;  
        };       

        // (O) 스마트 포인터를 사용하여, 복사 생성자, 소멸자를 구현할 필요가 없고, 
        // (O) 멤버 변수도 1개여서 Swap으로 복사 대입 연산자를 구현할 필요가 없습니다.
        ImplPtr m_Impl; 
    public:
        // val1, val2 : new 로 생성된 것을 전달하세요.
        T(int* val1, int* val2);

        int GetVal1() const;
        int GetVal2() const;
    };

    // ----
    // 정의에서
    // ----

    // 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
    // 복사 대입 연산은 임시 개체 생성 후 swap 합니다.
    class IntPtr {
    private:
        int* m_Ptr; 
    public: 
        explicit IntPtr(int* ptr) :
            m_Ptr(ptr) {}
        IntPtr(const IntPtr& other) :
            m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {} 
        ~IntPtr() {delete m_Ptr;}

        IntPtr& operator =(const IntPtr& other) {
            IntPtr temp(other); 
             Swap(temp); 
            return *this;
        }
        void Swap(IntPtr& other) {
            std::swap(this->m_Ptr, other.m_Ptr);  
        }

        const int* operator ->() const {return m_Ptr;}
        int* operator ->() {return m_Ptr;}

        const int& operator *() const {return *m_Ptr;}
        int& operator *() {return *m_Ptr;}

        bool IsValid() const {return m_Ptr != NULL ? true : false;}    
    };

    // ----
    // Impl 정의
    // ----
    class T::Impl {
    public: // T 에서 멤버 변수를 자유롭게 쓰도록 public 입니다.
        // 스마트 포인터를 사용합니다. 암시적 복사 생성자에서 복제본을 만들고, 소멸자에서 잘 소멸합니다.
        IntPtr m_Val1;
        IntPtr m_Val2;
        Impl(int* val1, int* val2) : 
            m_Val1(val1),
            m_Val2(val2) {}
    private:        
        // 복사 대입 연산자는 사용하지 않으므로 private로 못쓰게 만듭니다.
        Impl& operator =(const Impl& other) {return *this;}  
    };

    // ----
    // ImplPtr 정의
    // ----
    T::ImplPtr::ImplPtr(T::Impl* ptr) :
        m_Ptr(ptr) {}
    T::ImplPtr::ImplPtr(const T::ImplPtr& other) :
        m_Ptr(other.IsValid() ? new T::Impl(*other.m_Ptr) : NULL) {} // Impl의 복사 생성자를 호출합니다.
    T::ImplPtr::~ImplPtr() {delete m_Ptr;} // Impl을 소멸시킵니다.

    T::ImplPtr& T::ImplPtr::operator =(const T::ImplPtr& other) {
        ImplPtr temp(other); 
        Swap(temp); 
        return *this;
    }
    void T::ImplPtr::Swap(T::ImplPtr& other) {
        std::swap(this->m_Ptr, other.m_Ptr);  
    }

    const T::Impl* T::ImplPtr::operator ->() const {return m_Ptr;}
    T::Impl* T::ImplPtr::operator ->() {return m_Ptr;}

    const T::Impl& T::ImplPtr::operator *() const {return *m_Ptr;}
    T::Impl& T::ImplPtr::operator *() {return *m_Ptr;}

    bool T::ImplPtr::IsValid() const {return m_Ptr != NULL ? true : false;}    

    // ----
    // T 정의
    // ----
    T::T(int* val1, int* val2) :
        m_Impl(new T::Impl(val1, val2)) {}
 
    // TImpl의 멤버 변수를 이용합니다.
    int T::GetVal1() const {return *(m_Impl->m_Val1);}
    int T::GetVal2() const {return *(m_Impl->m_Val2);}    
}
TEST(TestLegacyCpp, PImpl) {
    {
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            using namespace PImpl_2;
            
            T t1(new int(10), new int(20));
            T t2(t1); // 새로운 int형 개체를 만들고 10, 20을 복제합니다.

            EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
        } 
        // (O) 복사 대입 연산 시에도 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            using namespace PImpl_2;
            
            T t1(new int(10), new int(20));
            T t2(new int(1), new int (2));
            t2 = t1; // (O) swap 버전 복사 대입 연산자 호출
            EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
        }
    }
}