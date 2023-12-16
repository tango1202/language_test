#include "gtest/gtest.h" 

namespace AutoPtr_1 {
    class T {};
    class U {
        T* m_T; // T의 포인터를 저장합니다.
    public:
        explicit U(T* t) : m_T(t) {}
    };

    U Func() {
        T* t = new T();
        U u(t);
        delete t; // (X) 오동작. t를 new로 생성했으니 delete 합니다. 그럼 u에 저장된 t는 어쩔까요?
        return u; // (X) 오동작. 소멸된 t를 담고 있는 u를 사용하면 안되죠.
    }    
}
namespace AutoPtr_2 {
    class T {};
    class U {
    public:
        T* m_T; // T의 포인터를 저장합니다.
    public:
        explicit U(T* t) : m_T(t) {}
    };
}
namespace AutoPtr_3 {
    class T {};
    class U {
    public:
        T* m_T; // T의 포인터를 저장합니다.
    public:
        explicit U(T* t) : m_T(t) {}
        ~U() {delete m_T;} // 소멸자에서 소멸시킵니다.
    };

}
namespace AutoPtr_4 {
    class T {};
    class U {
    private:

        // T의 포인터를 auto_ptr로 관리합니다.
        // 복사 생성자, 복사 대입 연산자에서 소유권을 이전합니다.
        // auto_ptr 소멸시 T를 알아서 소멸시킵니다. 
#if 201103L <= __cplusplus // C++11~ 
        std::unique_ptr<T> m_T;
#else       
        std::auto_ptr<T> m_T;
#endif
    public:
        explicit U(T* t) : m_T(t) {}
        T* GetPtr() {return m_T.get();}
    };
}
namespace AutoPtr_5 {
    template<typename T>
    class my_auto_ptr { 
        T* m_Ptr; 
    public:
        my_auto_ptr(T* ptr) : 
            m_Ptr(ptr) {}
        ~my_auto_ptr() {
            delete m_Ptr;
        }
    
        // 소유권 이전
        my_auto_ptr(my_auto_ptr& other) : 
            m_Ptr(other.m_Ptr) { 
            other.m_Ptr = NULL; // 소유권이 this로 왔으므로 other는 NULL 입니다.
        }
        my_auto_ptr& operator =(my_auto_ptr& other) { 
            delete m_Ptr; // 이전 관리하던 포인터는 소멸시킵니다.
            m_Ptr = other.m_Ptr; 
            other.m_Ptr = NULL; // 소유권이 this로 왔으므로 other는 NULL 입니다.

            return *this;
        } 
        T* operator ->() {
            return m_Ptr;
        } 
        T& operator *() {
            return *m_Ptr;
        } 
        T* get() {
            return m_Ptr;
        } 
        T* release() { 
            T* result = m_Ptr; 
            m_Ptr = NULL; // 더이상 소유권 없습니다.
            return result; 
        }
    };
}
TEST(TestLegacyCpp, AutoPtr) {
    {
        using namespace AutoPtr_2;

        U u1(new T);
        U u2 (new T);

        delete u1.m_T; // (△) 비권장. U의 사용법을 잘 숙지하고, 엄청 꼼꼼하게 잊지 않고 호출해야 합니다.
        delete u2.m_T;
    }
    {
        using namespace AutoPtr_3;

        U u1(new T); // (O) delete m_T;는 U 소멸자에서 호출합니다.
        U u2(new T);

        // u1 = u2; //(X) 예외 발생. 기존 u1 것은 delete해야 하고, u1, u2는 동일한 포인터를 소유합니다. 누가 소멸시켜야 하나요? 
    } 
    {
        using namespace AutoPtr_4;
   
        U u1(new T); // (O)명시적으로 T를 delete하지 않아도 auto_ptr 자동 소멸시 소멸됩니다.
        U u2(new T);
#if 201103L <= __cplusplus // C++11~
        u1 = std::move(u2);
#else
        u1 = u2; //(O) 소유권을 u1으로 이전합니다. u2는 무효화된 포인터가 됩니다. 
        EXPECT_TRUE(u1.GetPtr() != NULL);
        EXPECT_TRUE(u2.GetPtr() == NULL);       
#endif
    }   
    {
        using namespace AutoPtr_5;

        my_auto_ptr<int> a(new int(10));
        my_auto_ptr<int> b(new int(20));

        a = b;

        EXPECT_TRUE(*a == 20); // 이전 b 값을 저장합니다.
        EXPECT_TRUE(b.get() == NULL); // 더이상 포인터를 관리하지 않습니다.
    }
}
