#include "gtest/gtest.h" 

namespace {
    class T {};
    class U {};
    void Func(std::shared_ptr<T> t, std::shared_ptr<U> u) {}
}

TEST(TestMordern, SharedPtr) {
    {
        class T {
        public:
            int m_Val;
        public:
            T() {std::cout << "T : Constructor" << std::endl;}
            ~T() {std::cout << "T : Destructor" << std::endl;}
        };
        std::shared_ptr<T> a{new T{}};
        EXPECT_TRUE(a.use_count() == 1); 

        std::shared_ptr<T> b{a};
        EXPECT_TRUE(a.use_count() == 2 && b.use_count() == 2); 
        {
            std::shared_ptr<T> c{a};
            EXPECT_TRUE(a.use_count() == 3 && b.use_count() == 3 && c.use_count() == 3);

            // a, b, c가 관리하는 T는 서로 공유합니다.
            a->m_Val = 1; 
            EXPECT_TRUE(a->m_Val == 1 && b->m_Val == 1 && c->m_Val == 1); 

            b->m_Val = 2;
            EXPECT_TRUE(a->m_Val == 2 && b->m_Val == 2 && c->m_Val == 2); 

            c->m_Val = 3;
            EXPECT_TRUE(a->m_Val == 3 && b->m_Val == 3 && c->m_Val == 3);            
        } // 유효 범위를 벗어났기 때문에 c가 소멸됨
        // c가 소멸되었으므로 2
        EXPECT_TRUE(a.use_count() == 2 && b.use_count() == 2);
    }
    // 제어 블록
    {
        class T{};

        std::shared_ptr<T> a{new T{}};
        std::shared_ptr<T> b{a};        
    }
    // make_shared
    {
        class T {
        public:
            explicit T(int) {}    
        };
        std::shared_ptr<T> a{new T{10}}; // (△) 비권장. T를 할당하고, 내부적으로 제어 블록을 할당
        std::shared_ptr<T> b = std::make_shared<T>(10); // T + 제어 블록 크기만큼 할당
    }
    {
        Func(std::shared_ptr<T>{new T}, std::shared_ptr<U>{new U}); // (△) 비권장. new T, new U 호출 순서에 따라 예외가 발생합니다.
        Func(std::make_shared<T>(), std::make_shared<U>()); // (O) 
    }
    {
        class T {
        public:
            explicit T(int) {}    
        };
        std::vector<std::shared_ptr<T>> v;

        v.push_back(std::shared_ptr<T>{new T{10}});
        v.push_back(std::make_shared<T>(10)); // 구문이 좀더 간결합니다.
        v.push_back(std::shared_ptr<T>{}); // nullptr

        EXPECT_TRUE(v.size() == 3 && v[2] == nullptr);
    }    
    // std::enable_shared_from_this
    {
        class T {
        private:
            T() {} // private로 만들어서 생성자 접근 차단
        public:
            static std::shared_ptr<T> Create() {
                return std::shared_ptr<T>{new T{}};      
            }  
        };
        // std::shared_ptr<T> a{new T{}}; // (X) 컴파일 오류. 생성자가 private
        std::shared_ptr<T> a {T::Create()};
        // a의 제어 블록을 공유합니다.
         std::shared_ptr<T> b{a}; 
        
         EXPECT_TRUE(a.use_count() == 2);

        // (X) 오동작. 동일한 개체를 각각의 제어 블록으로 관리합니다. 소유권을 공유하지 않았기 때문에 소멸시 각각 delete 하여 소유권 분쟁이 발생합니다.
        // a가 관리하는 포인터를 이용하여 새로운 제어 블록을 만듭니다.
        // std::shared_ptr<T> b{a.get()}; 
        // EXPECT_TRUE(a.use_count() == 1 && b.use_count() == 1);
    }
    {
        class T : public std::enable_shared_from_this<T> {
        private:
            T() {} // private로 만들어서 생성자 접근 차단
        public:
            // this 포인터로부터 shared_ptr을 생성합니다.
            std::shared_ptr<T> GetPtr() {
                return shared_from_this();
            }
            static std::shared_ptr<T> Create() {
                return std::shared_ptr<T>{new T{}};      
            }  
        };

        std::shared_ptr<T> a{T::Create()}; // shared_ptr이 만들어 졌습니다. 이제 shared_from_this()를 사용할 수 있습니다.
        T* t{a.get()};
        std::shared_ptr<T> b{t->GetPtr()}; // 관리하는 개체로부터 shared_ptr을 생성합니다.

        EXPECT_TRUE(a.use_count() == 2);        
    }
    // std::shared_ptr을 이용한 복사 생성자, 복사 대입 연산자
    {
        class Data {
            int m_Val;
        public:
            explicit Data(int val) : m_Val(val) {}
            int GetVal() const {return m_Val;}
            void SetVal(int val) {m_Val = val;}
        };
        class T {
        public:    
            std::shared_ptr<Data> m_Data1;
            std::shared_ptr<Data> m_Data2;
        public:
            // new로 생성된 Data를 받기 위해 unique_ptr을 인자로 사용합니다.
            T(std::unique_ptr<Data> data1, std::unique_ptr<Data> data2) :
                m_Data1(std::move(data1)),
                m_Data2(std::move(data2)) {}

            // // 컴파일러의 암시적 버전과 동일합니다.
            // T(const T& other) : 
            //     m_Data1(other.m_Data1),
            //     m_Data2(other.m_Data2) {}

            // // 컴파일러의 암시적 버전과 동일합니다.
            // ~T() {}

            // // 컴파일러의 암시적 버전과 동일합니다.
            // T& operator =(const T& other) {
            //     // shared_ptr& operator=( const shared_ptr& r ) noexcept; 입니다. 
            //     // 포인터 복사와 참조 카운트 변경만 있으므로 예외 발생은 없습니다.
            //     m_Data1 = other.m_Data1; 
            //     m_Data2 = other.m_Data2; 
                
            //     return *this;
            // }
        };

        T a{std::unique_ptr<Data>{new Data{1}}, std::unique_ptr<Data>{new Data{1}}};
        T b{std::unique_ptr<Data>{new Data{2}}, std::unique_ptr<Data>{new Data{2}}};
        T c{std::unique_ptr<Data>{new Data{3}}, std::unique_ptr<Data>{new Data{3}}};

        EXPECT_TRUE(a.m_Data1.use_count() == 1 && b.m_Data1.use_count() == 1 && c.m_Data1.use_count() == 1);

        //복사 생성합니다. a와 d는 동일 Data 입니다.
        T d{a};
        EXPECT_TRUE(a.m_Data1.use_count() == 2 && b.m_Data1.use_count() == 1 && c.m_Data1.use_count() == 1 && d.m_Data1.use_count() == 2);

        // a의 Data 값을 수정하면 d의 값도 수정됩니다.
        a.m_Data1->SetVal(10);
        EXPECT_TRUE(a.m_Data1->GetVal() == 10 && b.m_Data1->GetVal() == 2 && c.m_Data1->GetVal() == 3 && d.m_Data1->GetVal() == 10);

        // 복사 대입 합니다. a, b, d는 동일 Data 입니다.
        b = d;
        EXPECT_TRUE(a.m_Data1.use_count() == 3 && b.m_Data1.use_count() == 3 && c.m_Data1.use_count() == 1 && d.m_Data1.use_count() == 3);

        // a의 Data 값을 수정하면 b, d의 값도 수정됩니다.
        a.m_Data1->SetVal(20);
        EXPECT_TRUE(a.m_Data1->GetVal() == 20 && b.m_Data1->GetVal() == 20 && c.m_Data1->GetVal() == 3 && d.m_Data1->GetVal() == 20);        
    }
    // deleter
    {
        std::shared_ptr<int> a{
            new int{10}, 
            [](int* obj) { // 람다 표현식외에 함수, 함수자 모두 가능합니다.
                delete obj;
                std::cout << "deleter test" << std::endl;
            }
        };  

        std::shared_ptr<int>b{a};
        *b = 20;
        EXPECT_TRUE(*a == 20 && *b == 20);
    }
    // allcator
    {
        std::shared_ptr<int> a{
            new int {10},
            std::default_delete<int>{}, // 기본 deleter
            std::allocator<int>{} // 기본 allocater
        };
        std::shared_ptr<int>b{a};
        *b = 20;
        EXPECT_TRUE(*a == 20 && *b == 20);
    }
    // 별칭 생성자
    {
        class SubData {
            int m_Val{0};
        public:
            SubData() {std::cout << "SubData : Constructor" << std::endl;}
            ~SubData() {std::cout << "SubData : Destructor" << std::endl;}

            int GetVal() const {return m_Val;}
            void SetVal(int val) {m_Val = val;}
        };
        class MainData {
            SubData m_SubData;
        public:
            MainData() {std::cout << "MainData : Constructor" << std::endl;}
            ~MainData() {std::cout << "MainData : Destructor" << std::endl;}            
            SubData& GetSubData() {return m_SubData;}        
        };

        std::shared_ptr<MainData> mainData1{new MainData{}};
        std::shared_ptr<MainData> mainData2{mainData1};
        
        EXPECT_TRUE(mainData1.use_count() == 2 && mainData2.use_count() == 2);

        // 별칭 생성자를 만듭니다. 생성과 소멸은 MainData로 하지만, 참조하는 포인터는 SubData입니다.
        std::shared_ptr<SubData> subData{mainData1, &(mainData1->GetSubData())};

        // mainData1, mainData2, subData에서 사용하므로 3입니다.
        EXPECT_TRUE(mainData1.use_count() == 3 && mainData2.use_count() == 3 && subData.use_count() == 3);

        // subData를 수정하면 MainData에서도 같이 수정됩니다.
        subData->SetVal(10);
        EXPECT_TRUE(mainData1->GetSubData().GetVal() == 10 && mainData2->GetSubData().GetVal() == 10 && subData->GetVal() == 10);
    }
    // owner_before
    {
        std::shared_ptr<int> a{new int{10}};
        std::shared_ptr<int> b{a};

        // a, b는 동일한 개체이므로 < 비교시 !(a < b) && !(b < a). 즉,동등함 
        EXPECT_TRUE(!(a < b) && !(b < a));

        // 비록 값은 10으로 같으나 주소가 다른 영역에 있으므로 동등하지 않음
        b = std::shared_ptr<int>{new int{10}};
        EXPECT_TRUE(!(!(a < b) && !(b < a)));

        // 별칭 생성자로 생성
        // 소유권 개체 : a, 접근 대상 개체 : data
        int data{10};
        std::shared_ptr<int> c{a, &data};

        // a, c 가 접근 대상 개체가 주소가 다른 영역에 있으므로 동등하지 않음
        EXPECT_TRUE(!(!(a < c) && !(c < a)));
       
        // a, c 의 소유권 개체는 동일하므로 동등함
        EXPECT_TRUE(!(a.owner_before(c)) && !(c.owner_before(a)));
    }
    // 형변환 
    {
        class Base {
        public:
            virtual ~Base() {}
            virtual int Func() {return 1;} // #1
        };

        class Derived : public Base {
        public: 
            virtual int Func() override {return 2;} // #2
        };

        std::shared_ptr<Derived> derived{new Derived{}};
        
        // dynamic_cast<std::shared_ptr<Base>>(derived); // (X) 컴파일 오류

        // 관리하는 개체를 Derived에서 Base로 형변환합니다.
        std::shared_ptr<Base> base{std::dynamic_pointer_cast<Base>(derived)};

        // 가상 함수가 잘 호출됩니다.
        EXPECT_TRUE(derived->Func() == 2 && base->Func() == 2);

        // drived, base 총 2개
        EXPECT_TRUE(derived.use_count() == 2 && base.use_count() == 2);
    }
    // 상호 참조
    {
        class Node {
            std::shared_ptr<Node> m_Parent; // 트리의 부모 노드
            std::vector<std::shared_ptr<Node>> m_Children; // 트리의 자식 노드

        public:
            explicit Node(std::shared_ptr<Node> parent) : m_Parent(parent) {
                std::cout << "Node : Constructor" << std::endl;               
            } 
            ~Node() {
                std::cout << "Node : Destructor" << std::endl;    
            }
            void Add(std::shared_ptr<Node> child) {
                m_Children.push_back(child);
            }
        };

        // // root는 부모가 없어서 nullptr로 노드를 생성합니다.
        // std::shared_ptr<Node> root{new Node{std::shared_ptr<Node>{}}};

        // // root를 부모 삼아 2개의 자식 노드를 생성합니다.
        // root->Add(std::shared_ptr<Node>{new Node{root}});
        // root->Add(std::shared_ptr<Node>{new Node{root}});

        // // root 1개 + child 2개 = 3개. root 개체가 소멸되면 참조 카운트만 2로 만드로 Node를 소멸시키지 않습니다.
        // EXPECT_TRUE(root.use_count() == 3);
    }
    // weak_ptr
    {
        class Node {
            std::weak_ptr<Node> m_Parent; // 트리의 부모 노드는 소유권이 없습니다.
            std::vector<std::shared_ptr<Node>> m_Children; // 트리의 자식 노드

        public:
            explicit Node(std::weak_ptr<Node> parent) : m_Parent(parent) {
                std::cout << "Node : Constructor" << std::endl;               
            } 
            ~Node() {
                std::cout << "Node : Destructor" << std::endl;    
            }
            void Add(std::shared_ptr<Node> child) {
                m_Children.push_back(child);
            }
        };

        // root는 부모가 없어서 nullptr로 노드를 생성합니다.
        std::shared_ptr<Node> root{new Node{std::weak_ptr<Node>{}}};

        // root를 부모 삼아 2개의 자식 노드를 생성합니다.
        root->Add(std::shared_ptr<Node>{new Node{root}});
        root->Add(std::shared_ptr<Node>{new Node{root}});

        // root 1개. weak_ptr 참조 카운트는 2개
        EXPECT_TRUE(root.use_count() == 1);
    }
    {
        std::shared_ptr<int> sp1{new int{10}};
        std::shared_ptr<int> sp2{sp1};
        std::weak_ptr<int> wp1{sp1}; // shared_ptr로부터 생성됩니다.

        EXPECT_TRUE(sp1.use_count() == 2 && sp1.use_count() == wp1.use_count()); // weak_ptr::use_count()는 shared_ptr의 참조 카운트 입니다.

        std::weak_ptr<int> wp2{wp1}; // weak_ptr로부터 생성됩니다.

        // *wp1 = 20; // weak_ptr은 관리하는 개체에 직접 접근할 수 없습니다.
        std::shared_ptr<int> temp1{wp1.lock()}; // shared_ptr을 만들어 접근해야 합니다.
        std::shared_ptr<int> temp2{wp2.lock()};

        *temp1 = 20;
        EXPECT_TRUE(*sp1 == 20 && *sp2 == 20 && *temp1 == 20 && *temp2 == 20);

        EXPECT_TRUE(sp1.use_count() == 4); // sp1, sp2, temp1, temp2 총 4개 입니다.
    }   
    // std::bad_weak_ptr
    {
        std::weak_ptr<int> wp;
        {
            std::shared_ptr<int> sp{new int{10}};
            wp = sp;
        } // sp가 소멸되었습니다.
        try {
            // 소멸된 sp를 사용하는 wp로 shared_ptr을 만듭니다.
            // bad_weak_ptr 예외가 발생합니다.
            std::shared_ptr<int> error{wp};
        }
        catch(std::bad_weak_ptr&) {
            std::cout << "bad_weak_ptr" << std::endl;
        }
    }
#if 201703L <= __cplusplus // C++17~    
    // C++17 배열 지원
    {
        std::unique_ptr<int[]> ptr{new int[3]{0, 1, 2}}; // 배열 개체. delete[] 를 호출하여 2개가 소멸됩니다.
        EXPECT_TRUE(ptr[0] == 0 && ptr[1] == 1 && ptr[2] == 2);
    }
#endif    
}