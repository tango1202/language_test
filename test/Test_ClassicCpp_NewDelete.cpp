#include "gtest/gtest.h" 

TEST(TestClassicCpp, NewDelete) {
    // ----
    // new-delete
    // ----
    {
        class T {
        public:
            T() {
                std::cout << "New-Delete Test : T::T()" << std::endl;
            }
            ~T() {
                std::cout << "New-Delete Test : T::~T()" << std::endl;
            }
        };

        T* t = new T; // (O) 메모리 할당. 생성자 호출
        delete t; // (O) 소멸자 호출. 메모리 해제

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
    // new(nothrow) 와 무의미한 널검사
    {
        class T {};
        T* t = new(std::nothrow) T; // (△) 비권장. 메모리 할당에 실패하면 널을 리턴하지만, 생성자에서 예외를 발생시키면 전파됩니다.
        if (t == NULL) {
            // 할당 실패시 처리할 코드
        }
    }
    {
        class T {
        public:
            T() {
                throw "My Error"; // 생성자에서 예외를 발생시킵니다.
            }
        };

        try {
            T* t = new(std::nothrow) T; // (△) 비권장. 메모리 할당에 실패하면 널을 리턴하지만, 생성자에서 예외를 발생시키면 전파됩니다.
            if (t == NULL) {
                // 할당 실패시 처리할 코드
            }
        }
        catch (const char* e) {
            // 생성자에서 예외를 발생할 경우 처리할 코드
            std::cout << e << std::endl; // My Error 출력
        }
    }
    {
        class T {
        public:
            T() {
                throw "My Error"; // 생성자에서 예외를 발생시킵니다.
            }
        };

        try {
            T* t = new T; 
        }
        catch (std::bad_alloc& e) {
            // 할당 실패시 처리할 코드
        }
        catch (const char* e) {
            // 생성자에서 예외를 발생할 경우 처리할 코드
            std::cout << e << std::endl; // My Error 출력
        }
    }    
    // ----
    // operator new 기본 재정의 방법
    // ---- 
    // 전역 operator new
    {
        class T {
        public:
            static void* operator new(std::size_t sz) {
                return ::operator new(sz); // 전역 operator new
            }
            static void operator delete(void* ptr) {
                ::operator delete(ptr); // 전역 operator delete
            }
        };
    }
    // malloc
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

            // sz는 오버헤드 공간을 제외한 크기가 전달됩니다.
            static void* operator new(std::size_t sz) { // #1
                // 혹시 모르니 검사하여 최소 1byte로 만듬    
                if (sz == 0) { 
                    ++sz;
                }

                // 핸들러가 예외를 발생시키거나 프로그램을 종료할 때까지 반복
                while (true) {
                    void* ptr = malloc(sz); // #1. 오버헤드 공간을 포함하여 할당합니다.
                    if (ptr != NULL) {
                        return ptr; // 성공적으로 할당했다면 리턴
                    }

                    std::new_handler handler = std::set_new_handler(NULL); // #2. 대충 NULL을 세팅하고 핸들러를 구합니다.
                    std::set_new_handler(handler); // #2. 핸들러 복원
                    // 핸들러가 있다면 실행
                    if (handler != NULL) {
                        handler(); // 핸들러가 발생시킨 예외 전파
                    }
                    // 핸들러가 없다면 std::bad_alloc
                    else {
                        throw std::bad_alloc();
                    }
                }
                return NULL;
            }
            static void operator delete(void* ptr) {
                free(ptr); // ptr == NULL 일 경우 아무 작업 안함
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
                std::cout << "T::delete(void* ptr)" << std::endl;
                ::operator delete(ptr); 
            }
            // sz : 해제할 byte 수
            static void operator delete(void* ptr, std::size_t sz) {
                std::cout << "delete(void* ptr, std::size_t sz)" << std::endl;
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
            virtual ~Base() {} // virtual 입니다.

            // sz : sizeof(Base) 또는 sizeof(Derived) 크기
            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }
            // sz : 소멸자가 virtual이어서 sizeof(Base) 또는 sizeof(Derived) 크기
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
            ~Base() {} // (X) 오동작. virtual이 아닙니다.

            // sizeof(Derived) 크기 : 16byte
            static void* operator new(std::size_t sz) {
                return ::operator new(sz);
            }
            // (X) 오동작. 소멸자가 virtual이 아니어서 Base의 크기인 8byte만 전달됩니다. 메모리 릭 발생
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
    // ----
    // new[] - delete[]
    // ----
    {
        class T {
            int m_Val;
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
        T* arr = new T[3]; // operator new[](std::size_t sz), sizeof(T) * 3 + 오버헤드
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
        public:
            static void* operator new(std::size_t sz) { // operator new를 정의하면 전역 위치 지정 생성을 가립니다.
                return ::operator new(sz);
            }
        }; 

        void* buffer = T::operator new(sizeof(T));
        // T* t = new(buffer) T; // (X) 컴파일 오류. 전역 위치 지정 생성이 가려졌습니다.
    }
    {
        class T {
        public:
            static void* operator new(std::size_t sz) { // operator new를 정의하면 전역 위치 지정 생성을 가립니다.
                return ::operator new(sz);
            }

            // Placement New 재정의
            static void* operator new(size_t sz, void* ptr) { // (O) 위치 지정 생성을 정의했습니다.
                return ptr;
            }
        }; 

        void* buffer = T::operator new(sizeof(T));
        T* t = new(buffer) T; // (O) 위치 지정 생성을 호출합니다.       
    }

    // ----
    // 스택에만 생성되는 개체
    // ----
    {
        class T {
        private:
            static void* operator new(std::size_t sz) {
                return NULL;
            }    
        };

        T t; // (O)
        // T* p = new T; // (X) 컴파일 오류
        // delete p;           
    }
    // ----
    // 힙에만 생성되는 개체
    // ----    
    {
        class T {
        protected: 
            ~T() {} // 외부에서 암시적으로 소멸할 수 없습니다.
        public:
            void Destroy() const {delete this;} // 자기 자신을 소멸시킵니다.
        };
        {
            // T t; // (X) 컴파일 오류. 스택에 만들고 암시적으로 소멸될 때 컴파일 오류가 발생합니다.
        }  
        {
            T* p = new T;
            // delete p; // (X) 컴파일 오류. delete는 외부에서 호출할 수 없습니다.
            p->Destroy(); // (O)
        }
    }
}
TEST(TestClassicCpp, NewHandler) { 
    // ----
    // new_handler
    // ----   
    {
        class MyException : public std::bad_alloc {}; // #2

        // 엄청 큰 데이터를 관리하는 클래스 입니다.
        class T { // #1
            int m_Big[1024 * 1024 * 1000]; // 1000 M * sizeof(int)
        public:
            // bad_alloc으로 포기합니다.
            static void MyExceptionHandler() { // #2
                throw MyException();
            }
            static void* operator new(std::size_t sz) { 

                // 내부적으로는 메모리를 할당하고, 실패하면 new_handler를 실행하는 과정을 무한히 반복합니다.
                // new_handler가 NULL 이거나 std::bad_alloc이나 std::abort()를 할때 까지요.
                return ::operator new(sz);
            } 
        };

        std::new_handler oldHandler = std::set_new_handler(&T::MyExceptionHandler); // #3. 핸들러를 설치합니다.

        // !! 테스트 속도가 느려 임시로 막습니다. 
        // T* arr[100] = {}; // 모두 NULL(0)로 초기화
        // for (int i = 0; i < 100; ++i) { // 대략 예외를 발생시킬때까지 반복합니다. 5 ~ 6개정도 할당하면 발생합니다.
        //     try {
        //         arr[i] = new T; // #4
        //     }
        //     catch (MyException& e) {
        //         // i 이전까지 생성된 T 개체를 모두 삭제합니다.
        //         for (int j = 0; j < i; ++j) {
        //             delete arr[j]; // #4
        //         }
        //         break; // T 할당을 그만두기 위해 for문을 탈출합니다.
        //     }
        // }

        std::set_new_handler(oldHandler); // #3. 이전 핸들러로 복원합니다.
    }
}

