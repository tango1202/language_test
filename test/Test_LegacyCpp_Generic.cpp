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
}

TEST(TestLegacyCpp, Generic) {
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

}

