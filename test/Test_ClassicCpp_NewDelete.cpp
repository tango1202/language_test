#include "gtest/gtest.h" 
#include <new>

//void * operator new(size_t, void* p) {return p}; 

void* operator new[](std::size_t sz)
{
    std::printf("2) new[](size_t), size = %zu\n", sz);
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success
 
    if (void *ptr = std::malloc(sz))
        return ptr;
 
    throw std::bad_alloc{}; // required by [new.delete.single]/3
}
void operator delete[](void* ptr, std::size_t size) noexcept {
    // std::puts("5) delete[](void* ptr)");
    std::free(ptr);
}  

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
    // operator new
    // ----     
    {
        class T {
            int m_X;
            int m_Y;
            int m_Z;
        public:
            T() :
                m_X(10), 
                m_Y(20) {} 
            T(int x, int y) : 
                m_X(x),
                m_Y(y) {}
            int GetX() const {return m_X;}
            int GetY() const {return m_Y;}

            void* operator new(std::size_t sz) {
                void* buffer = malloc(sz);
                return buffer;

                // return ::operator new(sz); // 전역 new
            }

            void* operator new(std::size_t sz, int) {
                void* buffer = malloc(sizeof(T));
                return buffer;
            }   

            // delete는 두가지 유형이 있음. 둘중에 하나만 유효
            void operator delete(void* ptr) {
                free(ptr);
                // ::operator delete(ptr); // 전역 delete
            }

            // 할당 취소할 바이트 수
            // 두 번째 양식은 기본 클래스의 함수를 operator delete 사용하여 파생 클래스의 개체를 소멸시키는 경우에 유용합니다.                       
            void operator delete(void* ptr, std::size_t size) {
                free(ptr);
            }

            void* operator new[](std::size_t sz) { // sizeof(T) * 갯수 + overhead(8byte)
                // std::printf("2) new[](size_t), size = %zu\n", sz);
                //if (sz == 0)
                //    ++sz; // avoid std::malloc(0) which may return nullptr on success
            
                if (void *ptr = std::malloc(sz))
                    return ptr;
            
                throw std::bad_alloc{}; // required by [new.delete.single]/3
            }
            void operator delete[](void* ptr, std::size_t size) noexcept {
                // std::puts("5) delete[](void* ptr)");
                std::free(ptr);
            }  
        };

        T* t = new T; // T::operator new(std::size_t sz), 기본 생성자 호출
        delete t;

        t = new(1) T; // operator new(std::size_t sz, int), 기본 생성자 호출
        delete t;

        t = new(1) T(1, 2); // operator new(std::size_t sz, int), T::T(int, int) 생성자 호출
        delete t;

        T* arr = new T[10]; // operator new[](std::size_t sz), sizeof(T) * 10 + overhead
        delete[] arr;

        int* arr2 = new int[10];
        delete[] arr2;
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
 
            t->~T(); // Placement new를 사용하면 명시적으로 소멸자를 호출해야 합니다.
            free(buffer); // malloc 으로 할당한 메모리를 해제합니다.
        }
        {
            void* buffer = malloc(sizeof(T));
            T* t = new(buffer) T(1, 2); // T의 값 생성자를 호출합니다. 
            EXPECT_TRUE(t->GetX() == 1 && t->GetY() == 2);

            // (△) 비권장. buffer를 직접 수정해도 잘 동작합니다.
            // 테스트를 위한 것이니 실제 코드에서 이런거 하시면 개체 지향 무너집니다.
            int* p = static_cast<int*>(buffer);
            *p = 100;
            *(p + 1) = 200;
            EXPECT_TRUE(t->GetX() == 100 && t->GetY() == 200);

            t->~T(); 
            // (△) 비권장. 소멸자를 호출하기는 했습니다만, 아직 buffer는 유효하여 t 접근이 되기는 합니다.
            EXPECT_TRUE(t->GetX() == 100 && t->GetY() == 200); 
            free(buffer);     
        } 
    }
}