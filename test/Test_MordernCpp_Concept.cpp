#include "gtest/gtest.h"

namespace Concept_1 {
    template<typename T> 
    T Add(T a, T b) {
        return a + b;
    }
}
namespace Concept_2 {
    template<
        typename T, 
        typename U = typename std::enable_if<
            std::is_integral<T>::value || // T가 정수 타입이면 true 입니다.
            std::is_floating_point<T>::value // T가 실수 타입이면 true 입니다.
        >::type // 조건이 true인 경우에만 enable_if<>::type이 정의됩니다. 따라서 조건이 false면 템플릿 인스턴스화가 되지 않습니다.
    > 
    T Add_11(T a, T b) {
        return a + b;
    }
}
#if 202002L <= __cplusplus // C++20~
namespace Concept_3 {
    template<typename T>
    requires std::integral<T> || std::floating_point<T> // T는 정수 타입이나 실수 타입이어야 합니다.
    T Add_20(T a, T b) {
        return a + b;
    }   
}
// 컨셉(concept)과 제약 조건
namespace Concept_4 {
    template<typename T>
    concept Number_20 = std::integral<T> || std::floating_point<T>; // T는 정수 타입이나 실수 타입이어야 합니다.   
  
    template<typename T>
    concept Integral_20 = std::is_integral<T>::value; // T는 정수 타입이어야 합니다.

    template<typename T>
    concept Addable_20 = requires(T a, T b) {
        a + b; // a + b 가 유효해야 합니다.
    };

    class A_11 {
    public:
        using Ref = A_11&; // 종속 타입
    };
    template<typename T> 
    concept MyType_20 = requires {
        typename T::Ref; // 종속 타입 Ref가 제공되어야 합니다.
    }; 

    template<typename T>
    concept Equal_20 = requires(T a, T b) {
        {a == b} -> std::convertible_to<bool>; // T 는 ==을 제공하고 결과는 bool로 변환되어야 합니다.
        {a != b} -> std::convertible_to<bool>; // T 는 !=을 제공하고 결과는 bool로 변환되어야 합니다.
    }; 

    template<typename T>
    concept AddableEqual_20 = requires(T a, T b) {
        
        {a == b} -> std::convertible_to<bool>; // T 는 ==을 제공하고 결과는 bool로 변환되어야 합니다.
        {a != b} -> std::convertible_to<bool>; // T 는 ==을 제공하고 결과는 bool로 변환되어야 합니다.
        
        requires Addable_20<T>; // requires 로 추가 제약 조건을 작성합니다.
    }; 

}
#endif
#if 202002L <= __cplusplus // C++20~
namespace Concept_5 {

    // 컨셉 설계
    template<typename T>
    concept Number_20 = std::integral<T> || std::floating_point<T>; // T는 정수 타입이나 실수 타입이어야 합니다.

    template<typename T>
    concept Addable_20 = requires (T a, T b) {  
        a + b; // a + b 가 유효해야 합니다.
    };

    template<typename T>
    concept NumberAddable_20 = // 숫자를 더할 수 있습니다.
        Number_20<T> && // 정수 타입이나 실수 타입이고,
        std::copyable<T> && // 복사 생성, 이동 생성, 복사 대입, 이동 대입, 소멸이 가능해야 합니다.
        Addable_20<T>; //  a + b 가 유효해야 합니다.

    template<typename T>
    requires NumberAddable_20<T>
    T Add_20(T a, T b) {
        return a + b;
    }
}
#endif
namespace Concept_6 {
    class IDrawable_11 {
    public:
        // 인터페이스이므로 사용 못하게 막음.
        IDrawable_11(const IDrawable_11&) = delete; 
        IDrawable_11(IDrawable_11&&) = delete;
        IDrawable_11& operator =(const IDrawable_11&) = delete;
        IDrawable_11& operator =(IDrawable_11&&) = delete;
    protected:
        // 인터페이스여서 상속한 개체에서만 사용할 수 있게 함
        IDrawable_11() = default;
        ~IDrawable_11() = default;
    public:
        // 자식 클래스에서 구체화 해야 합니다.
        virtual void Draw() const = 0;
    };
    class Rectangle_11 : public IDrawable_11 {
    public:
        virtual void Draw() const override {
            std::cout << "Rectangle_11::Draw()" << std::endl;
        }
    }; 
    class Circle_11 : public IDrawable_11 {
    public:
        virtual void Draw() const override {
            std::cout << "Circle_11::Draw()" << std::endl;
        }
    };  

    // IDrawable_11 인터페이스를 사용한 개체를 Draw() 합니다.
    void Draw(const IDrawable_11& drawable) {
        drawable.Draw();
    }    
}
#if 202002L <= __cplusplus // C++20~
namespace Concept_7 {
    template<typename T>
    concept Drawable_20 = requires(T obj) {
        obj.Draw(); // Draw() 멤버 함수를 호출할 수 있어야 합니다.
    };

    // Drawable_20 컨셉을 준수하는 개체를 Draw() 합니다.
    template<typename T>
    requires Drawable_20<T> // Draw() 함수를 호출할 수 있어야 합니다.
    void Draw_20(const T& drawable) {
        drawable.Draw();
    }
}
#endif
#if 202002L <= __cplusplus // C++20~
namespace Concept_8 {
    template<typename T>
    requires std::integral<T> || std::floating_point<T> // 논리 연산자로 합성합니다.
    auto Func_20(T val) {
        return val;
    }

    // requires로 템플릿 인자에 적용
    template<typename T> 
    requires std::integral<T> 
    auto Func1_20(T val) {
        return val;
    }

    // 후행 requires로 함수 선언의 끝에 템플릿 인자에 적용
    template<typename T> 
    auto Func2_20(T val) requires std::integral<T> {
        return val;
    }

    // 템플릿 인자에 적용
    template<std::integral T> 
    auto Func3_20(T val) {
        return val;
    }  

    // 비타입 템플릿 인자에 적용
    template<std::integral auto v>
    auto Func4_20 () {
        return v;
    }  

    // 리턴값과 함수 인자에 적용
    std::integral auto Add_20(std::integral auto val) {
        return val;
    }
    
    // 타입 추론에 적용
    int Func() {return 1;}  
}
namespace Concept_9 {
    // 클래스 템플릿
    template<std::integral T> 
    class A_20 {};

    // 멤버 함수
    template<typename T>
    class B_20 {
    public:
        void Func(T other) requires std::integral<T> {} 
    };

    // 템플릿 특수화
    template<typename T>
    class C_20 {
    public:
        int Func() {return 1;}    
    };

    template<std::integral T>
    class C_20<T> {
    public:
        int Func() {return 2;}    
    };

    // 오버로딩
    int Func_20(auto val) {
        return 1;
    }
    int Func_20(std::integral auto val) {
        return 2;
    }
    int Func_20(std::floating_point auto val) {
        return 3;
    }
    int Func_20(long val) {
        return 4;
    }
}
namespace Concept_10 {
    // 익명 컨셉
    template<typename T>
    requires requires(T a, T b) { // requires(T a, T b) {}은 익명 컨셉입니다.
        a + b; 
    }
    T Add_20(T a, T b) {return a + b;}
}
#endif

TEST(TestMordern, Concept) {

    // 개요
    {
        using namespace Concept_1;
        EXPECT_TRUE(Add(1, 2) == 3);
        EXPECT_TRUE(Add(1.0, 2.0) == 3.0);  
        EXPECT_TRUE(Add(std::string("Hello"), std::string("World")) == std::string("HelloWorld")); // (△) 비권장. 의도하지 않았는데, 되네요.       
    }
    {
        using namespace Concept_2;
        EXPECT_TRUE(Add_11(1, 2) == 3);
        EXPECT_TRUE(Add_11(1.0, 2.0) == 3.0);  
        // EXPECT_TRUE(Add_11(std::string("Hello"), std::string("World")) == std::string("HelloWorld")); // (X) 컴파일 오류. std::is_integral<T>::value 가 true 인 것만 가능합니다.
    }
#if 202002L <= __cplusplus // C++20~    
    {
        using namespace Concept_3;
        EXPECT_TRUE(Add_20(1, 2) == 3);
        EXPECT_TRUE(Add_20(1.0, 2.0) == 3.0);
        // EXPECT_TRUE(Add_20(std::string("Hello"), std::string("World")) == std::string("HelloWorld")); // (X) 컴파일 오류. std::is_integral<T>::value 가 true 인 것만 가능합니다.        
    }
#endif   
#if 202002L <= __cplusplus // C++20~    
    {
        // 컨셉 설계
        using namespace Concept_5;
        EXPECT_TRUE(Add_20(1, 2) == 3);
        EXPECT_TRUE(Add_20(1.0, 2.0) == 3.0);
        //EXPECT_TRUE(Add_20(std::string("Hello"), std::string("World")) == std::string("HelloWorld")); // (X) 컴파일 오류. std::is_integral<T>::value 가 true 인 것만 가능합니다.
    }
#endif
    {
        using namespace Concept_6;

        Rectangle_11 rect;
        Circle_11 circle;

        // rect와 circle의 가상 함수인 Draw가 호출됩니다.
        Draw(rect); 
        Draw(circle);
    }    
#if 202002L <= __cplusplus // C++20~  
    {
        using namespace Concept_7;
        class Rectangle {
        public:
            void Draw() const {
                std::cout << "Rectangle::Draw()" << std::endl;
            }
        }; 
        class Circle {
        public:
            void Draw() const {
                std::cout << "Circle::Draw()" << std::endl;
            }
        };
        
        Rectangle rect;
        Circle circle;

        // Drawable_20 컨셉을 준수하는 개체만 호출 가능합니다.
        Draw_20(rect);
        Draw_20(circle);
    }
#endif
#if 202002L <= __cplusplus // C++20~  
    {
        using namespace Concept_8;

        Func_20(1); // int;
        Func_20(1.0); // double
        // Func_20(std::string("Hello")); // (X) 컴파일 오류.

        std::integral auto val_20 = Func();  

        // 람다 표현식의 템플릿 인자에 적용
        auto val1_20 = [] <std::integral T> (T val) {
            return val;
        };  
        auto val2_20 = [] <typename T> requires std::integral<T> (T val) {
            return val;
        }; 
        auto val3_20 = [] <typename T> (T val) requires std::integral<T> {
            return val;
        };     

        // 람다 표현식의 비타입 템플릿 인자에 적용
        auto val4_20 = [] <std::integral auto v> () {
            return v;
        }; 

        // 람다 표현식의 함수 인자에 적용
        auto val5_20 = [] (std::integral auto val) {
            return val;
        };  
    }
    {
        using namespace Concept_9;

        // 컨셉 활용
        A_20<int> a1;
        // A_20<double> a2; // (X) 컴파일 오류

        B_20<int> b1;
        b1.Func(10);

        B_20<double> b2;
        // b2.Func(10.0); // (X) 컴파일 오류

        EXPECT_TRUE(C_20<double>{}.Func() == 1);
        EXPECT_TRUE(C_20<int>{}.Func() == 2); // integral 컨셉 버전

        EXPECT_TRUE(Func_20("Hello") == 1); // 일반 버전  
        EXPECT_TRUE(Func_20(1) == 2); // integral 컨셉 버전
        EXPECT_TRUE(Func_20(1.0) == 3); // floating_point 컨셉 버전
        EXPECT_TRUE(Func_20(1L) == 4); // long 버전
    }
    {
        using namespace Concept_10;
        Add_20(1, 2); // + 가 제공되어 사용 가능합니다.

        class A {}; // + 가 제공되지 않습니다.
        A a, b;
        // Add_20(a, b); // (X) 컴파일 오류. + 가 제공되지 않습니다.
    }
#endif
}