#include "gtest/gtest.h"
#include <vector>

namespace {

    // 테스트하려는 클래스 입니다.
    class MyVector {
    private:
        std::vector<int> m_Data;
    public:
        void Add(int a) {m_Data.push_back(a);}
        int GetAt(int i) const {return m_Data[i];}
        int GetCount() const {return m_Data.size();}
    };

    // MyVector를 사용하는 Fixture 입니다.
    class MyFixture : 
        public ::testing::Test {
    protected:
        // protected여서 이를 상속받은 테스트에서 접근 가능합니다.
        MyVector m_MyData;
    protected:
        // 각 테스트에 사용할 데이터를 준비합니다.
        void SetUp() override {
            m_MyData.Add(1); 
            m_MyData.Add(2);
        }
        // 테스트가 종료되면 데이터를 정리합니다.
        void TearDown() override {
            // MyVector는 vector만 있으므로 소멸자에서 알아서 정리됩니다.
        }
    };
}

// MyFixture 는 사용하는 Fixture의 클래스 이름입니다.
TEST_F(MyFixture, Test1) {
    EXPECT_TRUE(m_MyData.GetAt(0) == 1);
    EXPECT_TRUE(m_MyData.GetCount() == 2);
    m_MyData.Add(3); 
    EXPECT_TRUE(m_MyData.GetCount() == 3);
}
// MyFixture 는 사용하는 Fixture의 클래스 이름입니다.
TEST_F(MyFixture, Test2) {
    EXPECT_TRUE(m_MyData.GetAt(1) == 2);
    EXPECT_TRUE(m_MyData.GetCount() == 2); // Test1에서 1개 더 넣었지만, Fixture는 새롭게 생성된 것입니다. 그러므로 2
}