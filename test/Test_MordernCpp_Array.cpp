#include "gtest/gtest.h" 

namespace {
    // 생성/소멸 테스트
    void CStyleArrayConstruct() {
        for(int i{0}; i < 100000; ++i) {
           int arr[100000];
        }        
    }
    void STLArrayConstruct() {
        for(int i{0}; i < 100000; ++i) {
            std::array<int, 100000> arr;
        }        
    }
     void STLVectorConstruct() {
        for(int i{0}; i < 100000; ++i) {
            std::vector<int> arr(100000);
        }        
    }      
    // 접근 테스트
    void CStyleArrayAccess() {
        int arr[100000];

        for(int i{0}; i < 100000; ++i) {
            arr[i] = i;
        }        
    }
    void STLArrayAccess() {
        std::array<int, 100000> arr;

        for(int i{0}; i < 100000; ++i) {
            arr[i] = i;
        }        
    }
     void STLVectorAccess() {
        std::vector<int> arr(100000);

        for(int i{0}; i < 100000; ++i) {
            arr[i] = i;
        }        
    }  

    template<typename Func>
    std::chrono::microseconds Measure(Func f) {
        std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};    
 
        f();

        std::chrono::system_clock::time_point end{std::chrono::system_clock::now()};
        std::chrono::microseconds val{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        return val;
    }
}

TEST(TestMordern, Array) {
    {
        std::array<int, 3> a{1, 2, 3};
        std::array<int, 3> b{4, 5}; 
        std::array<int, 2> c{4, 5};   
        
        // int* ptr = a; // (X) 컴파일 오류. array는 포인터로 암시적 변환이 되지 않습니다.
        int* ptr = a.data(); // data를 이용하면 포인터로 변환됩니다.  
        
        EXPECT_TRUE(b[0] == 4 && b[1] == 5 && b[2] == 0); // 요소의 갯수보다 적게 초기화하면 나머지 요소는 0으로 초기화 됩니다.

        // c = a; // (X) 컴파일 오류. 요소의 갯수가 다르면 컴파일 오류가 발생합니다.
        b = a; // 요소의 갯수가 같으면 대입이 가능합니다.
        EXPECT_TRUE(b[0] == 1 && b[1] == 2 && b[2] == 3);
    }

    {
        // !! 테스트 시간이 오래걸려 주석
        // 생성/소멸 테스트
        // std::cout << "CStyleArray : " << Measure(CStyleArrayConstruct).count() << std::endl;
        // std::cout << "STLArray : " << Measure(STLArrayConstruct).count() << std::endl;
        // std::cout << "STLVector : " << Measure(STLVectorConstruct).count() << std::endl;

        // 접근 테스트
        std::cout << "CStyleArray : " << Measure(CStyleArrayAccess).count() << std::endl;
        std::cout << "STLArray : " << Measure(STLArrayAccess).count() << std::endl;
        std::cout << "STLVector : " << Measure(STLVectorAccess).count() << std::endl;
    }

}