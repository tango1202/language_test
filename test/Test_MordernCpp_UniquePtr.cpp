#include "gtest/gtest.h" 

namespace UniquePtr_1 {
    class T {};
    class U {};
    void Func(std::unique_ptr<T> t, std::unique_ptr<U> u) {}
}

namespace UniquePtr_2 {
    // unique_ptr을 이용한 PImpl 구현
    // ----
    // 선언에서
    // ----
    class T {
        class Impl; // 포인터만 사용되므로 전방 선언으로 충분합니다.
        std::unique_ptr<Impl> m_Impl; // 스마트 포인터여서 소멸자에서 delete 합니다.

    public:
        T(int x, int y);
        T(const T& other); // 복사 생성자입니다.
        T(T&& other) = default; // 복사 생성자를 정의했기 때문에 이동 생성자가 암시적으로 정의되지 않습니다. 따라서, 명시적으로 정의합니다.

        bool IsValid() const;
        int GetX() const;
        int GetY() const;

        void SetX(int x);
        void SetY(int y);
    };
    // ----
    // 정의에서
    // ----
    class T::Impl {
    public:
        int m_X;
        int m_Y;
        Impl(int x, int y) : m_X(x), m_Y(y) {}
    }; 

    T::T(int x, int y) : m_Impl{std::unique_ptr<T::Impl>{new Impl{x, y}}} {}
    T::T(const T& other) : m_Impl{other.m_Impl ? new Impl(*other.m_Impl) : nullptr} {}

    bool T::IsValid() const {return m_Impl ? true : false;}
    int T::GetX() const {return m_Impl->m_X;}
    int T::GetY() const {return m_Impl->m_Y;}

    void T::SetX(int x) {m_Impl->m_X = x;}
    void T::SetY(int y) {m_Impl->m_Y = y;}
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
    // unique_ptr을 이용한 PImpl 구현
    {

        using namespace UniquePtr_2;

        T a{10, 20};
        EXPECT_TRUE(a.IsValid() && a.GetX() == 10 && a.GetY() == 20);

        T b{a}; // 복사 생성합니다.
        EXPECT_TRUE(b.IsValid() && b.GetX() == 10 && b.GetY() == 20);
        a.SetX(1);
        a.SetY(2);
        EXPECT_TRUE(a.IsValid() && a.GetX() == 1 && a.GetY() == 2); 
        EXPECT_TRUE(b.IsValid() && b.GetX() == 10 && b.GetY() == 20);

        T c{std::move(b)};
        EXPECT_TRUE(b.IsValid() == false); // 이동되어 Impl은 무효화되었습니다.
        EXPECT_TRUE(c.IsValid() && c.GetX() == 10 && c.GetY() == 20);
    }

#if 201402L <= __cplusplus // C++14~        
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
#endif    
}

