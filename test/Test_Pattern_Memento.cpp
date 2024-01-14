#include "gtest/gtest.h" 

TEST(TestPattern, Memento) {

    // ----
    // #2. Rectangle을 Move()후 복원할 수 있는 정보를 저장합니다.
    // ----
    class RectangleMoveMemento {
        int m_Left;
        int m_Top;
    public:
        RectangleMoveMemento(int left, int top) : m_Left{left}, m_Top{top} {}

        int GetLeft() const {return m_Left;}
        int GetTop() const {return m_Top;}
    };    

    // ----
    // #1. Originator 입니다. CreateMoveMemento()에서 Move()를 복원할 할 수 있는 정보를 제공하며, SetMemento()에서 복원합니다.
    // ----
    class Rectangle {
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;
    public:
        Rectangle(int l, int t, int w, int h) : m_Left{l}, m_Top{t}, m_Width{w}, m_Height{h} {}

        int GetLeft() const {return m_Left;}
        int GetTop() const {return m_Top;}

        void Move(int left, int top) {
            m_Left = left;
            m_Top = top;
        }

        // #1. Move() 시 수정된 속성을 복원할 수 있는 정보를 리턴합니다.
        std::unique_ptr<RectangleMoveMemento> CreateMoveMemento() {
            return std::unique_ptr<RectangleMoveMemento>{new RectangleMoveMemento{m_Left, m_Top}};
        }

        // #1. Move() 시 수정된 속성을 복원합니다.
        void SetMemento(const RectangleMoveMemento& memento) {
            m_Left = memento.GetLeft();
            m_Top = memento.GetTop();
        }
    }; 

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
        virtual Rectangle& GetSelected() = 0;

        virtual void Move(int left, int top) = 0;
    };

    class MyApp : public ICommandReceiver {
        std::vector<std::unique_ptr<Rectangle>> m_Rectangles;
        size_t m_SelectedIndex;
    public:
        MyApp() : m_SelectedIndex{0} {}

        virtual Rectangle& GetSelected() override { // 선택된 개체를 리턴합니다.
            assert(m_SelectedIndex < m_Rectangles.size());

            return *m_Rectangles[m_SelectedIndex];
        }
        virtual void Move(int left, int top) {
            GetSelected().Move(left, top);
        }

        void Add(std::unique_ptr<Rectangle> rectangle) { // 개체를 추가합니다.
            assert(rectangle);

            m_Rectangles.emplace_back(rectangle.release());
        } 
        void Select(size_t index) { // 관리중인 개체를 선택합니다.
            assert(index < m_Rectangles.size());
            
            m_SelectedIndex = index;
        }
    };

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
        virtual void Unexecute() = 0;   
    };

    // ----
    // #3. Execute()시 Memento를 생성하고, Unexecute()시 수정한 값을 복원합니다.
    // ----
    class MoveCommand : public Command {
        ICommandReceiver& m_Receiver;
        int m_Left;
        int m_Top;
        std::unique_ptr<RectangleMoveMemento> m_Memento;
    public:
        explicit MoveCommand(ICommandReceiver& receiver, int left, int top) : m_Receiver{receiver}, m_Left{left}, m_Top{top} {}

        virtual void Execute() override {
            Rectangle& rectangle{m_Receiver.GetSelected()};
            m_Memento = rectangle.CreateMoveMemento();
            assert(m_Memento);

            m_Receiver.Move(m_Left, m_Top);
        }
        virtual void Unexecute() override {
            if (m_Memento) {
                Rectangle& rectangle{m_Receiver.GetSelected()};
                rectangle.SetMemento(*m_Memento);
            }
        }
    };

    // ----
    // 테스트 코드
    // ----  
    MyApp myApp;

    myApp.Add(std::unique_ptr<Rectangle>{new Rectangle{1, 2, 10, 20}});
    myApp.Select(0);
    const Rectangle& rectangle{myApp.GetSelected()};

    MoveCommand moveCommand{myApp, 3, 4};
   
    moveCommand.Execute(); // #4. 왼쪽 상단값을 수정합니다.
    EXPECT_TRUE(rectangle.GetLeft() == 3 && rectangle.GetTop() == 4);

    moveCommand.Unexecute(); // #4. 이전 값으로 되돌립니다.
    EXPECT_TRUE(rectangle.GetLeft() == 1 && rectangle.GetTop() == 2);
}