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

    // 유효 범위가 지나면, T 타입의 포인터를 소멸시키는 개체
    template<typename T>
    class Holder : 
        private Uncopyable,
        private OnlyStackAssignable {
        T* m_Ptr;
    public:
        explicit Holder(T* ptr) : 
            m_Ptr(ptr) {}
        ~Holder() {
            delete m_Ptr;
        }
        bool IsValid() const {return m_Ptr != NULL ? true : false;}
    };

    // 유효 범위가 지나면, 설정한 속성을 복원시키는 개체
    template< typename GetterT, typename SetterT, typename ValueT>
    class Restorer : 
        private Uncopyable,
        private OnlyStackAssignable {
        SetterT m_Setter;
        ValueT m_OldValue;
    public:
        Restorer(GetterT getter, SetterT setter, ValueT value) :
            m_Setter(setter), 
            m_OldValue(getter()) { // 이전값 기억
                m_Setter(value); // value로 설정
        }
        ~Restorer() {
            m_Setter(m_OldValue); // 이전값 복원
        }
    };   

    void Func1() {throw std::bad_alloc();} // 일부러 예외를 발생시킴
    void Func2() {}   
}

TEST(TestCppPattern, Holder) {
    class T {
    public:
        ~T() {
            std::cout<<"delete T"<<std::endl; // 화면에 표시되는 지 확인합니다.
        }        
    };
    // Func1에서 예외를 발생해도 정상 동작하는지 확인
    {
        try {
            T* ptr = new T;
            Holder<T> holder(ptr);
            Func1(); // 예외 발생
            Func2();
        } 
        catch (...) {
            // ptr의 소멸자가 호출되고 catch에 진입함
        }
    } // 유효 범위를 벗어나면, 예외가 발생하면 holder의 소멸자가 호출되어 ptr이 소멸됩니다.

    // 복사 생성, 대입 연산, new 생성이 컴파일 안되는지 확인
    {
        Holder<T> holder1(new T);
        Holder<T> holder2(new T);
        // Holder<T> holder3(holder1); // (X) 컴파일 오류. 복사 생성자를 막았습니다.
        // holder2 = holder1; // (X) 컴파일 오류. 대입 연산자를 막았습니다.
        // Holder<T> *p = new Holder<T>(new T); // (X) 컴파일 오류. Holder를 new로 생성하지 못하도록 막았습니다.  
    }
}

TEST(TestCppPattern, Restorer) {
    // 테스트용 속성 관리자
    class Manager {
        int m_Val;
    public:
        int GetVal() const {return m_Val;}
        void SetVal(int val) {m_Val = val;} 
    };

    // 속성값을 리턴하는 함수자
    class ManagerGetter { 
        const Manager& m_Manager; 
    public:
        explicit ManagerGetter(const Manager& manager) : 
            m_Manager(manager) {} 
        int operator ()() const {return m_Manager.GetVal();}
    };

    // 속성값을 세팅하는 함수자
    class ManagerSetter { 
        Manager& m_Manager; 
    public:
        explicit ManagerSetter(Manager& manager) : 
            m_Manager(manager) {} 
        void operator ()(int val) {m_Manager.SetVal(val);}
    }; 

    Manager manager;
    manager.SetVal(10); 

    {
        ManagerGetter getter(manager);
        ManagerSetter setter(manager);
        Restorer<ManagerGetter, ManagerSetter, int> managerRestorer(
            getter,
            setter,
            20
        );
        EXPECT_TRUE(manager.GetVal() == 20); // 20 으로 설정함
    } // Restore가 소멸되면서 원래값 복원  
    EXPECT_TRUE(manager.GetVal() == 10); // 원래값인 10으로 복원함
}