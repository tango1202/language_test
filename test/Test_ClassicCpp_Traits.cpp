#include "gtest/gtest.h" 

#include <cmath>

namespace Traits_1 {

    template<typename T> 
    class ObjTraits {
    public:
        typedef T Type;
        typedef T* PointerType;
    };

    template<typename T> 
    class Runner {
        // T는 Type과 PointerType을 제공해야 합니다.
        typedef typename T::Type Type;
        typedef typename T::PointerType PointerType;
 
        Type m_Val1; // Type == T::Type
        typename T::Type m_Val2; // T::Type == Type
    };

    // Type과 PointerType을 제공하기 위해 ObjTraits를 사용합니다. 
    class A : public ObjTraits<int> {};

    // Type과 PointerType을 직접 제공합니다.
    class B {
    public:
        typedef int Type;
        typedef int* PointerType;
    };
}

namespace Traits_2 {

    // A와 B는 더하기시 올림을 해야 합니다.
    class A {
    public:
        double m_Val;
        explicit A(double val) : m_Val(val) {}    
    };
    class B {
    public:
        double m_Val;    
        explicit B(double val) : m_Val(val) {}
    }; 
    // C와 D는 더하기시 내림을 해야 합니다.
    class C {
    public:
        double m_Val;  
        explicit C(double val) : m_Val(val) {}  
    }; 
    class D {
    public:
        double m_Val;  
        explicit D(double val) : m_Val(val) {}   
    };

    template<typename T>
    int AddCeil(T val1, T val2) {
        return std::ceil(val1.m_Val + val2.m_Val);
    }
    template<typename T>
    int AddFloor(T val1, T val2) {
        return std::floor(val1.m_Val + val2.m_Val);
    }
}

namespace Traits_3 {

    // 올림, 내림 오버로딩을 위한 더미 클래스
    class CeilTag {};
    class FloorTag {};

    // A와 B는 더하기시 올림을 해야 합니다.
    class A {
    public:
        typedef CeilTag AddTag; // 올림
        double m_Val;
        explicit A(double val) : m_Val(val) {}    
    };
    class B {
    public:
        typedef CeilTag AddTag; // 올림
        double m_Val;    
        explicit B(double val) : m_Val(val) {}
    }; 
    // C와 D는 더하기시 내림을 해야 합니다.
    class C {
    public:
        typedef FloorTag AddTag; // 내림
        double m_Val;  
        explicit C(double val) : m_Val(val) {}  
    }; 
    class D {
    public:
        typedef FloorTag AddTag; // 내림
        double m_Val;  
        explicit D(double val) : m_Val(val) {}   
    };

    // T::AddTag에 따라 올림과 내림을 오버로딩을 통해 분기하여 호출합니다.
    template<typename T> 
    int Add(T val1, T val2) {
        typename T::AddTag addTag;
        return AddInternal(val1, val2, addTag);
    } 
    // 올림을 합니다.
    template<typename T>
    int AddInternal(T val1, T val2, CeilTag) {

        return std::ceil(val1.m_Val + val2.m_Val);
    }
    // 내림을 합니다.
    template<typename T>
    int AddInternal(T val1, T val2, FloorTag) {
        return std::floor(val1.m_Val + val2.m_Val);
    }
}

namespace Traits_4 {

    // 템플릿 특수화 컴파일을 위한 전방 선언
    class C;
    class D;

    // 올림, 내림 오버로딩을 위한 더미 클래스
    class CeilTag {};
    class FloorTag {};

    // 일반적으로 모든 타입은 올림을 사용합니다.
    template<typename T>
    class AddTraits {
    public:
        typedef CeilTag AddTag; 
    };
    // C와 D는 내림을 사용합니다.
    template<> 
    class AddTraits<C> {
    public:
        typedef FloorTag AddTag;         
    };
    template<> 
    class AddTraits<D> {
    public:
        typedef FloorTag AddTag;         
    };

    class A {
    public:
        double m_Val;
        explicit A(double val) : m_Val(val) {}    
    };
    class B {
    public:
        double m_Val;    
        explicit B(double val) : m_Val(val) {}
    }; 
    class C {
    public:
        double m_Val;  
        explicit C(double val) : m_Val(val) {}  
    }; 
    class D {
    public:
        double m_Val;  
        explicit D(double val) : m_Val(val) {}   
    };

    // AddTraits<T>::AddTag에 따라 올림과 내림을 오버로딩을 통해 분기하여 호출합니다.
    template<typename T> 
    int Add(T val1, T val2) {
        typename AddTraits<T>::AddTag addTag;
        return AddInternal(val1, val2, addTag);
    } 
    // 올림을 합니다.
    template<typename T>
    int AddInternal(T val1, T val2, CeilTag) {

        return std::ceil(val1.m_Val + val2.m_Val);
    }
    // 내림을 합니다.
    template<typename T>
    int AddInternal(T val1, T val2, FloorTag) {
        return std::floor(val1.m_Val + val2.m_Val);
    }
}


TEST(TestClassicCpp, Traits) {
    {
        using namespace Traits_1;

        Runner<A> a;
        Runner<B> b;
    }

    {
        using namespace Traits_2;

        A a1(1.1), a2(1.2);
        B b1(1.1), b2(1.5);
        C c1(1.1), c2(1.2);
        D d1(1.1), d2(1.5);

        // (△) 비권장 A, B, C, D 타입에 따라 사용자가 다른 함수를 호출해야 합니다.
        EXPECT_TRUE(AddCeil(a1, a2) == 3); // 올림
        EXPECT_TRUE(AddCeil(b1, b2) == 3); // 올림
        EXPECT_TRUE(AddFloor(c1, c2) == 2); // 내림
        EXPECT_TRUE(AddFloor(d1, d2) == 2); // 내림        
    }
    {
        using namespace Traits_3;

        A a1(1.1), a2(1.2);
        B b1(1.1), b2(1.5);
        C c1(1.1), c2(1.2);
        D d1(1.1), d2(1.5);

        // (O) A, B, C, D에 정의된 타입 특성으로 알아서 올림과 내림을 호출합니다.
        EXPECT_TRUE(Add(a1, a2) == 3); // 올림
        EXPECT_TRUE(Add(b1, b2) == 3); // 올림
        EXPECT_TRUE(Add(c1, c2) == 2); // 내림
        EXPECT_TRUE(Add(d1, d2) == 2); // 내림         
    }
    {
        using namespace Traits_4;

        A a1(1.1), a2(1.2);
        B b1(1.1), b2(1.5);
        C c1(1.1), c2(1.2);
        D d1(1.1), d2(1.5);

        // (O) AddTraits에 정의된 A, B, C, D 타입 특성에 따라 올림과 내림을 호출합니다.
        EXPECT_TRUE(Add(a1, a2) == 3); // 올림
        EXPECT_TRUE(Add(b1, b2) == 3); // 올림
        EXPECT_TRUE(Add(c1, c2) == 2); // 내림
        EXPECT_TRUE(Add(d1, d2) == 2); // 내림         
    }

}
