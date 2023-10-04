#include "gtest/gtest.h" 

namespace {

    template<typename T>
    class Allocator {
    public:
        typedef size_t    size_type;
        typedef ptrdiff_t difference_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef T         value_type;

        Allocator() {}
        Allocator(const Allocator&) {}

        // T를 n개 만큼 저장할 수 있는 메모리 영역을 할당합니다.
        pointer allocate(size_type n, const void * = 0) {
            T* t = (T*) malloc(n * sizeof(T));
            std::cout<<"Allocator::allocate()"<<std::endl;
            return t;
        }
    
        // allocate에서 할당한 메모리를 해제합니다.
        void deallocate(void* p, size_type) {
            free(p);
            std::cout<<"Allocator::deallocate()"<<std::endl;
        }

        pointer address(reference x) const {return &x;}
        const_pointer address(const_reference x) const {return &x;}
        Allocator<T>& operator =(const Allocator&) {return *this;}

        // p 메모리 위치에 val을 복사 생성합니다. 
        void construct(pointer p, const T& val) { 
            new (p) T(val);
        }
        // p 메모리 위치의 T 개체의 소멸자를 호출합니다.
        void destroy(pointer p) {
            p->~T();
        }

        // 지원되는 최대 할당 크기를 리턴합니다.
        size_type max_size() const {
            return std::numeric_limits<size_t>::max();
        }
    
        // T 타입에 대한 할당자(another)를 이용하여
        // 다른 타입인 U 타입용 컨테이너를 할당할 수 있게 합니다.
        // T::rebind<U>::other(another).allocate(10, static_cast<U*>(0));
        template<class U>
        struct rebind {typedef Allocator<U> other;};

        template<class U>
        Allocator(const Allocator<U>&) {}

        template<class U>
        Allocator& operator =(const Allocator<U>&) {return *this;}
    };
}


TEST(TestClassicCpp, Allocator) {
    {

        class A {
        public:
            A() {
                std::cout<<"A()"<<std::endl;
            }
            A(const A& other) {
                std::cout<<"A(const A& other)"<<std::endl;        
            }
            ~A() {
                std::cout<<"~A()"<<std::endl;     
            }
        };

        std::vector<A, Allocator<A>> v;

        v.push_back(A());
    }

}

