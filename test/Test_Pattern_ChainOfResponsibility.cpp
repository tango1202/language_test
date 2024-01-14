#include "gtest/gtest.h" 


namespace {
    enum BorderStyle {Width, Color};
    class Border {
        std::unique_ptr<unsigned int> m_Width;
        std::unique_ptr<unsigned int> m_Color;
    public:
        Border() = default; // 속성이 세팅되지 않은 기본 Border를 생성합니다.
        ~Border() = default;
    private:
        Border(const Border&) = delete; 
        Border(Border&&) = delete; 
        Border& operator =(const Border&) = delete; 
        Border& operator =(Border&&) = delete; 
    public:
        unsigned int GetWidth() const {
            if (!m_Width) {
                return 0; // 설정되지 않으면 기본적으로 0 입니다.
            } 
            return *m_Width;
        }
        unsigned int GetColor() const {
            if (!m_Color) {
                return 0x00000000; // 설정되지 않으면 기본적으로 0입니다.
            }
            return *m_Color;
        }
        void SetWidth(unsigned int val) {
            m_Width = std::unique_ptr<unsigned int>{new unsigned int{val}};
        }
        void SetColor(unsigned int val) {
            m_Color = std::unique_ptr<unsigned int>{new unsigned int{val}};
        }      
        // ----
        // #6. this에 없는 속성은 other의 것을 사용하여 합칩니다.
        // ----
        void Merge(const Border& other) {
            if (!m_Width && other.m_Width) {
                SetWidth(*other.m_Width);
            }
            if (!m_Color && other.m_Color) {
                SetColor(*other.m_Color);
            }
        }
    };

    // ----
    // #2. Handler 입니다. MergeBorder()를 연결된 다음 개체에 요청합니다.
    // ----
    class IStyledBorderHandler {
    protected:
        IStyledBorderHandler() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~IStyledBorderHandler() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        IStyledBorderHandler(const IStyledBorderHandler&) = delete;
        IStyledBorderHandler(IStyledBorderHandler&&) = delete;
        IStyledBorderHandler& operator =(const IStyledBorderHandler&) = delete;
        IStyledBorderHandler& operator =(IStyledBorderHandler&&) = delete;   
    public:
        // border에 스타일을 처리를 누적합니다.
        virtual void MergeBorder(Border& border) const = 0;
    };

    // ----
    // #1. Composite 패턴으로 Control, Label, Edit, Panel을 구현합니다.
    // ----
    class Control : public IStyledBorderHandler { // #3
        std::unique_ptr<Border> m_Border;
        const IStyledBorderHandler* m_Successor; // #4
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
        void SetBorder(BorderStyle style, unsigned int val) {
            if (!m_Border) {
                m_Border = std::unique_ptr<Border>{new Border};
            }
            switch (style) {
            case BorderStyle::Width:
                m_Border->SetWidth(val);
                break;
            case BorderStyle::Color:
                m_Border->SetColor(val);
                break;
            }
        }
        // ----
        // #5. 자기의 속성과 다음 개체의 속성을 누적합니다.
        // ----
        virtual void MergeBorder(Border& border) const override { 
            
            if (m_Border) { // 자기 자신의 Border를 누적합니다.
                border.Merge(*m_Border); 
            }
            if (m_Successor) { // 다음 개체의 속성을 누적합니다.
                m_Successor->MergeBorder(border);
            }
        }
        void SetSuccessor(const IStyledBorderHandler* successor) { // #4
            m_Successor = successor;
        }

        // 복합 개체용 인터페이스입니다.
        virtual void Add(std::unique_ptr<Control> child) = 0;
        virtual Control& GetChild(int index) = 0;
    };

    class Label : public Control {
    public:
        Label() = default;

        virtual void Add(std::unique_ptr<Control> child) override {
            throw "Can not support"; 
        }
        virtual Control& GetChild(int index) override {
            throw "Can not support";
        }
    };
    class Edit : public Control {
    public:
        Edit() = default;

        virtual void Add(std::unique_ptr<Control> child) override {
            throw "Can not support"; 
        }
        virtual Control& GetChild(int index) override {
            throw "Can not support";
        }
    };

    class Panel : public Control {
        std::vector<std::shared_ptr<Control>> m_Children;
    public:
        Panel() = default;

        virtual void Add(std::unique_ptr<Control> child) override {
            assert(child);

            child->SetSuccessor(this); // #4
            m_Children.emplace_back(child.release());
        }
        virtual Control& GetChild(int index) override {
            assert(index < m_Children.size());
            
            return *m_Children[index];
        }
    };
}

TEST(TestPattern, ChainOfResponsibility) {

    // ----
    // 테스트 코드
    // ----
    std::unique_ptr<Control> subPanel{new Panel};
    subPanel->SetBorder(BorderStyle::Width, 3); // subPanel의 테두리의 굵기는 3으로 설정합니다.

    std::unique_ptr<Control> subPanelLabel{new Label};
    subPanelLabel->SetBorder(BorderStyle::Width, 1);
    subPanel->Add(std::move(subPanelLabel)); // 라벨의 두께는 직접 설정합니다.

    subPanel->Add(std::unique_ptr<Control>{new Edit}); // 상위 속성을 따릅니다.  

    Panel rootPanel;
    rootPanel.SetBorder(BorderStyle::Color, 0x000000FF); // 0x0000 00FF 색으로 설정합니다.
    rootPanel.Add(std::unique_ptr<Control>{new Label});
    rootPanel.Add(std::unique_ptr<Control>{new Edit});
    rootPanel.Add(std::move(subPanel)); // 하위 Panel을 추가합니다.
    
    Border border1;
    rootPanel.GetChild(0).MergeBorder(border1); // rootPanel의 Label 속성입니다.
    EXPECT_TRUE(border1.GetWidth() == 0); // 디폴트 속성을 따릅니다.
    EXPECT_TRUE(border1.GetColor() == 0x000000FF); // root에 설정된 색상을 따릅니다.

    Border border2;
    rootPanel.GetChild(2).GetChild(0).MergeBorder(border2); // subPanel의 Label 속성입니다.
    EXPECT_TRUE(border2.GetWidth() == 1); // 직접 설정한 속성을 따릅니다.
    EXPECT_TRUE(border2.GetColor() == 0x000000FF); // root에 설정된 색상을 따릅니다.

    Border border3;
    rootPanel.GetChild(2).GetChild(1).MergeBorder(border3); // subPanel의 Edit 속성입니다.
    EXPECT_TRUE(border3.GetWidth() == 3); // subPanel에 설정한 속성을 따릅니다.
    EXPECT_TRUE(border3.GetColor() == 0x000000FF); // root에 설정된 색상을 따릅니다.

}