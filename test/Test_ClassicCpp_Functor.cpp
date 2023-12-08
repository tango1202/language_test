#include "gtest/gtest.h" 

#include <algorithm>

namespace Functor_1 {
    template<typename InputIterator, typename Function>
    Function my_for_each(InputIterator first, InputIterator last, Function f) {
        for (; first != last; ++first) { 
            f(*first); // 함수명 + () 또는 개체명 + () 로 호출됩니다.
        }
        return f; 
    }
    // 10을 세팅하는 함수입니다.
    void Func(int& val) {
        val = 10;
    }

    // 20을 세팅하는 함수자입니다.
    class Functor {
    public:
        void operator ()(int& val) {
            val = 20;
        } 
    };

    // 각 요소에 지정한 값을 추가합니다.
    class Adder {
        int m_Val; // 상태를 저장합니다.
    public:
        explicit Adder(int val) : m_Val(val) {}
        void operator ()(int& val) {
            val += m_Val;
        }  
        int GetVal() const {return m_Val;}  
    };
}
namespace Functor_2 {
    // 인자 1개를 사용하는 함수자 정의
#if 201103L <= __cplusplus // C++11~
    class Adder {
        using argument_type = int&;
        using result_type = void;
#else    
    class Adder : std::unary_function<int&, void> {
#endif        
        argument_type m_Val;

    public:
        explicit Adder(argument_type val) : m_Val(val) {}
        result_type operator ()(argument_type val) {
            val += m_Val;
        }  
        int GetVal() const {return m_Val;}      
    };
}

namespace Functor_2 {
    template<typename InputIterator, typename Predicate>
    InputIterator my_find_if( // std 버전도 대략 이렇게 구현되었습니다.
        InputIterator first, 
        InputIterator last,
	    Predicate pred) {
        
        // 조건자(Predicate)가 참이면 해당 위치의 이터레이터를 리턴합니다.
        while (first != last && !pred(*first)) {
            ++first;
        }
      return first;
    }

    // 7보다 작은지 검사하는 함수
#if 201103L <= __cplusplus // C++11~
    class Less_7 {
        using argument_type = int;
        using result_type = bool;
#else
    class Less_7 : std::unary_function<int, bool> {
#endif        
    public:
        result_type operator ()(argument_type val) {
            return val < 7 ? true : false;
        }       
    };

}


TEST(TestClassicCpp, Functor) {
    // 함수자
    {
        using namespace Functor_1;

        // 0으로 초기화 되어 생성
        std::vector<int> v(3);

        // 함수 호출
        my_for_each(v.begin(), v.end(), Func);
        EXPECT_TRUE(v[0] == 10 && v[1] == 10 && v[2] == 10); 

        // 함수자 호출
        my_for_each(v.begin(), v.end(), Functor());
        EXPECT_TRUE(v[0] == 20 && v[1] == 20 && v[2] == 20); 
    }
    // 함수자에서 상태 활용 
    {
        using namespace Functor_1;

        std::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        // v 의 각 요소에 10을 더합니다.
        Adder adder(10);
        std::for_each(v.begin(), v.end(), adder);
        EXPECT_TRUE(v[0] == 11 && v[1] == 12 && v[2] == 13);
    }

    // 함수자 타입 특성 클래스(Traits)
    {
        using namespace Functor_2;
        std::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        // v 의 각 요소에 10을 더합니다.
        int init = 10; // int& 타입이어서 변수로 값을 전달합니다.
        Adder adder(init);
        std::for_each(v.begin(), v.end(), adder);
        EXPECT_TRUE(v[0] == 11 && v[1] == 12 && v[2] == 13);
    }
    // binder
    {
        using namespace Functor_2;

        std::vector<int> v;
        v.push_back(7);
        v.push_back(4);
        v.push_back(3);   

        std::vector<int>::iterator itr;

        itr = my_find_if( 
            v.begin(),
            v.end(),
            Less_7()
        );  
        EXPECT_TRUE(*itr == 4);  

        // less(x, y)는 x < y 를 통해 true/false를 리턴하는 이항 함수자
        // bind2nd()를 통해 알고리즘에서 less(벡터 요소, 7) 로 호출됨
        // 7보다 작은 첫 요소를 리턴함
        itr = std::find_if( 
            v.begin(),
            v.end(),
#if 201103L <= __cplusplus // C++11~
            std::bind(std::less<int>(), std::placeholders::_1, 7)
#else            
            std::bind2nd(std::less<int>(), 7) // 7보다 작으면 true를 리턴하는 단항 함수자
#endif
        );  

        EXPECT_TRUE(*itr == 4);  

    }
    // 아답터, 부정자
    {
        class A {
        public:
            void Func() {}
        };

        std::vector<A> v(2);
        std::vector<A>::iterator itr = v.begin();
        std::vector<A>::iterator endItr = v.begin();
        for (; itr != endItr; ++itr) {
            (*itr).Func();
        }
#if 201103L <= __cplusplus // C++11~
        std::for_each(v.begin(), v.end(), std::mem_fn(&A::Func));
#else
        std::for_each(
            v.begin(), v.end(), 
            std::mem_fun_ref(&A::Func) // 요소.Func()으로 호출해 주는 어뎁터입니다.
        );
#endif
    }
}
