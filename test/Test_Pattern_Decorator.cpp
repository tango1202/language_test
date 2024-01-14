#include "gtest/gtest.h" 


namespace {

    // ----
    // #1. Component 입니다. Draw() 함수를 재구현해야 합니다.
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
        virtual void Draw() const = 0;
    };  
    // ----
    //  #2. 개체 구현입니다. Draw() 함수를 재구현 하였습니다.
    // ----    
    class Label : public Control {
    public:
        Label() = default;

         virtual void Draw() const override {
            std::cout << "Label::Draw()" << std::endl;
        }      
    };
    class Edit : public Control {
    public:
        Edit() = default;

        virtual void Draw() const override {
            std::cout << "Edit::Draw()" << std::endl;
        }  
    };
    // ----
    //  #3. Decorator 입니다. Draw() 함수 호출시 DrawNewIcon()을 추가로 호출합니다.
    // ----
    class NewIconDecorator : public Control {
        std::unique_ptr<Control> m_Control;
    public:
        explicit NewIconDecorator(std::unique_ptr<Control> control) : m_Control{std::move(control)} {
            assert(m_Control);  
        }

        virtual void Draw() const override {
            assert(m_Control); 

            m_Control->Draw();
            DrawNewIcon(); // #3
        }
    private:
        void DrawNewIcon() const {
            std::cout << "NewBehaviorDecorator::DrawBehavior()" << std::endl; // #3
        }
    };

    class LogDecorator : public Control {
        std::unique_ptr<Control> m_Control;
    public:
        explicit LogDecorator(std::unique_ptr<Control> control) : m_Control{std::move(control)} {
            assert(m_Control); 
        }

        virtual void Draw() const override {
            assert(m_Control); 

            m_Control->Draw();
            Log();
        }
    private:
        void Log() const {
            std::cout << "LogDecorator::Log()" << std::endl;
        }
    };
}

TEST(TestPattern, Decorator) {

    {
        // ----
        // 테스트 코드
        // ----           
        // Draw 호출시 NewIconDecorator인 것은 NewIcon을 추가로 출력합니다.
        std::vector<std::shared_ptr<Control>> v;
        v.emplace_back(std::unique_ptr<Control>{new Label});
        v.emplace_back(std::unique_ptr<Control>{new NewIconDecorator{std::unique_ptr<Control>{new Edit}}}); // #3. 런타임에 NewIcon을 출력할지 결정할 수 있습니다.

        for (auto& control : v) {
            control->Draw();
        }
    }
    {
        // ----
        // 테스트 코드
        // ----        

        // Draw 호출시 로그를 기록합니다.
        std::vector<std::shared_ptr<Control>> v;
        v.emplace_back(std::unique_ptr<Control>{new LogDecorator{std::unique_ptr<Control>{new Label}}});
        v.emplace_back(
            std::unique_ptr<Control>{new LogDecorator{
                std::unique_ptr<Control>{new NewIconDecorator{
                    std::unique_ptr<Control>{new Edit}
                }}
            }}
        );

        for (auto& control : v) {
            control->Draw();
        }       
    }
}