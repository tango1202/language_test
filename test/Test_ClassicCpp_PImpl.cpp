#include "gtest/gtest.h" 

// ----
// PImpl
// ----
namespace {
    // // ----
    // // 선언에서
    // // ----
    // class T {
    //     class Impl;
    //     Impl* m_Impl; // T::Impl 타입의 클래스 포인터를 멤버 변수로 사용합니다. 선언 시점에 구체 구현은 은닉되어 있습니다. 
    // public:
    //     // val1, val2 : new 로 생성된 것을 전달하세요.
    //     T(int* val1, int* val2);
    //     T(const T& other);
    //     ~T();
    //     T& operator =(const T& other);
    //     void Swap(T& other);

    //     int GetVal1() const;
    //     int GetVal2() const;
    // };

    // // ----
    // // 정의에서
    // // ----

    // // 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
    // // 대입 연산은 임시 개체 생성 후 swap 합니다.
    // class IntPtr {
    // private:
    //     int* m_Ptr; 
    // public: 
    //     explicit IntPtr(int* ptr) :
    //         m_Ptr(ptr) {}
    //     IntPtr(const IntPtr& other) :
    //         m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {}
    //     ~IntPtr() {delete m_Ptr;}

    //     IntPtr& operator =(const IntPtr& other) {
    //         IntPtr temp(other); 
    //          Swap(temp); 
    //         return *this;
    //     }
    //     void Swap(IntPtr& other) {
    //         std::swap(this->m_Ptr, other.m_Ptr);  
    //     }

    //     const int* operator ->() const {return m_Ptr;}
    //     int* operator ->() {return m_Ptr;}

    //     const int& operator *() const {return *m_Ptr;}
    //     int& operator *() {return *m_Ptr;}

    //     bool IsValid() const {return m_Ptr != NULL ? true : false;}    
    // };

    // class T::Impl {
    // public: // T 에서 멤버 변수를 자유롭게 쓰도록 public 입니다.
    //     // 스마트 포인터를 사용합니다. 암시적 복사 생성자에서 복제본을 만들고, 소멸자에서 잘 소멸합니다.
    //     IntPtr m_Val1;
    //     IntPtr m_Val2;
    //     Impl(int* val1, int* val2) : 
    //         m_Val1(val1),
    //         m_Val2(val2) {}
    // private:        
    //     // 대입 연산자는 사용하지 않으므로 private로 못쓰게 만듭니다.
    //     Impl& operator =(const Impl& other) {return *this;}  
    // };

    // T::T(int* val1, int* val2) :
    //     m_Impl(new T::Impl(val1, val2)) {}
    // T::T(const T& other) :
    //     m_Impl(new T::Impl(*other.m_Impl)) {} // T::Impl의 복사 생성자를 호출합니다.
    // T::~T() {delete m_Impl;} // T::Impl을 소멸시킵니다.
    
    // // Swap을 이용하여 대입 연산자를 구현합니다.
    // T& T::operator =(const T& other) {
    //     T temp(other); 
    //     Swap(temp); 
    //     return *this;
    // } 
    // void T::Swap(T& other) {
    //     std::swap(this->m_Impl, other.m_Impl);
    // }

    // // T::Impl의 멤버 변수를 이용합니다.
    // int T::GetVal1() const {return *(m_Impl->m_Val1);}
    // int T::GetVal2() const {return *(m_Impl->m_Val2);}

    // ----
    // 선언에서
    // ----

    class TImpl; // 중첩 클래스는 전방 선언이 안되어 별도 클래스로 선언하고, 전방 선언합니다.
    class TImplPtr {
    private:
        TImpl* m_Ptr; 
    public: 
        explicit TImplPtr(TImpl* ptr);
        TImplPtr(const TImplPtr& other);
        ~TImplPtr();

        TImplPtr& operator =(const TImplPtr& other);
        void Swap(TImplPtr& other);

        const TImpl* operator ->() const;
        TImpl* operator ->();

        const TImpl& operator *() const;
        TImpl& operator *();

        bool IsValid() const;  
    };

    class T {
        TImplPtr m_Impl; // 중첩 클래스는 전방 선언이 안되어 별도 클래스로 선언하고, 전방 선언합니다.
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
    // 대입 연산은 임시 개체 생성 후 swap 합니다.
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

    // TImpl 정의
    class TImpl {
    public: // T 에서 멤버 변수를 자유롭게 쓰도록 public 입니다.
        // 스마트 포인터를 사용합니다. 암시적 복사 생성자에서 복제본을 만들고, 소멸자에서 잘 소멸합니다.
        IntPtr m_Val1;
        IntPtr m_Val2;
        TImpl(int* val1, int* val2) : 
            m_Val1(val1),
            m_Val2(val2) {}
    private:        
        // 대입 연산자는 사용하지 않으므로 private로 못쓰게 만듭니다.
        TImpl& operator =(const TImpl& other) {return *this;}  
    };

    // TImplPtr 정의
    TImplPtr::TImplPtr(TImpl* ptr) :
        m_Ptr(ptr) {}
    TImplPtr::TImplPtr(const TImplPtr& other) :
        m_Ptr(other.IsValid() ? new TImpl(*other.m_Ptr) : NULL) {}
    TImplPtr::~TImplPtr() {delete m_Ptr;}

    TImplPtr& TImplPtr::operator =(const TImplPtr& other) {
        TImplPtr temp(other); 
        Swap(temp); 
        return *this;
    }
    void TImplPtr::Swap(TImplPtr& other) {
        std::swap(this->m_Ptr, other.m_Ptr);  
    }

    const TImpl* TImplPtr::operator ->() const {return m_Ptr;}
    TImpl* TImplPtr::operator ->() {return m_Ptr;}

    const TImpl& TImplPtr::operator *() const {return *m_Ptr;}
    TImpl& TImplPtr::operator *() {return *m_Ptr;}

    bool TImplPtr::IsValid() const {return m_Ptr != NULL ? true : false;}    

    // T 정의
    T::T(int* val1, int* val2) :
        m_Impl(new TImpl(val1, val2)) {}
 
    // TImpl의 멤버 변수를 이용합니다.
    int T::GetVal1() const {return *(m_Impl->m_Val1);}
    int T::GetVal2() const {return *(m_Impl->m_Val2);}    
}
TEST(TestClassicCpp, PImpl) {
    {
        // (O) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10), new int(20));
            T t2(t1); // 새로운 int형 개체를 만들고 10, 20을 복제합니다.

            EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
        } 
        // (O) 대입 연산 시에도 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
        {
            T t1(new int(10), new int(20));
            T t2(new int(1), new int (2));
            t2 = t1; // (O) swap 버전 대입 연산자 호출
            EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
        }
    }
}