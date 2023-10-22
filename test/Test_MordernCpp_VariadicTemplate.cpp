#include "gtest/gtest.h" 

namespace VariadicTemplate_1 {
    template<typename Type>
    Type Sum_11(Type param) {
        return param; // 재귀 호출하다가 마지막에 도달하면 호출됩니다.
    }
    // Types는 파라메터 팩입니다.
    template<typename Type, typename... Types>
    Type Sum_11(Type param, Types... params) {
        // 재귀 호출시 params의 첫번째 인자인 prams#1은 param으로 전달되고, 
        // 나머지 params#2 ~ params#N 은 params에 전달됩니다.
        return param + Sum_11(params...); 
    }    

    // func(params...) 를 호출합니다.
    template<typename Func, typename... Params>
    int Forwarding_11(Func func, Params... params) {
        return func(params...);
    }

    int MySum(int a, int b, int c) {
        return a + b + c;
    }

    template<typename... Types>
    int Func_11(Types... params) {
        return sizeof...(params);
    }

}

namespace VariadicTemplate_2 {
    int Sum(int a, int b, int c) {
        return a + b + c;
    }
    template<typename... Params>
    int Func_11(Params... params) {
        return Sum((params + 1)...); // 파라메터 팩의 각 요소에 1을 더해 배포합니다.
    }        
}
#if 201703L <= __cplusplus // C++17~
// (C++17~) Fold 표현식
namespace VariadicTemplate_3 {
    template<typename... Params>
    int UnaryRightFold_17(Params... params) {
        // (param[1] + (나머지 전개)) 
        // (param[1] + (param[2] + (나머지 전개)))
        // (param[1] + (param[2] + (param[3] + (나머지 전개))))
        // 즉, (param[1] op (... op (param[N-1] op param[N]))) 으로 전개 됩니다.        
        return (params +...); 
    }  
    // UnaryRightFold_17 과 동일하게 전개되나 마지막에 init 을 추가합니다.
    template<typename... Params>
    int BinaryRightFold_17(int init, Params... params) {
        // (param[1] + (나머지 전개 + val)) 
        // (param[1] + (param[2] + (나머지 전개 + val)))
        // (param[1] + (param[2] + (param[3] + (나머지 전개))))
        // 즉, (param[1] op (... op (param[N] op init))) 으로 전개 됩니다.        
        return (params +...+ init); 
    } 
    template<typename... Params>
    int UnaryLeftFold_17(Params... params) {
        // ((나머지 전개) + param[N]) 
        // (((나머지 전개) + param[N-1]) + param[N]) 
        // ((((나머지 전개) + param[N-2]) + param[N-1]) + param[N])         
        // 즉, (((param[1] op param[2]) op ...) op param[N]) 으로 전개 됩니다.        
        return (...+ params); 
    }   
    // UnaryLeftFold_17 과 동일하게 전개되나 마지막에 init 을 추가합니다.
    template<typename... Params>
    int BinaryLeftFold_17(int init, Params... params) {
        // ((나머지 전개) + param[N]) 
        // (((나머지 전개) + param[N-1]) + param[N]) 
        // ((((나머지 전개) + param[N-2]) + param[N-1]) + param[N])         
        // 즉, (((init op param[1]) op ...) op param[N]) 으로 전개 됩니다.        
        return (init +...+ params); 
    }   
}
#endif

TEST(TestMordern, VariadicTemplate) {
    {
        using namespace VariadicTemplate_1;
        int val{Sum_11(1, 3, 5)};
        EXPECT_TRUE(val == 1 + 3 + 5);  
    }

    {
        using namespace VariadicTemplate_1;
        EXPECT_TRUE(Forwarding_11(MySum, 1, 2, 3) == 1 + 2 + 3);       
    }
    {
        using namespace VariadicTemplate_1;
        EXPECT_TRUE(Func_11(1, 2, 3) == 3);
    }
    // 파라메터 팩 확장
    {
        using namespace VariadicTemplate_2;
        EXPECT_TRUE(Func_11(1, 2, 3) == 2 + 3 + 4);
    }
#if 201703L <= __cplusplus // C++17~    
    // (C++17~) Fold 표현식
    {
        using namespace VariadicTemplate_3;
        EXPECT_TRUE(UnaryRightFold_17(1, 2, 3) == 1 + 2 + 3);
        EXPECT_TRUE(BinaryRightFold_17(10, 1, 2, 3) == 1 + 2 + 3 + 10);  

        EXPECT_TRUE(UnaryLeftFold_17(1, 2, 3) == 1 + 2 + 3);
        EXPECT_TRUE(BinaryLeftFold_17(10, 1, 2, 3) == 10 + 1 + 2 + 3); 
    }
#endif    
}