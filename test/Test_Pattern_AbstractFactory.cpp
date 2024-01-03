#include "gtest/gtest.h" 


namespace {

    // ----
    // 기본 인터페이스
    // ----
    class Button { // #1
    protected:
        Button() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        Button(const Button&) = delete;
        Button(Button&&) = delete;
        Button& operator =(const Button&) = delete;
        Button& operator =(Button&&) = delete;   
    public:
        virtual ~Button() = default; // 다형 소멸 하도록 public virtual

        virtual void Click() = 0; // 버튼을 클릭합니다.
    };
    
    class Check { // #1
    protected:
        Check() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        Check(const Check&) = delete;
        Check(Check&&) = delete;
        Check& operator =(const Check&) = delete;
        Check& operator =(Check&&) = delete;   
    public:
        virtual ~Check() = default; // 다형 소멸 하도록 public virtual

        virtual void Toggle() = 0; // 체크 선택을 토글합니다.
    };

    class RadioGroup { // #1
    protected:
        RadioGroup() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        RadioGroup(const RadioGroup&) = delete;
        RadioGroup(RadioGroup&&) = delete;
        RadioGroup& operator =(const RadioGroup&) = delete;
        RadioGroup& operator =(RadioGroup&&) = delete;   
    public:
        virtual ~RadioGroup() = default; // 다형 소멸 하도록 public virtual

        virtual void Select(size_t index) = 0; // 주어진 인덱스를 선택합니다.
    };

    class ControlFactory { //#2
    protected:
        ControlFactory() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        ControlFactory(const ControlFactory&) = delete;
        ControlFactory(ControlFactory&&) = delete;
        ControlFactory& operator =(const ControlFactory&) = delete;
        ControlFactory& operator =(ControlFactory&&) = delete;
    public:
        ~ControlFactory() = default; // 다형 소멸 하도록 public virtual
    public:
        virtual std::unique_ptr<Button> CreateButton() const = 0;
        virtual std::unique_ptr<Check> CreateCheck() const = 0;
        virtual std::unique_ptr<RadioGroup> CreateRadioGroup() const = 0;
    };

    // ----
    // #3. Normal 계열 클래스
    // ----
    class NormalButton : public Button {
    public: 
        NormalButton() = default;

        virtual void Click() override { // 버튼을 클릭합니다.
            std::cout << "NormalButton : Click()" << std::endl;
        }
    };

    class NormalCheck : public Check {
    public: 
        NormalCheck() = default;

        virtual void Toggle() override { // 체크 선택을 토글합니다.
            std::cout << "NormalCheck : Toggle()" << std::endl;
        }
    };  

     class NormalRadioGroup : public RadioGroup {
    public: 
        NormalRadioGroup() = default;

        virtual void Select(size_t index) override { // 주어진 인덱스를 선택합니다.
            std::cout << "NormalRadioGroup : Select()" << std::endl;
        }
    };     

    // Normal 계열로 생성합니다.
    class NormalFactory : public ControlFactory { 
    public:
        NormalFactory() = default;
    public:
        virtual std::unique_ptr<Button> CreateButton() const override {
            return std::unique_ptr<Button>(new NormalButton{});
        }
        virtual std::unique_ptr<Check> CreateCheck() const override {
            return std::unique_ptr<Check>(new NormalCheck{});

        }
        virtual std::unique_ptr<RadioGroup> CreateRadioGroup() const override {
            return std::unique_ptr<RadioGroup>(new NormalRadioGroup{});
        }
    };

    // ----
    // #3. Mobile 계열 클래스
    // ----
    class MobileButton : public Button {
    public: 
        MobileButton() = default;

        virtual void Click() override { // 버튼을 클릭합니다.
            std::cout << "MobileButton : Click()" << std::endl;
        }
    };

    class MobileCheck : public Check {
    public: 
        MobileCheck() = default;

        virtual void Toggle() override { // 체크 선택을 토글합니다.
            std::cout << "MobileCheck : Toggle()" << std::endl;
        }
    };  

     class MobileRadioGroup : public RadioGroup {
    public: 
        MobileRadioGroup() = default;

        virtual void Select(size_t index) override { // 주어진 인덱스를 선택합니다.
            std::cout << "MobileRadioGroup : Select()" << std::endl;
        }
    };     

    // Mobile 계열로 생성합니다.
    class MobileFactory : public ControlFactory { 
    public:
        MobileFactory() = default;
    public:
        virtual std::unique_ptr<Button> CreateButton() const override {
            return std::unique_ptr<Button>(new MobileButton{});
        }
        virtual std::unique_ptr<Check> CreateCheck() const override {
            return std::unique_ptr<Check>(new MobileCheck{});

        }
        virtual std::unique_ptr<RadioGroup> CreateRadioGroup() const override {
            return std::unique_ptr<RadioGroup>(new MobileRadioGroup{});
        }
    };

    // #4. 모바일 인지 검사합니다.
    bool IsMobile() {
        return true; // 테스트 용으로 그냥 true를 리턴합니다.
    } 

    // #4. 모바일 인지 아닌지에 따라 ControlFactory를 생성합니다.
    std::unique_ptr<ControlFactory> CreateControlFactory() {
        if (IsMobile()) { 
            // Mobile용 팩토리를 사용합니다.
            return std::unique_ptr<ControlFactory>{new MobileFactory{}};
        }
        else {
            // Normal용 팩토리를 사용합니다.
            return std::unique_ptr<ControlFactory>{new NormalFactory{}};
        }
    }

    // #6. 전달받은 factory에 따라 다른 컨트롤을 생성하여 사용합니다.
    void TestAbstractFactory(ControlFactory& factory) {
        factory.CreateButton()->Click();
        factory.CreateCheck()->Toggle();
        factory.CreateRadioGroup()->Select(0);
    }
}

TEST(TestPattern, AbstractFactory) {

    std::unique_ptr<ControlFactory> factory{CreateControlFactory()}; // #4

    TestAbstractFactory(*factory); // #5
}