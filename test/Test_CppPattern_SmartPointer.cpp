#include "gtest/gtest.h" 

namespace {
    // 복사 생성과 대입 연산을 할 수 없는 개체
    class Uncopyable {   
    protected:
        Uncopyable() {} // 상속해서만 사용 가능
        ~Uncopyable() {}
    private:
        Uncopyable(const Uncopyable& other) {} // 누군가가 접근하면 private여서 컴파일 오류
        Uncopyable& operator =(const Uncopyable& other) {return *this;}
    };

    // 스택만 할당할 수 있는 개체
    class OnlyStackAssignable {
    protected: 
        OnlyStackAssignable() {} // 상속해서만 사용 가능
        ~OnlyStackAssignable() {}
    private:
        static void* operator new(std::size_t sz) {return NULL;} // 누군가가 접근하면 private여서 컴파일 오류
    };

    // ----
    // DeepPtr
    // ----
    template<typename T> 
    class DeepPtr :
        private OnlyStackAssignable { // new 로 생성 안됨.
        T* m_Ptr;   
    public:
        explicit DeepPtr(T* ptr) :
            m_Ptr(ptr) {}
        DeepPtr(const DeepPtr& other) : 
            m_Ptr(other.Clone()) {}
        ~DeepPtr() {
            Release();
        }   
        DeepPtr& operator =(const DeepPtr& other) {
            DeepPtr temp(other);
            std::swap(this->m_Ptr, temp.m_Ptr);
            return *this;
        } 

    public:
        void Release() {delete m_Ptr;}
        T* Clone() const {
            if (m_Ptr == NULL) {
                return NULL;
            }
            return new T(*m_Ptr); // (△) 비권장. 다형적이면 문제입니다.
        }

        const T* operator ->() const {return m_Ptr;}
        T* operator ->() {return const_cast<T*>(m_Ptr);}

        // NULL 시 예외 발생
        const T& operator *() const {return *m_Ptr;}
        T& operator *() {return *m_Ptr;}

        bool IsValid() const {return m_Ptr != NULL ? true : false;}
    };
  
}

TEST(TestCppPattern, DeepPtr) {
    DeepPtr<int> a(new int(10));
    DeepPtr<int> b(new int(20));

    a = b;
    EXPECT_TRUE(*a == 20);
}