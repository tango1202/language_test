#include "gtest/gtest.h" 


namespace {
    // ----
    // #1. Component 입니다. 단일 개체와 복합 개체용 인터페이스를 함께 정의합니다.
    // ----
    class Control {
    protected:
        Control() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~Control() = default; // 다형 소멸 하도록 public virtual
    private:
        Control(const Control&) = delete;
        Control(Control&&) = delete;
        Control& operator =(const Control&) = delete;
        Control& operator =(Control&&) = delete;          
    public:
        // #1. 단일 개체용 인터페이스입니다.
        virtual void SetEnable(bool val) = 0;

        // #2. 복합 개체용 인터페이스입니다.
        virtual void Add(std::unique_ptr<Control> child) = 0;
        virtual void Remove(int index) = 0;
        virtual Control& GetChild(int index) = 0;
    };

    // ----
    // #2. 단일 개체의 구현입니다. 복합 개체용 함수들은 예외를 발생시킵니다.
    // ----
    class Label : public Control {
    public:
        Label() = default;

         virtual void SetEnable(bool val) override {
            std::cout << "Label::SetEnable()" << std::endl;
        }      
        virtual void Add(std::unique_ptr<Control> child) override {
            throw "Can not support"; // #2
        }
        virtual void Remove(int index) override {
            throw "Can not support"; // #2
        }
        virtual Control& GetChild(int index) override {
            throw "Can not support"; // #2
        }
    };
    class Edit : public Control {
    public:
        Edit() = default;

        virtual void SetEnable(bool val) override {
            std::cout << "Edit::SetEnable()" << std::endl;
        }  
        virtual void Add(std::unique_ptr<Control> child) override {
            throw "Can not support"; // #2
        }
        virtual void Remove(int index) override {
            throw "Can not support"; // #2
        }
        virtual Control& GetChild(int index) override {
            throw "Can not support"; // #2
        }
    };

    // ----
    // #3. 복합 개체인 Composite 입니다. SetEnable() 실행시 모든 하위 개체에 적용합니다.
    // ----
    class Panel : public Control {
        std::vector<std::shared_ptr<Control>> m_Children;
    public:
        Panel() = default;

        // Child들의 SetEnable들을 설정합니다.
        virtual void SetEnable(bool val) override {
            for (auto& child : m_Children) { // #3
                child->SetEnable(val);
            }
        } 
        virtual void Add(std::unique_ptr<Control> child) override {
            assert(child);
            m_Children.emplace_back(child.release());
        }
        virtual void Remove(int index) override {
            assert(index < m_Children.size());

            m_Children.erase(m_Children.begin() + index);
        }
        virtual Control& GetChild(int index) override {
            assert(index < m_Children.size());

            return *m_Children[index];
        }
    };
}

TEST(TestPattern, Composite) {

    // ----
    // 테스트 코드
    // ----   
    std::unique_ptr<Control> subPanel{new Panel};
    subPanel->Add(std::unique_ptr<Control>{new Label});
    subPanel->Add(std::unique_ptr<Control>{new Edit});   

    Panel rootPanel;
    rootPanel.Add(std::unique_ptr<Control>{new Label});
    rootPanel.Add(std::unique_ptr<Control>{new Edit});
    rootPanel.Add(std::move(subPanel)); // 하위 Panel을 추가합니다.

    rootPanel.SetEnable(true); // #4. 모든 하위 개체들의 SetEnable()을 실행합니다.
}