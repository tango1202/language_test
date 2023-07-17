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
    // ----
    // operator new
    // ---- 
    // 기본 구현
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
                void* buffer = malloc(sz);
                return buffer;
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
                return malloc(sz);
            }
            static void* operator new(std::size_t sz, int val) { // int val을 인자로 전달받습니다.
                return malloc(sz);
            } 
            static void operator delete(void* ptr) {
                free(ptr); // ptr == NULL 일 경우 아무 작업 안함
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
                return malloc(sz);
            }        
            static void operator delete(void* ptr) {
                std::cout<<"T::delete(void* ptr)"<<std::endl;
                free(ptr); 
            }
            // sz : 해제할 바이트 수
            static void operator delete(void* ptr, std::size_t sz) {
                std::cout<<"delete(void* ptr, std::size_t sz)"<<std::endl;
                free(ptr); 
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
                return malloc(sz);
            }
            // sz : sizeof(Base) 또는 sizeof(Derived) 크기
            static void operator delete(void* ptr, std::size_t sz) {
                free(ptr); 
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
                return malloc(sz);
            }
            // (X) 오동작. 소멸시에는 Base의 크기 : 8byte. 메모리 릭 발생
            static void operator delete(void* ptr, std::size_t sz) {
                free(ptr);  
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
                if (sz == 0) { // 혹시 모르니 검사
                    ++sz;
                }
                return malloc(sz);
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
                 return malloc(sz);
            }
            // sz : sizeof(T) * 배열 요수 갯수 + 오버헤드
            static void operator delete[](void* ptr, std::size_t sz) { 
                free(ptr);
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
 
            t->~T(); // 위치 지정 생성을 사용하면 명시적으로 소멸자를 호출해야 합니다.
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
                if (sz == 0) { 
                    ++sz;
                }
                return malloc(sz);
            }
            static void operator delete(void* ptr) {
                free(ptr);  
            }
            // 위치 지정 생성 재정의
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

            t->~T(); // 위치 지정 생성을 사용하면 명시적으로 소멸자를 호출해야 합니다.
            T::operator delete(buffer); // static 함수 호출하듯이 사용합니다.
        }

        {
            T* t = new T; // operator new(size_t sz, void* ptr) 가 호출되지는 않습니다.
            EXPECT_TRUE(t->GetX() == 10 && t->GetY() == 20);
            delete t;
        }
    }
}