#include "gtest/gtest.h" 

namespace {
    #define BOOL int
    #define TRUE 1
    #define FALSE 0

    // template의 타입 재정의
    template<class T> 
    struct ClassT { 
        typedef T Type;
        typedef const T ConstType; 
    }; 
}

TEST(TestClassicCpp, Type) {
    EXPECT_TRUE(sizeof(wchar_t) == 2);  
    EXPECT_TRUE(sizeof(long double) == 16);  
   
    EXPECT_TRUE(sizeof(BOOL) == 4);

    BOOL b = TRUE;
    EXPECT_TRUE(!b == FALSE); // !b == false이고 false를 int로 캐스팅하면 FALSE(0)

    b = FALSE;
    EXPECT_TRUE(!b == TRUE); // !b == true이고 true를 int로 캐스팅하면 TRUE(1)
    
    // b = FALSE;
    // EXPECT_TRUE(~b == FALSE); // (X) ~b 는 0xfffffffe 입니다.
}
TEST(TestClassicCpp, TypeDef) {
    typedef struct {int a; int b;} MyData, *pMyData;
    MyData myData1;
    MyData* myData2;
    pMyData myData3;

    ClassT<int>::ConstType constVal = 20;  
}

TEST(TestClassicCpp, TypeLimit) {
    EXPECT_TRUE(std::numeric_limits<int>::max() == 2147483647);
    EXPECT_TRUE(std::numeric_limits<int>::min() == -2147483648);
}

TEST(TestClassicCpp, SizeOf) {
    class MyClass {
        int i;
        int j;
    };

    MyClass myClass;
    MyClass& myClassRef = myClass;

    EXPECT_TRUE(sizeof(myClass) == 8);
    EXPECT_TRUE(sizeof(myClass) == sizeof(myClassRef));
}