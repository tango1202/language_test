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

    // new시 오류가 발생하면 재시도시 오류가 발생하지 않도록 처리하는 개체
    class NewHandler {

    public:
        // 생성시 NewHandler를 설정하고, 소멸시 이전 new_handler로 복원합니다.
        class Restorer : 
            private Uncopyable,
            private OnlyStackAssignable {
            std::new_handler m_OldHandler;
        public:
            explicit Restorer() : 
                m_OldHandler(std::set_new_handler(GetInstance().GetHandler())) {
            }
            ~Restorer() {
                std::set_new_handler(m_OldHandler);
            }
        };
    private:
        // 미리 예약된 메모리 공간입니다. 
        // new 실패시 UsingReservedHandler 에서 해제하여 메모리 공간을 확보해 줍니다.
        class Reserved : 
            private Uncopyable {
            char* m_Ptr;
        public:
            explicit Reserved(size_t size = 0) : 
                m_Ptr(NULL) { 
                Reset(size);          
            }
            ~Reserved() {
                Release(); // 메모리를 해제합니다.
            } 
            void Reset(size_t size) {
                Release();
                if (0 < size) {
                    m_Ptr = new char[size];
                }
            }
        public:    
            // 예약된 공간을 해제하고, NULL로 초기화 합니다. 
            void Release() {
                delete[] m_Ptr; 
                if (m_Ptr != NULL) {
                }
                m_Ptr = NULL; 
            }
            // 예약된 공간이 있는지 없는지 리턴합니다.
            bool IsValid() const {return m_Ptr != NULL ? true : false;} 
        }; 
    public:    
        // 모드설정에 따라 new_handler가 변경됩니다.
        enum Mode {UsingReserved, Another, Remove, BadAlloc, Abort}; 
    private:      
        Mode m_Mode; 
        Reserved m_Reserved;
    public:

    private:
        explicit NewHandler(Mode mode, size_t reservedSize) : 
            m_Mode(mode),
            m_Reserved(reservedSize) {}
    public:
        // 전역적으로만 생성됩니다. BadAlloc을 사용하며 예약 공간은 0입니다.
        static const NewHandler& GetInstance() {
            static NewHandler s_NewHandler(BadAlloc, 0);
            return s_NewHandler;
        }
        static NewHandler& GetInstanceRef() {
            return const_cast<NewHandler&>(GetInstance());
        }
    public:
        // mode : UsingReserved, Another, Remove, BadAlloc, Abort
        Mode GetMode() const {return m_Mode;}
        void SetMode(Mode mode) {m_Mode = mode;} 
        void ResetReserved(size_t size) {
            m_Reserved.Reset(size); // 메모리 예약 공간을 재할당합니다. 
        }
    private:    
        // m_Mode 에 맞게 Handler 리턴합니다.
        std::new_handler GetHandler() const {
            std::new_handler result = &BadAllocHandler;
            switch(GetMode()) {
            case UsingReserved: result = &UsingReservedHandler; break;
            case Another: result = &AnotherHandler; break;
            case Remove: result = &RemoveHandler; break;
            case BadAlloc: result = &BadAllocHandler; break;
            case Abort: result = &AbortHandler; break;
            }
            return result;
        }
    private:            
        // new 할당에 실패하면, 예약된 공간을 해제하여 메모리를 늘려줍니다.
        static void UsingReservedHandler() {
            // 예약된 공간이 있다면, 해제하고 재시도 하고,
            if (GetInstance().m_Reserved.IsValid()) {
                GetInstanceRef().m_Reserved.Release();
            }
            // 예약된 공간이 없다면 다른 new_handler를 설치합니다.
            else {
                std::set_new_handler(&AnotherHandler);
            }
        }
        // 다른 new_handler를 설치합니다.
        static void AnotherHandler() {
            // [Todo] 다른 처리 방법이 있다면 시도합니다.
            std::set_new_handler(&BadAllocHandler);
        }
        // new 처리자의 설치 제거합니다. 아마도 std::bad_alloc이 발생됩니다.
        static void RemoveHandler() {
            std::set_new_handler(NULL);
        }
        // bad_alloc으로 포기합니다.
        static void BadAllocHandler() {
            throw std::bad_alloc();
        }
        // std::abort()로 프로그램 종료합니다.
        static void AbortHandler() {
            std::abort();
        }
    };
}

TEST(TestCppPattern, NewHandler) {

    // ----
    // set_new_handler
    // ----
    {

        // 엄청 큰 데이터를 관리하는 클래스 입니다.
        class T {
            int m_Big[1024 * 1024 * 1000]; // 1000 M * sizeof(int)
        public:
            static void* operator new(std::size_t sz) { 

                // 예외가 발생하거나 유효 범위가 벗어나면 NewHandler::Restorer 소멸자에서 이전 handler로 복원해 줍니다.
                NewHandler::Restorer restorer;

                // 내부적으로는 메모리를 할당하고, 실패하면 handler를 실행하는 과정을 무한히 반복합니다.
                // handler가 std::bad_alloc이나 std::abort()를 할때 까지요.
                std::cout<<"----## ::operator new : Start"<<std::endl;  
                void* ptr = ::operator new(sz); 
                std::cout<<"----## ::operator new : End"<<std::endl; 
                return ptr;
            } 
        };
        class Tester {
        public:    
            // new가 실패할때까지 반복해서 재귀 할당 합니다.
            // Holder에 생성된 개체를 담습니다.
            static void Recursive() {
                std::cout<<"## Recursive"<<std::endl;
                Holder<T> holder(new T);
                Recursive();
            }
        };

        // 최초 사용시 메모리 영역을 예약합니다.
        NewHandler::GetInstanceRef().ResetReserved(sizeof(T) * 2);
        {
            try {
                // new 실패시 UsingReserved -> Another -> BadAlloc 순으로 Handler를 변경합니다.
                NewHandler::GetInstanceRef().SetMode(NewHandler::UsingReserved); 
                Tester::Recursive();
            }
            catch(std::bad_alloc& e) {
                std::cout<<"## [UsingReserved] catch(std::bad_alloc& e)"<<std::endl;    
            }
        }
        {
            // Handler 를 제거합니다. std::bad_alloc을 발생시킵니다.
            try {
                NewHandler::GetInstanceRef().SetMode(NewHandler::Remove); 
                Tester::Recursive();
            }
            catch(std::bad_alloc& e) {
                std::cout<<"## [Remove] catch(std::bad_alloc& e)"<<std::endl;    
            }
        }
        {
            // 프로그램을 종료합니다.
            // NewHandler::GetInstanceRef().SetMode(NewHandler::Abort);  
            // Tester::Recursive();
        }
    }
}