#include "gtest/gtest.h" 

#include <algorithm>

#if 202002L <= __cplusplus // C++20~  
#include <ranges>
namespace Ranges_1 {
    template<std::ranges::range R> // R은 range이어야 합니다.
    class MyView : public std::ranges::view_interface<MyView<R>> {
    private:
        std::ranges::iterator_t<R> m_Begin;
        std::ranges::iterator_t<R> m_End;
    public:
        // 컴파일 타임에도 동작할 수 있도록 constexpr 생성자로 만듭니다.
        // 복사 되지 않게 참조 타입으로 전달받습니다.
        constexpr explicit MyView(R& range) : 
            m_Begin{std::ranges::begin(range)},
            m_End{std::ranges::end(range)} {}
        constexpr auto begin() {return m_Begin;}
        constexpr auto end() {return m_End;}
    };
}
#endif

TEST(TestModern, Ranges) {

#if 202002L <= __cplusplus // C++20~  
    // 개요
    {
        class T {
        public:
            int begin() const {return 0;} // int 타입은 이터레이팅 할 수 없습니다.
            int end() const {return 0;} // int 타입은 이터레이팅 할 수 없습니다.
        };
        class U {
        public:
            int* begin() const {return nullptr;} // int* 타입은 이터레이팅 할 수 있습니다.
            int* end() const {return nullptr;} // int* 타입은 이터레이팅 할 수 있습니다.
        };         

        // static_assert(std::ranges::range<int>); // (X) 컴파일 오류. int 타입은 이터레이팅 할 수 없습니다.
        static_assert(std::ranges::range<int[3]>); // (O) 배열
        static_assert(std::ranges::range<std::vector<int>>); // (O) STL 컨테이너
        // static_assert(std::ranges::range<T>); // (X) 컴파일 오류. begin - end에서 int를 리턴하므로 이터레이팅 할 수 없습니다.
        static_assert(std::ranges::range<U>); // (O) 이터레이팅 할 수 있는 개체
    }

    {
        class A {
        private:
            int m_Val;
        public:
            explicit A(int val) : m_Val{val} {}
            A(const A& other) : m_Val(other.m_Val) {
                std::cout << "Range A : Copy Constructor" << std::endl;
            }
            A(A&&) = delete;
            A& operator =(const A&) = delete;
            A& operator =(A&&) = delete;

            int GetVal() const {return m_Val;}
        };

        std::vector<A> v{A{1}, A{2}, A{3}}; // Copy Constructor 3회 호출합니다.
        auto r{std::views::reverse(v)}; // 역순으로 바꿔서 보여주는 view입니다. 이때 요소 복제를 하지 않습니다.
        
        EXPECT_TRUE(v[0].GetVal() == 1 && v[1].GetVal() == 2 && v[2].GetVal() == 3);
        EXPECT_TRUE(r[0].GetVal() == 3 && r[1].GetVal() == 2 && r[2].GetVal() == 1); // 역순으로 탐색됩니다.
    }

    // 포인트 개체
    {
        class T {
        public:
            int begin() const {return 0;} // int 타입은 이터레이팅 할 수 없습니다.
        };
    
        T t;
        
        EXPECT_TRUE(std::begin(t) == 0); // (△) 비권장. 전달받은 개체의 begin() 함수를 호출합니다.
        // EXPECT_TRUE(std::ranges::begin(t) == 0); // (X) 컴파일 오류. 전달받은 개체의 이터레이팅 시작점을 리턴합니다. 이터레이팅 할 수 없으므로 오류를 리턴합니다.
    
        class U {
        public:
            int* begin() const {return nullptr;} // int* 타입은 이터레이팅 할 수 있습니다.
        }; 
        U u;

        EXPECT_TRUE(std::begin(u) == nullptr); //  전달받은 개체의 begin() 함수를 호출합니다.
        EXPECT_TRUE(std::ranges::begin(u) == nullptr); // 전달받은 개체의 begin() 함수를 호출합니다.
    }
    {
        // auto f{std::begin}; // (X) 컴파일 오류. 함수자로 사용할 수 없습니다.
        auto g{std::ranges::begin}; // 함수자로 사용할 수 있습니다.
    }
#endif    

    // 범위(Range) 알고리즘
    {
        std::vector<int> v{3, 2, 1};
        std::sort(v.begin(), v.end()); // 기존 방식은 시작과 끝 이터레이터를 전달해야 합니다.
        EXPECT_TRUE(v[0] == 1 && v[1] == 2 && v[2] == 3);
    }
#if 202002L <= __cplusplus // C++20~      
    {
        std::vector<int> v{3, 2, 1};
        std::ranges::sort(v); // 범위를 전달하면 됩니다.
        EXPECT_TRUE(v[0] == 1 && v[1] == 2 && v[2] == 3);       
    }
#endif

    // 범위(Range) 알고리즘의 Projection
    {
        std::vector<std::pair<std::string, int>> v{
            {"Kim", 30}, 
            {"Park", 10},
            {"Lee", 20} 
        };

        std::sort(v.begin(), v.end());
        EXPECT_TRUE(v[0].first == "Kim" && v[1].first == "Lee" && v[2].first == "Park"); // pair 값으로 정렬되어 Kim, Lee, Park 순서입니다.
    }
    {
        std::vector<std::pair<std::string, int>> v{
            {"Kim", 30}, 
            {"Park", 10},
            {"Lee", 20} 
        };

#if 201402L <= __cplusplus // C++14~   
        std::sort(
            v.begin(),
            v.end(), 
            // [](std::pair<std::string, int> left, std::pair<std::string, int> right) { // (C++14부터 람다 표현식에 auto를 사용할 수 있습니다.)
            [](auto left, auto right) { // Compare를 pair::second를 사용하도록 사용자 정의합니다.
                return left.second < right.second;
            }
        );
        EXPECT_TRUE(v[0].first == "Park" && v[1].first == "Lee" && v[2].first == "Kim"); // pair.second 값으로 정렬되어 Park, Lee, Kim 순서입니다.
#endif
    }
#if 202002L <= __cplusplus // C++20~     
        std::vector<std::pair<std::string, int>> v{
            {"Kim", 30}, 
            {"Park", 10},
            {"Lee", 20} 
        };
        std::ranges::sort(
            v, 
            {}, // 기본 Compare인 std::ranges::less를 사용합니다.
            &std::pair<std::string, int>::second // 요소 타입인 std::pair<std::string, int> 대신 std::pair<std::string, int>::second를 비교하는데 사용합니다.
        );
        EXPECT_TRUE(v[0].first == "Park" && v[1].first == "Lee" && v[2].first == "Kim"); // pair.second 값으로 정렬되어 Park, Lee, Kim 순서입니다.
#endif

#if 202002L <= __cplusplus // C++20~  
    // 뷰(View) 
    {
        using namespace Ranges_1;

        std::vector<int> v{1, 2, 3};

        MyView view{v};

        EXPECT_TRUE(view.size() == 3); 
        EXPECT_TRUE(view[0] == 1 && view[1] == 2 && view[2] == 3);
    }
    // 미리 정의된 뷰(View)
    { // empty_view
        std::ranges::empty_view<int> r;
        EXPECT_TRUE(r.size() == 0);
        EXPECT_TRUE(r.begin() == nullptr); // 이터레이터가 nullptr입니다.
        EXPECT_TRUE(r.end() == nullptr);
    }
    { // single_view
        int i = 3;
        auto r{std::views::single(3)};
        EXPECT_TRUE(r.size() == 1 && r[0] == 3); // 이터레이팅 할 수 있는 요소가 1개입니다.
    } 
    { // iota_view
        auto r{std::views::iota(10, 13)};
        EXPECT_TRUE(
            r.size() == 3 && 
            r[0] == 10 && r[1] == 11 && r[2] == 12 // 10에서 13보다 작을때까지 만듭니다.
        );
    }
    { // filter_view
        int arr[]{1, 2, 3, 4, 5};
        auto r{
            std::views::filter(
                arr, 
                [](int val) {return 3 < val ? true : false;} // 3보다 큰 것만 true입니다. 상수 시간에 크기를 알지 못하므로 길이를 알지 못하므로 ranges::size()를 구할 수 없습니다.
            )
        };   
        EXPECT_TRUE(
            *r.begin() == 4 &&  // 랜덤 접근을 지원하지 않습니다.
            *(++r.begin()) == 5
        );
    }  
    { // transform_view
        int arr[]{1, 2, 3};
        auto r{
            std::views::transform(
                arr, 
                [](int val) {return val * 10;} // 요소에 10을 곱합니다. 
            )
        };   
        EXPECT_TRUE(
            r.size() == 3 && 
            r[0] == 10 && r[1] == 20 && r[2] == 30
        );
        EXPECT_TRUE(arr[0] == 1 && arr[1] == 2 && arr[2] == 3); // 원본이 수정되는건 아닙니다.

    }  
    { //!! transform_view 복제 테스트

        class A {
        private:
            int m_Val;
        public:
            explicit A(int val) : m_Val{val} {}
            A(const A& other) : m_Val(other.m_Val) {
                std::cout << "Range A : Copy Constructor" << std::endl;
            }
            A(A&&) = delete;
            A& operator =(const A&) = delete;
            A& operator =(A&&) = delete;

            int GetVal() const {return m_Val;}
            void SetVal(int val) {m_Val = val;}
        };


        std::vector<A> arr{A{1}, A{2}, A{3}};
        auto r{
            std::views::transform(
                arr, 
                [](const A& a) {return A{a.GetVal() * 10};} // 요소에 10을 곱합니다. 
            )
        };   
        EXPECT_TRUE(
            r.size() == 3 && 
            r[0].GetVal() == 10 && r[1].GetVal() == 20 && r[2].GetVal() == 30
        );
        EXPECT_TRUE(arr[0].GetVal() == 1 && arr[1].GetVal() == 2 && arr[2].GetVal() == 3); // 원본이 수정되는건 아닙니다.

    }    
    { // take_view
        int arr[]{1, 2, 3, 4, 5};
        auto r{std::views::take(arr, 2)}; // 앞의 2개로 구성됩니다.
        EXPECT_TRUE(
            r.size() == 2 && 
            r[0] == 1 && r[1] == 2
        );
    }
    { // drop_view
        int arr[]{1, 2, 3, 4, 5};
        auto r{std::views::drop(arr, 2)}; // 앞의 2개를 뺀 나머지로 구성됩니다. 
        EXPECT_TRUE(
            r.size() == 3 && 
            r[0] == 3 && r[1] == 4 && r[2] == 5
        );
    }   
    { // join_view
        std::vector<std::string> v{std::string{"ab"}, std::string{"cd"}};
        auto r{std::views::join(v)}; // v의 요소들이 range인 경우 이를 평면화합니다. 상수 시간에 크기를 알지 못하므로 ranges::size()를 구할 수 없습니다.
        EXPECT_TRUE(
            *r.begin() == 'a' && // 랜덤 접근을 지원하지 않습니다. 
            *(++r.begin()) == 'b' && 
            *(++++r.begin()) == 'c' && 
            *(++++++r.begin())  == 'd'
        );
    }
    { // split_view
        std::string v{"ab^^cd"};
        auto r{std::views::split(v, std::string{"^^"})}; // v를 ^^으로 분할합니다. 상수 시간에 크기를 알지 못하므로 ranges::size()를 구할 수 없습니다.
        auto sub1 = *r.begin();
        auto sub2 = *(++r.begin());
        EXPECT_TRUE(
            sub1.size() == 2 &&
            sub1[0] == 'a' && sub1[1] == 'b' &&

            sub2.size() == 2 &&
            sub2[0] == 'c' && sub2[1] == 'd'            
        );
    } 
    { // reverse_view
        int arr[]{1, 2, 3};    
        auto r{std::views::reverse(arr)}; // v를 역순으로 구성합니다.
        EXPECT_TRUE(
            r.size() == 3 &&
            r[0] == 3 && r[1] == 2 && r[2] == 1            
        );
    }
    { // elements_view
        std::vector<std::tuple<int, std::string>> v{
            {0, "Kim"}, 
            {1, "Lee"}
        };
        auto r{std::views::elements<0>(v)}; // tuple의 0번째 요소로 구성합니다.
         EXPECT_TRUE(
            r.size() == 2 && 
            r[0] == 0 && r[1] == 1 
        );
    }  
    { // keys_view, values_view
        std::map<int, std::string> m{
            {0, "Kim"}, 
            {1, "Lee"}
        };
        auto keys{std::views::keys(m)}; // map의 Key로 요소로 구성합니다.
        EXPECT_TRUE(
            keys.size() == 2 && 
            *keys.begin() == 0 && // 랜덤 접근을 지원하지 않습니다. 
            *(++keys.begin()) == 1 
        );
        auto values{std::views::values(m)}; // map의 value로 요소로 구성합니다.
        EXPECT_TRUE(
            values.size() == 2 && 
            *values.begin() == "Kim" && // 랜덤 접근을 지원하지 않습니다.
            *(++values.begin()) == "Lee"
        );
    }  
    // 뷰(View) 합성
    {
        std::vector<std::pair<std::string, int>> v{
            {"Kim", 30}, 
            {"Park", 10},
            {"Lee", 20} 
        };
        auto r1{std::views::elements<1>(v)}; // 점수만 추출합니다.
        auto r2{ // 20 이상인 것으로 구성합니다.
            std::views::filter(
                r1,
                [](int val) {return 20 <= val;} 
            )
        }; 
        auto r3{
            std::views::transform( // 20이상인 것에 10을 곱합니다.
                r2,
                [](int val) {return val * 10;}
            )
        };
        EXPECT_TRUE(*r3.begin() == 300 && *(++r3.begin()) == 200); // filter에서 랜덤 접근을 지원하지 않습니다.
    }  
    {
        std::vector<std::pair<std::string, int>> v{
            {"Kim", 30}, 
            {"Park", 10},
            {"Lee", 20} 
        };
        auto r{
            std::views::transform( // 20이상인 것에 10을 곱합니다.
                std::views::filter( // 20 이상인 것으로 구성합니다.
                    std::views::elements<1>(v), // 점수만 추출합니다.
                    [](int val) {return 20 <= val;}
                ),
                [](int val) {return val * 10;} 
            )
        };

        EXPECT_TRUE(*r.begin() == 300 && *(++r.begin()) == 200); // filter에서 랜덤 접근을 지원하지 않습니다.   
    } 
    {
        std::vector<std::pair<std::string, int>> v{
            {"Kim", 30}, 
            {"Park", 10},
            {"Lee", 20} 
        };
        auto r{
            std::views::elements<1>(v) | // 점수만 추출합니다.
            std::views::filter([](int val) {return 20 <= val;}) | // 20 이상인 것으로 구성합니다.
            std::views::transform([](int val) {return val * 10;}) // 20이상인 것에 10을 곱합니다.
        };

        EXPECT_TRUE(*r.begin() == 300 && *(++r.begin()) == 200); // filter에서 랜덤 접근을 지원하지 않습니다. 
    }  
    // **뷰(View)의 성질 전파**
    {
        std::vector<int> v{1, 2, 3};
        auto r{std::views::transform(v, [](int val) {return val * 10;})};

        // transform은 size()와 랜덤접근이 가능합니다.
        EXPECT_TRUE(r.size() == 3 && r[0] == 10 && r[1] == 20 && r[2] == 30);
    }
    {
        std::vector<int> v{1, 2, 3};
        auto r1{std::views::filter(v, [](int val) {return 20 <= val;})};
        auto r2{std::views::transform(r1, [](int val) {return val * 10;})};

        // (X) 컴파일 오류. filter()의 결과 Range()를 사용하기 때문에 size()와 랜덤 접근을 사용할 수 없습니다.
        // EXPECT_TRUE(r1.size() == 3 && r1[0] == 10 && r1[1] == 20 && r1[2] == 30);

    }
    // 뷰(View)의 지연 생성
    {
        auto r{
            std::views::iota(10) | // 10부터 1씩 증가하는 무한한 시퀀스입니다.
            std::views::take(3) // 앞의 3개로만 구성합니다. 즉, 10, 11, 12입니다.
        };

        int sum{0};
        for(auto val : r) {
            sum += val;
        }
        EXPECT_TRUE(sum == 10 + 11 + 12);
    } 
#endif
}