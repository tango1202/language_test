#include "gtest/gtest.h" 

#include <typeinfo>

namespace MetaProgramming_1 {

    // Factorial n -1 을 재귀 호출합니다.
    template<unsigned int n>
    struct Factorial { 
        enum {
            Val = n * Factorial<n-1>::Val
        }; 
    };
    // 0일때 특수화 버전. 더이상 재귀 호출을 안합니다.
    template<>
    struct Factorial<0> { 
        enum {
            Val = 1
        }; 
    };
}

namespace MetaProgramming_2 {

    template<typename T>
    class my_smart_ptr { 
        T* m_Ptr; 
    public:
        my_smart_ptr(T* ptr) : 
            m_Ptr(ptr) {}
        ~my_smart_ptr() {
            delete m_Ptr;
        }

    private:
        // 테스트와 상관없어서 복사 생성자와 복사 대입 연산자는 private로 막아 두었습니다.
        my_smart_ptr(my_smart_ptr& other) {}
        my_smart_ptr& operator =(my_smart_ptr& other) {return *this;}

    public:
        T* GetPtr() {
            return m_Ptr;
        } 
        // T 개체의 Clone()을 호출합니다.
        T* Clone() const {
            if (m_Ptr == NULL) {
                return(NULL);
            }
            return m_Ptr->Clone();
        }
    };
}
namespace MetaProgramming_3 {

    template<typename T>
    class my_smart_ptr { 
        T* m_Ptr; 
    public:
        my_smart_ptr(T* ptr) : 
            m_Ptr(ptr) {}
        ~my_smart_ptr() {
            delete m_Ptr;
        }

    private:
        my_smart_ptr(my_smart_ptr& other) {}
        my_smart_ptr& operator =(my_smart_ptr& other) {return *this;}

    public:
        T* GetPtr() {
            return m_Ptr;
        } 
        T* Clone() const {
            if (m_Ptr == NULL) {
                return(NULL);
            }
            return new T(*m_Ptr);
        }
    };

    // 추상 클래스 입니다.
    class Shape {
    protected:
        Shape() {} 
        Shape(const Shape& other) {}
    public:
        virtual ~Shape() {}
        virtual Shape* Clone() const = 0; 
    };

    class Rectangle : public Shape {
    public:
        virtual Rectangle* Clone() const { 
            return new Rectangle(*this); 
        }
    };
    // Shape은 Clone()을 사용하므로 특수화 합니다.
    template<>
    class my_smart_ptr<Shape> { 
        Shape* m_Ptr; 
    public:
        my_smart_ptr(Shape* ptr) : 
            m_Ptr(ptr) {}
        ~my_smart_ptr() {
            delete m_Ptr;
        }

    private:
        my_smart_ptr(my_smart_ptr& other) {}
        my_smart_ptr& operator =(my_smart_ptr& other) {return *this;}

    public:
        Shape* GetPtr() {
            return m_Ptr;
        } 
        // Shape 개체의 Clone()을 호출합니다.
        Shape* Clone() const {
            if (m_Ptr == NULL) {
                return(NULL);
            }
            return m_Ptr->Clone();
        }
    };    
}

namespace MetaProgramming_4 {

    template<typename T>
	class CloneTraits {
	public:
		// 일반적인 복제는 복사 생성자를 사용합니다.
		static T* Clone(const T* ptr) {
			if (ptr == NULL) {
				return  NULL;
			}

			return new T(*ptr);
		}
	};

    template<typename T>
    class my_smart_ptr { 
        T* m_Ptr; 
    public:
        my_smart_ptr(T* ptr) : 
            m_Ptr(ptr) {}
        ~my_smart_ptr() {
            delete m_Ptr;
        }

    private:
        my_smart_ptr(my_smart_ptr& other) {}
        my_smart_ptr& operator =(my_smart_ptr& other) {return *this;}

    public:
        T* GetPtr() {
            return m_Ptr;
        } 
        // CloneTraits를 이용하여 복제합니다.
        T* Clone() const {
            return CloneTraits<T>::Clone(m_Ptr);
        }
    };

    // 추상 클래스 입니다.
    class Shape {
    protected:
        Shape() {} 
        Shape(const Shape& other) {}
    public:
        virtual ~Shape() {}
        virtual Shape* Clone() const = 0; 
    };

    class Rectangle : public Shape {
    public:
        virtual Rectangle* Clone() const { 
            return new Rectangle(*this); 
        }
    };
    // Shape을 복제할 땐 Clone 함수를 사용합니다.
    template<>
	class CloneTraits<Shape> {
	public:
		static Shape* Clone(const Shape* ptr) 
		{
			if (ptr == NULL) {
				return NULL;
			}

			return ptr->Clone();
		}
	};
}

namespace MetaProgramming_5 {

    // Clone() 을 제공하는 단위 인터페이스
    class ICloneable {
    private:
        ICloneable(const ICloneable& other) {} // 인터페이스여서 외부에서 사용 못하게 복사 생성자 막음
        ICloneable& operator =(const ICloneable& other) {return *this;} // 인터페이스여서 외부에서 사용 못하게 복사 대입 연산자 막음    
    protected:
        ICloneable() {} // 인터페이스여서 상속한 개체에서만 생성할 수 있게함 
        ~ICloneable() {} // 인터페이스여서 protected non-virtual(상속해서 사용하고, 다형 소멸 안함) 입니다. 
    public:
        virtual ICloneable* Clone() const = 0; // 순가상 함수입니다. 자식 클래스에서 구체화 해야 합니다.
    };

    // D가 B를 상속하였는지 검사하는 템플릿
    template<typename D, typename B> 
    class IsDerivedFrom {
        class No {};
        class Yes {No no[2];}; // No 클래스보다 크기가 큰 클래스

        // B*로 변환되는 타입이라면 Yes를 리턴하고, 그렇지 않은 모든 것을은 No를 리턴합니다.
        static Yes Test(B*); // 컴파일 타임에만 사용해서 선언만 하고 정의는 불필요합니다.
        static No Test(...); // 컴파일 타임에만 사용해서 선언만 하고 정의는 불필요합니다.
    public:
        // Test() 함수에 D*를 전달했을때 B*로 변환되었다면 상속받았기 때문에 true, 그렇지 않으면 false를 저장합니다.
        enum {Val = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes)};
    };

    // CloneTag<true>와 CloneTag<false> 타입을 만듭니다.
    template<bool val>
    class CloneTag {
    public:
        enum {Val = val};
    };

    // IsDerivedFrom을 이용하여 ICloneable 상속 여부를 컴파일 타임에 판단하여, 컴파일 타임에 복사 생성할지, Clone을 호출할지 결정합니다.
	template<typename T>
    class CloneTraits {
        static T* Clone(const T* ptr, CloneTag<false>) {
            return new T(*ptr); 
        }
        static T* Clone(const T* ptr, CloneTag<true>) {
            return ptr->Clone();
        }
	public:
        // 오버로딩을 통해 컴파일 타임에 복사 생성자를 사용할지 Clone()을 사용할 지 결정합니다.
        static T* Clone(const T* ptr) {
			if (ptr == NULL) {
				return  NULL;
			}

            return Clone(
                ptr, 
                CloneTag<IsDerivedFrom<T, ICloneable>::Val>()
            );
        }

		// if 문을 사용했기 때문에 런타임에 복사 생성자를 사용할지 Clone()을 사용할 지 결정합니다.
		// static T* Clone(const T* ptr) {
		// 	if (ptr == NULL) {
		// 		return  NULL;
		// 	}

        //     // ICloneable을 상속하지 않았다면 복사 생성
        //     // (X) 컴파일 오류. Shape의 복사 생성자는 protected 여서 사용할 수 없습니다.
        //     if (!IsDerivedFrom<T, ICloneable>::Val) {
        //         return new T(*ptr);
        //     }
        //     // ICloneable을 상속했다면 Clone() 호출
        //     else {
        //         return ptr->Clone();
        //     }
		// }        
	};
    template<typename T>
    class my_smart_ptr { 
        T* m_Ptr; 
    public:
        my_smart_ptr(T* ptr) : 
            m_Ptr(ptr) {}
        ~my_smart_ptr() {
            delete m_Ptr;
        }

    private:
        // 테스트와 상관없어서 복사 생성자와 복사 대입 연산자는 private로 막아 두었습니다.
        my_smart_ptr(my_smart_ptr& other) {}
        my_smart_ptr& operator =(my_smart_ptr& other) {return *this;}

    public:
        T* GetPtr() {
            return m_Ptr;
        } 
        // CloneTraits를 이용하여 복제합니다.
        T* Clone() const {
            return CloneTraits<T>::Clone(m_Ptr);
        }
    };  

    // 추상 클래스 입니다.
    // ICloneable을 상속합니다.
    class Shape : public ICloneable {
    protected:
        Shape() {} 
        Shape(const Shape& other) {}
    public:
        virtual ~Shape() {}

        // Shape::Clone()을 하면 ICloneable*이 아니라 Shape* 리턴되도록 조정합니다.
        virtual Shape* Clone() const { 
            return NULL; 
        }
    };

    class Rectangle : public Shape {
    public:
        // (O) 자식 개체에서는 자식 타입으로 리턴합니다.
        // Rectangle의 복사 생성자를 이용하며 복제본을 리턴합니다.
        virtual Rectangle* Clone() const { 
            return new Rectangle(*this); 
        }
    };      
}
TEST(TestClassicCpp, MetaProgramming) {
    {
        using namespace MetaProgramming_1;

        // 컴파일 타임에 계산된 120이 val에 대입됩니다.
        unsigned int val = Factorial<5>::Val; 
        EXPECT_TRUE(val == 1 * 2 * 3 * 4 * 5);
    }
    {
        using namespace MetaProgramming_2;
        
        // 추상 클래스 입니다.
        class Shape {
        protected:
            Shape() {} 
            Shape(const Shape& other) {}
        public:
            virtual ~Shape() {}
            // (O) 부모 개체에서는 Shape* 으로 리턴합니다.  
            virtual Shape* Clone() const = 0; 
        };

        class Rectangle : public Shape {
        public:
            // (O) 자식 개체에서는 자식 타입으로 리턴합니다.
            // Rectangle의 복사 생성자를 이용하며 복제본을 리턴합니다.
            virtual Rectangle* Clone() const { 
                return new Rectangle(*this); 
            }
        };  

        my_smart_ptr<Shape> sp1(new Rectangle);
        my_smart_ptr<Shape> sp2(sp1.Clone());     

        // sp2는 Rectangle 을 관리합니다.
        EXPECT_TRUE(typeid(*sp2.GetPtr()).name() == typeid(Rectangle).name());
    }
    {
        using namespace MetaProgramming_3;

        my_smart_ptr<Shape> sp1(new Rectangle);
        my_smart_ptr<Shape> sp2(sp1.Clone());     

        // sp2는 Rectangle 을 관리합니다.
        EXPECT_TRUE(typeid(*sp2.GetPtr()).name() == typeid(Rectangle).name());

        my_smart_ptr<int> sp3(new int);
        my_smart_ptr<int> sp4(sp3.Clone());

        // sp4는 int 를 관리합니다.
        EXPECT_TRUE(typeid(*sp4.GetPtr()).name() == typeid(int).name());
    }
    {
        using namespace MetaProgramming_4;

        my_smart_ptr<Shape> sp1(new Rectangle);
        my_smart_ptr<Shape> sp2(sp1.Clone());     

        // sp2는 Rectangle 을 관리합니다.
        EXPECT_TRUE(typeid(*sp2.GetPtr()).name() == typeid(Rectangle).name());

        my_smart_ptr<int> sp3(new int);
        my_smart_ptr<int> sp4(sp3.Clone());

        // sp4는 int 를 관리합니다.
        EXPECT_TRUE(typeid(*sp4.GetPtr()).name() == typeid(int).name());    
    }
    {
        using namespace MetaProgramming_5;

        my_smart_ptr<Shape> sp1(new Rectangle);
        my_smart_ptr<Shape> sp2(sp1.Clone());     
        
        // sp2는 Rectangle 을 관리합니다.
        EXPECT_TRUE(typeid(*sp2.GetPtr()).name() == typeid(Rectangle).name());
         
        my_smart_ptr<int> sp3(new int);
        my_smart_ptr<int> sp4(sp3.Clone());

        // sp4는 int 를 관리합니다.
        EXPECT_TRUE(typeid(*sp4.GetPtr()).name() == typeid(int).name());      
    }
    {
        using namespace MetaProgramming_5;
        
        // // 컴타일타임에 ICloneable 상속했는지는 판단하고, 런타임에 if 문으로 복사 생성할지, Clone()을 호출할지 분기합니다.
        // Shape* shape = CloneTraits<Shape>::Clone(new Rectangle);
        // EXPECT_TRUE(typeid(*shape).name() == typeid(Rectangle).name());
        // delete shape;
    }
}
