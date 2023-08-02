#include "gtest/gtest.h" 

namespace {
    // 템플릿 정의. 기존 int, char의 타입명이 T로 대체되어 코딩되었습니다.
    template<typename T>
    class Container {
        T m_Data[10];
    public:
        const T GetAt(size_t index) const {return m_Data[index];} 
        void SetAt(T data, size_t index) {m_Data[index] = data;} 
    };  

    // 템플릿 클래스 정의부 - 코드가 생성되지 않은 상태
    template<typename T> 
    class MyClass {}; 

    // 템플릿 함수 정의부 - 코드가 생성되지 않은 상태
    template<typename T>
    T MyFunc(T val) {return val + val;} 


    // 멤버 엑세스 지정
    template<typename T> 
    class A {
    private:
        typedef T TType;
    };  

    template<typename T> 
    class B : public T { // T로 A<int> 를 전달할 예정임. A<int>::TType은 int이나 private여서 접근 불가
        typename T::TType m_Member; // typename은 종속 타입 참고
    };
}
namespace Generic_1 {
    // > 
    // template<bool b = 3 > 4> // (X) 컴파일 오류
    // class A {};
    
    template<bool b = (3 > 4)> // (O)
    class B {};
}
namespace Generic_2 {
    template<typename T>
    class A {};

    template<typename T>
    using Alias = A<T>; // A의 별칭
}

TEST(TestClassicCpp, Generic) {
    {
        // class IContainer {
        // public:    
        //     virtual const void* GetAt(size_t index) const = 0;
        //     virtual void SetAt(void* data, size_t index) = 0; // (△) 비권장. 모든 포인터를 대충 담을 수 있는 void 포인터를 사용합니다.
        // };

        // class IntContainer : public IContainer {
        //     int m_Data[10];
        // public:
        //     virtual const int* GetAt(size_t index) const {return static_cast<const int*>(&m_Data[index]);} // 가상 함수의 리턴값은 int*로 변경할 수 있습니다.
        //     virtual void SetAt(void* data, size_t index) {m_Data[index] = *static_cast<int*>(data);} // (△) 비권장. 모든 포인터를 대충 담을 수 있는 void 포인터를 사용합니다.
        // };
        // class CharContainer : public IContainer {
        //     char m_Data[10];
        // public:
        //     virtual const char* GetAt(size_t index) const {return static_cast<const char*>(&m_Data[index]);} // 가상 함수의 리턴값은 char*로 변경할 수 있습니다.
        //     virtual void SetAt(void* data, size_t index) {m_Data[index] = *static_cast<char*>(data);} // (△) 비권장. 모든 포인터를 대충 담을 수 있는 void 포인터를 사용합니다.
        // };

        // IntContainer intContainer;
        // int i = 10;
        // intContainer.SetAt(&i, 5); // (△) 비권장. 포인터라서 사용하기 귀찮습니다.
        // EXPECT_TRUE(*intContainer.GetAt(5) == 10); // (△) 비권장. 포인터라서 사용하기 귀찮습니다.

        // CharContainer charContainer;
        // char c = 'c';
        // charContainer.SetAt(&c, 5);
        // EXPECT_TRUE(*charContainer.GetAt(5) == 'c');   
    }
    {
        // (△) 비권장. 훨씬 낫습니다. 그러나, 로직은 중복됩니다.
        class IntContainer {
            int m_Data[10];
        public:
            const int GetAt(size_t index) const {return m_Data[index];} 
            void SetAt(int data, size_t index) {m_Data[index] = data;} 
        };
        class CharContainer {
            char m_Data[10];
        public:
            virtual char GetAt(size_t index) const {return m_Data[index];}
            virtual void SetAt(char data, size_t index) {m_Data[index] = data;} 
        };

        IntContainer intContainer;
        int i = 10;
        intContainer.SetAt(i, 5);
        EXPECT_TRUE(intContainer.GetAt(5) == 10);

        CharContainer charContainer;
        char c = 'c';
        charContainer.SetAt(c, 5);
        EXPECT_TRUE(charContainer.GetAt(5) == 'c');   
    }  
    {
        // 템플릿 인스턴스화. T 대신 int 를 사용합니다.
        Container<int> intContainer;
        int i = 10;
        intContainer.SetAt(i, 5);
        EXPECT_TRUE(intContainer.GetAt(5) == 10);

        // 템플릿 인스턴스화. T 대신 char를 사용합니다.
        Container<char> charContainer;
        char c = 'c';
        charContainer.SetAt(c, 5);
        EXPECT_TRUE(charContainer.GetAt(5) == 'c');           
    }  
    {
        // 템플릿 인스턴스부 - 이때 코드가 생성됨
        MyClass<int> myClass;   

        // 템플릿 함수 인스턴스부 - 이때 코드가 생성됨
        int val = MyFunc<int>(10);      
    }   
    {
        // B<A<int>> b; // (X) 컴파일 오류. 템플릿 인스턴스화시 private 멤버 접근 오류 발생
    }  
    // 템플릿 별칭 
    {
        using namespace Generic_2;
    
        Alias<int> alias;
    }  
}

