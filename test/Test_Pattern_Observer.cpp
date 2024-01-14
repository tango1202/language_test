#include "gtest/gtest.h" 

#include <list>
#include <numeric>
#include <algorithm>

TEST(TestPattern, Observer) {

    // ----
    // #1. Subject를 감시합니다.
    // ----
    class IObserver {
    protected:
        IObserver() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~IObserver() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        IObserver(const IObserver&) = delete;
        IObserver(IObserver&&) = delete;
        IObserver& operator =(const IObserver&) = delete;
        IObserver& operator =(IObserver&&) = delete;   
    public:
        // Subject가 수정될때 호출됩니다.
        virtual void Update() = 0;
    };
    // ----
    // #2. Attach()와 Detach()로 IObserver를 등록/해제 할 수 있으며, 변경 사항이 있으면, 모든 IObserver의 Update()를 호출합니다.
    // ----
    class Subject {
    private:
        std::list<IObserver*> m_Observers;
    protected:
        Subject() = default; // 상속해서만 사용하도록 protected
        ~Subject() = default; // 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        Subject(const Subject&) = delete;
        Subject(Subject&&) = delete;
        Subject& operator =(const Subject&) = delete;
        Subject& operator =(Subject&&) = delete;   
    public:
        // Observer를 등록합니다.
        void Attach(IObserver* observer) {
            assert(observer);

            m_Observers.push_back(observer);
        }
        // Observer를 해제합니다.
        void Detach(IObserver* observer) {
            assert(observer);

            std::list<IObserver*>::iterator itr{std::find(m_Observers.begin(), m_Observers.end(), observer)};
            
            if (itr != m_Observers.end()) {
                m_Observers.erase(itr);
            }
        }

        // #2. 변경 사항이 있으면, 모든 Observer의 Update()를 호출합니다.
        void Notify() {
            for(auto observer : m_Observers) {
                observer->Update();
            }
        }
    };
    // ----
    // #3. 데이터가 추가되면 모든 Observer에 통지합니다.
    // ----
    class MyDoc : public Subject {
        std::vector<int> m_Data;
    public:
        MyDoc() = default;
        ~MyDoc() = default;
    private:
        MyDoc(const MyDoc&) = delete;
        MyDoc(MyDoc&&) = delete;
        MyDoc& operator =(const MyDoc&) = delete;
        MyDoc& operator =(MyDoc&&) = delete;  
    public:
        void Add(int val) {
            m_Data.push_back(val);
            Notify(); // #3
        }
        int GetSum() const {
            return std::accumulate(m_Data.begin(), m_Data.end(), 0);
        }
        int GetAver() const {
            assert(0 < m_Data.size());
            
            return GetSum() / m_Data.size();
        } 
    };

    // ----
    // #4. MyDoc을 감시하며, 변경 사항이 있으면, Sum값을 업데이트 합니다.
    // ----
    class SumObserver : public IObserver {
        MyDoc& m_MyDoc;
        int m_Val;
    public:
        explicit SumObserver(MyDoc& myDoc) : m_MyDoc{myDoc} {}
 
        virtual void Update() override {
            m_Val = m_MyDoc.GetSum(); // #4
        }

        int GetVal() const {return m_Val;}
    };

    // ----   
    // #4. MyDoc을 감시하며, 변경 사항이 있으면, Aver값을 업데이트 합니다.
    // ----
    class AverObserver : public IObserver {
        MyDoc& m_MyDoc;
        int m_Val;
    public:
        explicit AverObserver(MyDoc& myDoc) : m_MyDoc{myDoc} {}
 
        virtual void Update() override {
            m_Val = m_MyDoc.GetAver(); // #4
        }

        int GetVal() const {return m_Val;}
    };

    // ----
    // 테스트 코드
    // ----  
    MyDoc myDoc;

    SumObserver sumObserver{myDoc};
    AverObserver averObserver{myDoc};

    myDoc.Attach(&sumObserver); // #2. myDoc을 감시합니다.
    myDoc.Attach(&averObserver);

    myDoc.Add(1);
    myDoc.Add(2);
    myDoc.Add(3);

    EXPECT_TRUE(sumObserver.GetVal() == 1 + 2 + 3);
    EXPECT_TRUE(averObserver.GetVal() == (1 + 2 + 3) / 3);

    myDoc.Detach(&averObserver); // #2. averObserver 감시를 해제 합니다.

    myDoc.Add(10); // #5
    
    EXPECT_TRUE(sumObserver.GetVal() == 1 + 2 + 3 + 10); // sum은 계속 감시중이어서 값이 갱신됩니다.
    EXPECT_TRUE(averObserver.GetVal() == (1 + 2 + 3) / 3); // #5. aver는 감시를 해제하여 값이 갱신되지 않습니다.
}