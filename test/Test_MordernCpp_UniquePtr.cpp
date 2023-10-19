#include "gtest/gtest.h" 

namespace {
    class T {};
    class U {};
    void Func(std::unique_ptr<T> t, std::unique_ptr<U> u) {}
}

TEST(TestMordern, UniquePtr) {
    {
        class T {
        public:
            ~T() {std::cout << "T : Destructor" << std::endl;}    
        };

        std::unique_ptr<T> a{new T}; // 단일 개체. delete를 호출하여 1개가 소멸됩니다.
        std::unique_ptr<T[]> b{new T[2]}; // 배열 개체. delete[] 를 호출하여 2개가 소멸됩니다.

        std::unique_ptr<int> c{new int{0}};
        std::unique_ptr<int> d{new int{1}};

        // 소유권 이전으로 c는 d의 값을 갖고, d는 nullptr 이 됩니다.
        // c = d; // (X) 컴파일 오류.
        c = std::move(d); // 소유권 이전시 이동 연산을 사용합니다.
        EXPECT_TRUE(*c == 1 && d == nullptr);
    }
    // unique_ptr을 컨테이너 요소로 사용하기
    {
        std::vector<std::unique_ptr<int>> v;
        std::unique_ptr<int> a{new int{10}};
        std::unique_ptr<int> b{new int{20}};

        // v.push_back(a); // (X) 컴파일 오류. unique_ptr은 복사 생성자가 없습니다.
        // v.push_back(b); // (X) 컴파일 오류. unique_ptr은 복사 생성자가 없습니다.

        // move를 이용하여 이동시킵니다.
        v.push_back(std::move(a)); // (O) 
        v.push_back(std::move(b)); // (O) 

        // 요소 개체(std::unique_ptr<int>)를 생성할 인자(int*)만 전달하고, 내부에서 개체(std::unique_ptr<int>)를 생성합니다.
        v.emplace_back(new int{30});

        EXPECT_TRUE(*v[0] == 10 && *v[1] == 20 && *v[2] == 30);
       
    }
    // std::default_delete
    {
        std::unique_ptr<int> a{new int{10}, std::default_delete<int>{}};
    }
    // C++14 make_unique
    {
        class T {};
        std::unique_ptr<T> a{new T}; 
        std::unique_ptr<T> b{new T[2]}; // (△) 비권장. unique_ptr<T[]>인데, 실수로 []을 빼먹었지만 컴파일 됩니다.
        //std::unique_ptr<T> c{std::make_unique<T>(2)};  // (X) 컴파일 오류
        std::unique_ptr<T[]> d{std::make_unique<T[]>(2)};
    }
    {
        Func(std::unique_ptr<T>{new T}, std::unique_ptr<U>{new U}); // (△) 비권장. new T, new U 호출 순서에 따라 예외가 발생합니다.
        Func(std::make_unique<T>(), std::make_unique<U>()); // (O) 
    }
    
}

