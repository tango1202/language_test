#include "gtest/gtest.h" 

TEST(TestClassicCpp, NewDelete) {
    // ----
    // new-delete
    // ----
    {
        class T {
        public:
            T() {
                std::cout<<"New-Delete Test : T::T()"<<std::endl;
            }
            ~T() {
                std::cout<<"New-Delete Test : T::~T()"<<std::endl;
            }
        };

        T* t = new T; // (O) 메모리 할당. 생성자 호출
        delete t; // (O) 소멸자 호출후. 메모리 해제

        T* arr = new T[3]; // (O) 메모리 할당(sizeof(T) * 3 + 오버헤드). 생성자 3회 호출
        delete[] arr; // (O) 소멸자 3회 호출. 메모리 해제(sizeof(T) * 3 + 오버헤드)
        // delete arr; // (X) 예외 발생. 소멸자가 1회만 호출되고, 프로그램이 다운될 수 있음
    }
    // ----
    // delete
    // ----
    {
        class T {};
        T* p = NULL;
        delete p; // (O) p가 NULL이어도 안전합니다.
        p = new T; 
        delete p; // (O) new로 생성한 것은 반드시 delete 해야 합니다.
        // delete p; // (X) 예외 발생. 두번 죽일 순 없습니다.
    }
    {
        class T {};
        T* t = new(std::nothrow) T; // (△) 비권장. 메모리 할당에 실패하면 널을 리턴하지만, 생성자에서 예외를 발생하면 전파됩니다.
        if (t == NULL) {
            // 할당 실패시 처리할 코드
        }
    }
    // ----
    // operator new 기본 재정의 방법
    // ---- 
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T() :
                m_X(10), 
                m_Y(20) {} 
            T(int x, int y) : 
                m_X(x),
                m_Y(y) {}
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}

            static void* operator new(std::size_t sz) {
                void* ptr = malloc(sz);
                if (ptr == NULL) {
                    throw std::bad_alloc(); // 할당 실패시 std::bad_alloc 방출
                }
                return ptr;
                // return ::operator new(sz); // 전역 operator new
            }
            static void operator delete(void* ptr) {
                free(ptr); // ptr == NULL 일 경우 아무 작업 안함
                // ::operator delete(ptr); // 전역 operator delete. ptr == NULL 일 경우 아무 작업 안함
            }
        }; 
        T* t = new T; // T::operator new(std::size_t sz), 기본 생성자 호출
        EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
        delete t; // T::operator delete(void* ptr)

        t = new T(1, 2); // T::operator new(std::size_t sz), T::T(int, int) 생성자 호출
        EXPECT_TRUE(t->GetX() == 1 && t->GetY() == 2);
        delete t; // T::operator delete(void* ptr) 
    }  
    // 사용자 정의
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T() :
                m_X(10), 
                m_Y(20) {} 
            T(int x, int y) : 
                m_X(x),
                m_Y(y) {}
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}

            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }
            static void* operator new(std::size_t sz, int val) { // int val을 인자로 전달받습니다.
                return ::operator new(sz);
            } 
            static void operator delete(void* ptr) {
                ::operator delete(ptr); // ptr == NULL 일 경우 아무 작업 안함
            }
        }; 

        T* t = new T; // T::operator new(std::size_t sz), 기본 생성자 호출
        EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
        delete t; 

        t = new(1) T; // T::operator new(std::size_t sz, int val), 기본 생성자 호출
        EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
        delete t; 

        t = new(1) T(1, 2); // T::operator new(std::size_t sz, int val), T::T(int, int) 생성자 호출
        EXPECT_TRUE(t->GetX() == 1 && t->GetY() == 2);
        delete t;
    }
    // operator delete
    {
        class T {
        public:
            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }        
            static void operator delete(void* ptr) {
                std::cout<<"T::delete(void* ptr)"<<std::endl;
                ::operator delete(ptr); 
            }
            // sz : 해제할 바이트 수
            static void operator delete(void* ptr, std::size_t sz) {
                std::cout<<"delete(void* ptr, std::size_t sz)"<<std::endl;
                ::operator delete(ptr); 
            }            
        }; 

        T* t = new T; 
        delete t; // T::delete(void* ptr) 호출
    }
    // 부모 클래스 operator new, delete
    {
        // sizeof(Base) == sizeof(int) * 2 + 가상 함수 테이블 크기
        class Base { 
            int m_X;
            int m_Y;
        public:
            virtual ~Base() {}

            // sz : sizeof(Base) 또는 sizeof(Derived) 크기
            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }
            // sz : sizeof(Base) 또는 sizeof(Derived) 크기
            static void operator delete(void* ptr, std::size_t sz) {
                ::operator delete(ptr); 
            } 
        };

        // sizeof(Derived) == sizeof(Base) + sizeof(int) * 2
        class Derived : public Base {
            int m_A;
            int m_B;
        public:
            virtual ~Derived() {}         
        };

        Base* base = new Derived; // Base::operator new(std::size_t sz) 호출
        delete base; // Base::delete(void* ptr, std::size_t sz) 호출
    }
    // 부모 클래스 non-Virual 소멸자
    {
        // sizeof(Base) == sizeof(int) * 2 = 8byte
        class Base { 
            int m_X;
            int m_Y;
        public:
            ~Base() {}

            // (X) 오동작. sizeof(Derived) 크기 : 16byte
            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }
            // (X) 오동작. 소멸시에는 Base의 크기 : 8byte. 메모리 릭 발생
            static void operator delete(void* ptr, std::size_t sz) {
                ::operator delete(ptr);  
            } 
        };

        // sizeof(Derived) == sizeof(Base) + sizeof(int) * 2 = 16byte
        class Derived : public Base {
            int m_A;
            int m_B;
        public:
            ~Derived() {}         
        };

        // Base* base = new Derived; // Base::operator new(std::size_t sz) 호출
        // delete base; // (X) 오동작.  Base의 소멸자가 호출되고 Base의 크기가 전달됨. Base::delete(void* ptr, std::size_t sz) 호출        
    }
    // 최소 1byte 할당
    {
        class T {
        public:
            static void* operator new(std::size_t sz) { // 1byte 전달됨
                if (sz == 0) { // 혹시 모르니 검사하여 최소 1byt로 만듬
                    ++sz;
                }
                void* ptr = malloc(sz);
                if (ptr == NULL) {
                    throw std::bad_alloc(); // 할당 실패시 std::bad_alloc 방출
                }
                return ptr;
            }
            static void operator delete(void* ptr) {
                free(ptr);  
            }            
        };

        EXPECT_TRUE(sizeof(T) == 1); // 빈 클래스도 1byte 할당됨
        T* t = new T;
        delete t;
    }
    // ----
    // new[] - delete[]
    // ----
    {
        class T {
        public:
            // sz : sizeof(T) * 배열 요수 갯수 + 오버헤드
            static void* operator new[](std::size_t sz) { 
                 return ::operator new[](sz);
            }
            // sz : sizeof(T) * 배열 요수 갯수 + 오버헤드
            static void operator delete[](void* ptr, std::size_t sz) { 
                ::operator delete[](ptr);
            } 
        };
        T* arr = new T[10]; // operator new[](std::size_t sz), sizeof(T) * 10 + 오버헤드
        delete[] arr; // operator delete[](void* ptr, std::size_t sz) 호출
    }
    // ----
    // placement New
    // ----
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T() : 
                m_X(10), 
                m_Y(20) {}
            T(int x, int y) :
                m_X(x), 
                m_Y(y) {}
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}
        };
        {
            void* buffer = malloc(sizeof(T)); // T 클래스 크기만큼 메모리를 할당합니다.

            T* t = new(buffer) T; // T 의 기본 생성자를 호출합니다.
            EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
 
            t->~T(); // Placement New를 사용하면 명시적으로 소멸자를 호출해야 합니다.
            free(buffer); // malloc 으로 할당한 메모리를 해제합니다.
        }
        {
            void* buffer = malloc(sizeof(T));
            T* t = new(buffer) T; // T 의 기본 생성자를 호출합니다.
            EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
            t->~T();

            // (O) 기존 t 위치에 other를 사용합니다.
            T* other = new(buffer) T(1, 2); // T의 값 생성자를 호출합니다. 
            EXPECT_TRUE(t->GetX() == 1 && t->GetY() == 2);

            // (△) 비권장. buffer를 직접 수정해도 동작하는지 확인합니다.
            // 테스트를 위한 것이니 실제 코드에서 이런거 하시면 캡슐화가 무너집니다.
            int* p = static_cast<int*>(buffer);
            *p = 100;
            *(p + 1) = 200;
            EXPECT_TRUE(other->GetX() == 100 && other->GetY() == 200);

            other->~T(); 
            // (△) 비권장. 소멸자를 호출하기는 했습니다만, 아직 buffer는 유효하여 other 접근이 되기는 합니다.
            EXPECT_TRUE(other->GetX() == 100 && other->GetY() == 200); 
            free(buffer);     
        } 
    }
    // operator new를 정의하면 전역 위치 지정 new가 가려지므로, 위치 지정 new 도 정의해야 함
    {
        class T {
            int m_X;
            int m_Y;
        public:
            T() : 
                m_X(10), 
                m_Y(20) {}
            T(int x, int y) :
                m_X(x), 
                m_Y(y) {}
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}

            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }
            static void operator delete(void* ptr) {
                ::operator delete(ptr);  
            }
            // Placement New 재정의
            static void* operator new(size_t sz, void* ptr) { 
                return ptr;
            }
        }; 
        {
            void* buffer = T::operator new(sizeof(T)); // static 함수 호출하듯이 사용합니다.
            T* t = new(buffer) T; // T 의 기본 생성자를 호출합니다.
            EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);

            // (△) 비권장. buffer를 직접 수정해도 동작하는지 확인합니다.
            // 테스트를 위한 것이니 실제 코드에서 이런거 하시면 캡슐화가 무너집니다.
            int* p = static_cast<int*>(buffer);
            *p = 100;
            *(p + 1) = 200;
            EXPECT_TRUE(t->GetX() == 100 && t->GetY() == 200);

            t->~T(); // Placement New를 사용하면 명시적으로 소멸자를 호출해야 합니다.
            T::operator delete(buffer); // static 함수 호출하듯이 사용합니다.
        }

        {
            T* t = new T; // operator new(size_t sz, void* ptr) 가 호출되지는 않습니다.
            EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
            delete t;
        }
    }
    // ----
    // 스택에만 생성되는 개체
    // ----
    {
        class T {
        private:
            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }    
        };

        T t; // (O)
        // T* p = new T; // (X) 컴파일 오류
        // delete p;           
    }
}
TEST(TestClassicCpp, NewHandler) {    
    // ----
    // set_new_handler
    // ----
    {
        // 생성시 new_handler를 설정하고, 소멸시 이전 new_handler로 복원합니다.
        class NewHandlerRestorer {
            std::new_handler m_OldHandler;
        public:
            explicit NewHandlerRestorer(std::new_handler handler) : 
                m_OldHandler(std::set_new_handler(handler)) {
                std::cout<<"----## NewHandlerRestorer : Start"<<std::endl;    
            }
            ~NewHandlerRestorer() {
                std::set_new_handler(m_OldHandler);
                std::cout<<"----## NewHandlerRestorer : End"<<std::endl;
            }
        private:
            // 복사 생성자, 대입 연산자 를 사용 못하게 막음
            NewHandlerRestorer(const NewHandlerRestorer& other);
            NewHandlerRestorer& operator =(const NewHandlerRestorer& other);
        };
        // 엄청 큰 데이터를 관리하는 클래스 입니다.
        class T {
            int m_Big[1024 * 1024 * 1000]; // 1000 M * sizeof(int)
        public:
            // 모드설정에 따라 new_handler가 변경됩니다.
            enum NewHandlerMode {UsingReserved, Another, Remove, BadAlloc, Abort};
        private:
            static NewHandlerMode& GetNewHandlerModeRef() {
                static NewHandlerMode s_Mode = T::BadAlloc; // 기본적으론 BadAlloc

                return s_Mode;
            }
            // s_Mode 에 맞게 NewHandler 리턴합니다.
            static std::new_handler GetNewHandler() {
                std::new_handler result = &T::BadAllocHandler;
                T::NewHandlerMode mode = T::GetNewHandlerModeRef();
                switch(mode) {
                case T::UsingReserved: result = &T::UsingReservedHandler; break;
                case T::Another: result = &T::AnotherHandler; break;
                case T::Remove: result = &T::RemoveHandler; break;
                case T::BadAlloc: result = &T::BadAllocHandler; break;
                case T::Abort: result = &T::AbortHandler; break;
                }
                return result;
            }
            // 미리 예약된 메모리 공간입니다. 
            // new 실패시 UsingReservedHandler 에서 해제하여 메모리 공간을 확보해 줍니다.
            class Reserved {
                T* m_Ptr;
            public:
                explicit Reserved() : 
                    m_Ptr(new T[2]) { // T 2개를 new 할 수 있는 공간을 미리 예약합니다.
                    std::cout<<"------## Reserved : Start"<<std::endl;          
                }
                ~Reserved() {
                    Release(); // 메모리를 해제합니다.
                } 
            private:
                // 복사 생성자, 대입 연산자 를 사용 못하게 막음
                Reserved(const Reserved& other);
                Reserved& operator =(const Reserved& other);     
            public:    
                // 예약된 공간을 해제하고, NULL로 초기화 합니다. 
                void Release() {
                    delete[] m_Ptr; 
                    if (m_Ptr != NULL) {
                        std::cout<<"------## Reserved : End"<<std::endl;
                    }
                    m_Ptr = NULL; 
                }
                // 예약된 공간이 있는지 없는지 리턴합니다.
                bool IsValid() const {return m_Ptr != NULL ? true : false;} 
            };
            static Reserved& GetReservedRef() {
                // new 실패시 해제하고 사용할 2개의 공간을 미리 할당해 둡니다.
                // 정적 지역 변수여서 함수 호출시 최초 1회 생성됩니다.
                static Reserved s_Reserved; 

                return s_Reserved;
            }        
            static void CreateReserved() {
                GetReservedRef(); // 최초 호출하여 메모리 공간을 예약합니다. 
            }

            // new 할당에 실패하면, 예약된 공간을 해제하여 메모리를 늘려줍니다.
            static void UsingReservedHandler() {
                std::cout<<"------## UsingReservedHandler"<<std::endl;
                // 예약된 공간이 있다면, 해제하고 재시도 하고,
                if (GetReservedRef().IsValid()) {
                    GetReservedRef().Release();
                }
                // 예약된 공간이 없다면 다른 new_handler를 설치합니다.
                else {
                    std::set_new_handler(&T::AnotherHandler);
                }
            }
            // 다른 new_handler를 설치합니다.
            static void AnotherHandler() {
                std::cout<<"------## AnotherHandler"<<std::endl;
                std::set_new_handler(&T::BadAllocHandler);
            }
            // new 처리자의 설치 제거합니다. 아마도 std::bad_alloc이 방출됩니다.
            static void RemoveHandler() {
                std::set_new_handler(NULL);
            }
            // bad_alloc으로 포기합니다.
            static void BadAllocHandler() {
                std::cout<<"------## BadAllocHandler"<<std::endl;
                throw std::bad_alloc();
            }
            // std::abort()로 프로그램 종료합니다.
            static void AbortHandler() {
                std::abort();
            }
        public:
            // new_handler 모드를 설정합니다.
            // mode : UsingReserved, Another, Remove, BadAlloc, Abort
            static void SetNewHandlerMode(NewHandlerMode mode) {
                GetNewHandlerModeRef() = mode;
            }

            static void* operator new(std::size_t sz) { 

                // 최초 new 요청시 메모리 영역을 예약합니다.
                T::CreateReserved();

                // 예외가 발생하거나 유효 범위가 벗어나면 NewHandlerRestorer 소멸자에서 이전 handler로 복원해 줍니다.
                NewHandlerRestorer restorer(T::GetNewHandler());

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

                // 포인터를 담고 있다가 유효 범위가 끝나면 소멸시킵니다.
                // new 가 성공하면 Holder 생성자에서 해당 포인터를 저장하고, 
                // 정상 종료나 예외발생에 따른 스택 풀기에서 소멸자가 호출됩니다.
                class Holder {
                    T* m_Ptr;
                public:
                    Holder(T* ptr) :
                        m_Ptr(ptr) {
                        std::cout<<"--## Holder : Start. operator new OK"<<std::endl;    
                    }
                    ~Holder() {
                        delete m_Ptr;
                        std::cout<<"--## Holder : End"<<std::endl;
                    }
                private:
                    // 복사 생성자, 대입 연산자 를 사용 못하게 막음
                    Holder(const Holder& other);
                    Holder& operator =(const Holder& other);
                };
                std::cout<<"## Recursive"<<std::endl;
                Holder t(new T);
                Recursive();
            }
        };
        {
            try {
                // new 실패시 UsingReserved -> Another -> BadAlloc 순으로 Handler를 변경합니다.
                T::SetNewHandlerMode(T::UsingReserved); 
                Tester::Recursive();
            }
            catch(std::bad_alloc& e) {
                std::cout<<"## [UsingReserved] catch(std::bad_alloc& e)"<<std::endl;    
            }
        }
        {
            // Handler 를 제거합니다. std::bad_alloc을 방출합니다.
            try {
                T::SetNewHandlerMode(T::Remove); 
                Tester::Recursive();
            }
            catch(std::bad_alloc& e) {
                std::cout<<"## [Remove] catch(std::bad_alloc& e)"<<std::endl;    
            }
        }
        {
            // 프로그램을 종료합니다.
            // T::SetNewHandlerMode(T::Abort);  
            // Tester::Recursive();
        }
    }
}