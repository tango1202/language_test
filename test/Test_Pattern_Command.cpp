#include "gtest/gtest.h" 

namespace {

    // ----
    // #1. Command 입니다. Execute()를 제공합니다.
    // ----
    class Command {
    protected:
        Command() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~Command() = default; // 다형 소멸 하도록 public virtual   
    private:
        Command(const Command&) = delete; 
        Command(Command&&) = delete; 
        Command& operator =(const Command&) = delete; 
        Command& operator =(Command&&) = delete;   
    public:
        virtual void Execute() = 0;     
    };
    // ----
    // #2. Invoker 입니다. Click()시 연결된 Command를 Execute() 합니다.
    // ----
    class Button {
        std::shared_ptr<Command> m_Command;
    public:
        explicit Button(std::shared_ptr<Command> command) : m_Command{command} {}
        ~Button() = default;
    private:
        Button(const Button&) = delete; 
        Button(Button&&) = delete; 
        Button& operator =(const Button&) = delete; 
        Button& operator =(Button&&) = delete;   
    public:
        void Click() {
            m_Command->Execute(); // #2
        }
    };

    // ----
    // #3. Receiver 입니다. Command의 Execute()시 실행할 인터페이스입니다.
    // ----
    class ICommandReceiver {
    protected:
        ICommandReceiver() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~ICommandReceiver() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        ICommandReceiver(const ICommandReceiver&) = delete;
        ICommandReceiver(ICommandReceiver&&) = delete;
        ICommandReceiver& operator =(const ICommandReceiver&) = delete;
        ICommandReceiver& operator =(ICommandReceiver&&) = delete;   
    public:
        virtual void Open() = 0;
        virtual void Save() = 0;
        virtual void Copy() = 0;
        virtual void Paste() = 0;
    };
    // ----
    // #4. Receiver 를 구체화한 클래스 입니다.
    // ----
    class MyApp : public ICommandReceiver {
     public:
        MyApp() = default; 
        ~MyApp() = default;
    private:
        MyApp(const MyApp&) = delete; 
        MyApp(MyApp&&) = delete; 
        MyApp& operator =(const MyApp&) = delete; 
        MyApp& operator =(MyApp&&) = delete; 

        virtual void Open() override {std::cout << "MyApp::Open()" << std::endl;}
        virtual void Save() override {std::cout << "MyApp::Save()" << std::endl;}
        virtual void Copy() override {std::cout << "MyApp::Copy()" << std::endl;}
        virtual void Paste() override {std::cout << "MyApp::Paste()" << std::endl;}               
    };
    // ----
    // #5. Command 를 구체화한 클래스 입니다.
    // ----
    class OpenCommand : public Command {
        ICommandReceiver& m_Receiver;
    public:
        explicit OpenCommand(ICommandReceiver& receiver) : m_Receiver{receiver} {}

        virtual void Execute() override {m_Receiver.Open();}
    };
    class SaveCommand : public Command {
        ICommandReceiver& m_Receiver;
    public:
        explicit SaveCommand(ICommandReceiver& receiver) : m_Receiver{receiver} {}

        virtual void Execute() override {m_Receiver.Save();}
    };
    class CopyCommand : public Command {
        ICommandReceiver& m_Receiver;
    public:
        explicit CopyCommand(ICommandReceiver& receiver) : m_Receiver{receiver} {}

        virtual void Execute() override {m_Receiver.Copy();}
    };
    class PasteCommand : public Command {
        ICommandReceiver& m_Receiver;
    public:
        explicit PasteCommand(ICommandReceiver& receiver) : m_Receiver{receiver} {}

        virtual void Execute() override {m_Receiver.Paste();}
    }; 

    // ----
    // #6. 여러개의 Command 들을 Execute() 합니다. 매크로 구현시 활용할 수 있습니다.
    // ----   
    class CompositeCommand : public Command {
        std::vector<std::shared_ptr<Command>> m_Children;    

    public:
        CompositeCommand() = default;

        virtual void Execute() override {
            for (auto& child : m_Children) { // 하위의 모든 Command를 실행합니다.
                child->Execute();
            }
        }
        void Add(std::shared_ptr<Command> child) {
            assert(child);
            m_Children.push_back(child);
        }
    };       
}

TEST(TestPattern, Command) {
    // ----
    // 테스트 코드
    // ----
    MyApp myApp;

    // Command 들을 생성합니다.
    std::shared_ptr<Command> openCommand{new OpenCommand{myApp}};
    std::shared_ptr<Command> saveCommand{new SaveCommand{myApp}};
    std::shared_ptr<Command> copyCommand{new CopyCommand{myApp}};
    std::shared_ptr<Command> pasteCommand{new PasteCommand{myApp}};
    std::shared_ptr<CompositeCommand> copyPasteCommand{new CompositeCommand{}};
    copyPasteCommand->Add(copyCommand); // #6. 여러개의 Command를 추가합니다.
    copyPasteCommand->Add(pasteCommand);   

    // #7. 버튼과 연결합니다.
    Button openButton{openCommand};
    Button saveButton{saveCommand};
    Button copyButton{copyCommand};
    Button pasteButton{pasteCommand};
    Button copyPasteButton{copyPasteCommand};

    // #7. 버튼 클릭시 Command가 실행됩니다.
    openButton.Click();
    saveButton.Click();
    copyButton.Click();
    pasteButton.Click();
    copyPasteButton.Click(); // #6. 여러개의 Command가 실행됩니다.
}