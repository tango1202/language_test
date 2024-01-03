#include "gtest/gtest.h" 


namespace {
    class Control {
    public:
        virtual ~Control() = default; // 다형 소멸 하도록 public virtual
    };
    class Label : public Control {
    public:
        explicit Label(const char* caption) {}
    };
    class Edit : public Control {};
    class Ok : public Control {};
    class Cancel : public Control {};
    class Panel : public Control {
        std::vector<std::shared_ptr<Control>> m_Controls;
    public:
        void Add(std::unique_ptr<Control> control) {
            m_Controls.emplace_back(control.release());
        }
    };

    // ----
    // 기본 인터페이스
    // ----
    class IControlBuilder { // #1
    protected:
        IControlBuilder() = default; // 상속해서만 사용하도록 protected
    private:
        IControlBuilder(const IControlBuilder&) = delete;
        IControlBuilder(IControlBuilder&&) = delete;
        IControlBuilder& operator =(const IControlBuilder&) = delete;
        IControlBuilder& operator =(IControlBuilder&&) = delete;
    protected:
        ~IControlBuilder() = default; // 다형 소멸을 하지 않으므로 protected non-virtual
    public:
        virtual void AddLabel(const char* caption) = 0;
        virtual void AddEdit() = 0;
        virtual void AddOk() = 0;
        virtual void AddCancel() = 0;
    };

    class PanelBuilder : public IControlBuilder { // #2
    private:
        std::unique_ptr<Panel> m_Panel;
    public:
        PanelBuilder() : m_Panel{new Panel{}} {}

        virtual void AddLabel(const char* caption) override {
            m_Panel->Add(std::unique_ptr<Control>(new Label{caption}));
        }
        virtual void AddEdit() override {
            m_Panel->Add(std::unique_ptr<Control>(new Edit{}));
        }
        virtual void AddOk() override {
            m_Panel->Add(std::unique_ptr<Control>(new Ok{}));   
        }
        virtual void AddCancel() override {
            m_Panel->Add(std::unique_ptr<Control>(new Cancel{}));   
        }  

        // #4. 생성된 패널을 리턴하고 내부 멤버 변수는 초기화 합니다.
        std::unique_ptr<Panel> Release() {return std::move(m_Panel);}  
    };

    // ----
    // Builder를 이용하여 요소를 합성합니다.
    // ----
    class IdPasswordDirector { // #3
    private:
        IControlBuilder& m_Builder;
    public:
        explicit IdPasswordDirector(IControlBuilder& builder) : m_Builder{builder} {}

        // ID, Password를 입력받는 Panel을 만듭니다.
        void Construct() {
            m_Builder.AddLabel("ID");
            m_Builder.AddEdit();

            m_Builder.AddLabel("Password");
            m_Builder.AddEdit();

            m_Builder.AddOk();
            m_Builder.AddCancel();
        }    
    };    

    class PanelWriter : public IControlBuilder {
    public:
        PanelWriter() {
            std::cout << "<Panel>" << std::endl;
        }
        ~PanelWriter() {
            std::cout << "</Panel>" << std::endl;
        } 
        virtual void AddLabel(const char* caption) override {
            std::cout << "    <Label caption = \"" << caption << "\"/>" << std::endl; 
        }
        virtual void AddEdit() override {
            std::cout << "    <Edit/>" << std::endl;
        }
        virtual void AddOk() override {
            std::cout << "    <Ok/>" << std::endl;  
        }
        virtual void AddCancel() override {
            std::cout << "    <Cancel/>" << std::endl; 
        } 
    };
    class PanelCounter : public IControlBuilder {
        size_t m_Count;
    public:
        PanelCounter() : m_Count{0} {}
        virtual void AddLabel(const char* caption) override {
            ++m_Count;
        }
        virtual void AddEdit() override {
            ++m_Count;
        }
        virtual void AddOk() override {
            ++m_Count;
        }
        virtual void AddCancel() override {
            ++m_Count;
        } 

        size_t GetCount() const {return m_Count;}
    };    
}

TEST(TestPattern, Builder) {

    {
        PanelBuilder panelBuilder;
        IdPasswordDirector director{panelBuilder};
        director.Construct();
        std::unique_ptr<Panel> panel{panelBuilder.Release()}; // #4. 생성된 panel을 구합니다.
    }
    {
        PanelWriter panelWriter;
        IdPasswordDirector director{panelWriter};
        director.Construct(); // 생성하는 정보를 cout으로 출력합니다.
    }
    {
        PanelCounter panelCounter;
        IdPasswordDirector director{panelCounter};
        director.Construct();
        EXPECT_TRUE(panelCounter.GetCount() == 6); // panel 내에 사용된 control 갯수를 출력합니다.       
    }

}