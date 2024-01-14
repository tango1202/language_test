#include "gtest/gtest.h" 

#include <list>

namespace {

    // ----
    // #1. Iterator 입니다. IAggregate 각 요소에 접근하는 함수들을 제공합니다.
    // ----
    class Iterator {
    protected:
        Iterator() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~Iterator() = default; // 다형 소멸 하도록 public virtual
    private:
        Iterator(const Iterator&) = delete; 
        Iterator(Iterator&&) = delete; 
        Iterator& operator =(const Iterator&) = delete; 
        Iterator& operator =(Iterator&&) = delete; 
    public:
        virtual void First() = 0; // 집합 개체의 처음으로 이동합니다.
        virtual void Next() = 0; // 다음 위치로 이동합니다.
        virtual bool IsDone() const = 0; // 끝인지 검사합니다. 
        virtual int GetCurrent() const = 0; // 현 위치의 값을 리턴합니다.
    };

    // ----
    // #2. 집합 개체로서 CreateIterator()를 제공해야 합니다.   
    // ----
    class IAggregate {
    protected:
        IAggregate() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~IAggregate() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        IAggregate(const IAggregate&) = delete;
        IAggregate(IAggregate&&) = delete;
        IAggregate& operator =(const IAggregate&) = delete;
        IAggregate& operator =(IAggregate&&) = delete;    
    public:
        virtual std::unique_ptr<Iterator> CreateIterator() = 0; // 이터레이터를 생성합니다.
        virtual size_t GetCount() const = 0; // 요소의 갯수를 구합니다.
        virtual void Add(int val) = 0; // 요소를 추가합니다.
        virtual void Remove(int index) = 0; // 요소를 삭제합니다.
    };
    // ----
    // #3. IAggregate를 구체화하며, CreateIterator()시 MyIterator를 생성합니다.
    // ----
    class MyAggregate : public IAggregate {
        std::vector<int> m_Container;
    public:
        MyAggregate() = default;

        // 내부적으로 MyInterator를 사용하여, 선언과 정의를 분리했습니다.
        virtual std::unique_ptr<Iterator> CreateIterator() override; // 이터레이터를 생성합니다. 
        virtual size_t GetCount() const override {return m_Container.size();} // 요소의 갯수를 구합니다.
        virtual void Add(int val) override {m_Container.push_back(val);} // 요소를 추가합니다.
        virtual void Remove(int index) override { // 요소를 삭제합니다.
            assert(index < m_Container.size());

            m_Container.erase(m_Container.begin() + index);
        } 
    
        int GetAt(int index) const {
            assert(index < m_Container.size());

            return m_Container[index];
        }
    };
    // ----
    // #4. Iterator를 구체화합니다. m_CurrentIndex로 현재 요소 위치를 관리합니다.
    // ----
    class MyIterator : public Iterator {
        MyAggregate& m_MyAggregate;
        int m_CurrentIndex; // 현재 요소의 위치입니다.
    public:
        explicit MyIterator(MyAggregate& myAggregate) : m_MyAggregate{myAggregate}, m_CurrentIndex{0} {}

        virtual void First() override {m_CurrentIndex = 0;} // 집합 개체의 처음으로 이동합니다.
        virtual void Next() override {++m_CurrentIndex;} // 다음 위치로 이동합니다.
        virtual bool IsDone() const override {return m_CurrentIndex < m_MyAggregate.GetCount() ? false : true;} // 끝인지 검사합니다. 
        virtual int GetCurrent() const override { // 현 위치의 값을 리턴합니다.
            assert(m_CurrentIndex < m_MyAggregate.GetCount());
            
            return m_MyAggregate.GetAt(m_CurrentIndex);
        } 
    };

    std::unique_ptr<Iterator> MyAggregate::CreateIterator() {
        return std::unique_ptr<Iterator>{new MyIterator{*this}}; // #3
    }
}

TEST(TestPattern, Iterator) {
    {
        std::vector<int> v;
        std::vector<int>::iterator itr{v.begin()};
        std::vector<int>::iterator endItr{v.end()};

        for (;itr != endItr; ++itr) {
            // vector 의 각 요소에 접근합니다.
        }        
    }
    {
        std::list<int> l;
        std::list<int>::iterator itr{l.begin()};
        std::list<int>::iterator endItr{l.end()};

        for (;itr != endItr; ++itr) {
            // list 의 각 요소에 접근합니다.
        }     
    }
    {
        // ----
        // 테스트 코드
        // ----        
        MyAggregate aggregate;
        aggregate.Add(10);
        aggregate.Add(20);
        aggregate.Add(30);

        std::unique_ptr<Iterator> itr{aggregate.CreateIterator()}; // #3
        
        itr->First();
        if (!itr->IsDone()) {EXPECT_TRUE(itr->GetCurrent() == 10);}
       
        itr->Next();
        if (!itr->IsDone()) {EXPECT_TRUE(itr->GetCurrent() == 20);}

        itr->Next();
        if (!itr->IsDone()) {EXPECT_TRUE(itr->GetCurrent() == 30);}
    }
}