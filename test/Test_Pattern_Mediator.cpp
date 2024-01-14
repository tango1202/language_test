#include "gtest/gtest.h" 

TEST(TestPattern, Mediator) {

    class Control;

    // ----
    // #1. 컨트롤의 변동사항을 통지받아 다른 컨트를들을 변경합니다.
    // ----
    class IMediator {
    protected:
        IMediator() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~IMediator() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        IMediator(const IMediator&) = delete;
        IMediator(IMediator&&) = delete;
        IMediator& operator =(const IMediator&) = delete;
        IMediator& operator =(IMediator&&) = delete;
    public:
        // 컨트롤에서 변경사항을 통지합니다.
        virtual void Changed(const Control& control) = 0;
    }; 

    // ----
    // #2. 컨트롤의 변동사항이 있을 경우, IMediator의 Changed()를 호출해 줍니다.
    // ----
    class Control {
        IMediator* m_Mediator;
    protected:
        Control() : m_Mediator{nullptr} {} // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~Control() = default; // 다형 소멸 하도록 public virtual   
    private:
        Control(const Control&) = delete;
        Control(Control&&) = delete;
        Control& operator =(const Control&) = delete;
        Control& operator =(Control&&) = delete;

    public:
        void SetMediator(IMediator* mediator) {m_Mediator = mediator;} 
    protected:
        void Changed() {
            if (m_Mediator) {m_Mediator->Changed(*this);}
        }
    };
    // ----
    // #3. Control의 구체한 개체입니다.
    // ----

    // 리스트의 선택 항목이 변경되면, Mediator의 Changed()를 호출해 줍니다.
    class List : public Control {
        std::vector<std::pair<std::string, std::string>> m_Data;
        size_t m_SelectedIndex;
    public: 
        List() : Control{}, m_SelectedIndex{0} {} 
    
        size_t GetSelectedIndex() const {return m_SelectedIndex;}

        void Add(const char* name, const char* addr) {
            assert(name && addr);

            m_Data.emplace_back(name, addr);
        }
        // 이름-주소를 리턴합니다.
        std::pair<std::string, std::string> GetAt(int index) {
            assert(index < m_Data.size());

            return m_Data[index];
        }

        void Select(size_t index) {
            assert(index < m_Data.size());

            m_SelectedIndex = index;
            Changed();
        } 
    };
    // Edit의 문자열이 변경되면, Mediator의 Changed를 호출해 줍니다.
    class Edit : public Control {
        std::string m_String;
    public: 
        explicit Edit() : Control{} {} 
    
        const std::string& GetString() const {return m_String;}
        void SetString(const std::string& str) {
            m_String = str;
            Changed();
        } 
    };

    // ----
    // #4. IMediator를 구체화한 개체입니다.
    // List의 선택 항목이 변경되면, 각 Edit의 문자열을 선택한 항목으로 변경합니다. 
    // ----
    class MyMediator : public IMediator {
        List* m_List;
        Edit* m_NameEdit; 
        Edit* m_AddrEdit;
    public:
        // 생성시 각 컨트롤의 Mediator를 설정합니다.
        MyMediator(List* list, Edit* nameEdit, Edit* addrEdit) : m_List{list}, m_NameEdit{nameEdit}, m_AddrEdit{addrEdit} {
            assert(m_List && m_NameEdit && m_AddrEdit);
 
            m_List->SetMediator(this);
            m_NameEdit->SetMediator(this);
            m_AddrEdit->SetMediator(this);
        }

        // 컨트롤중 리스트가 변경되면 이름과 주소를 갱신합니다.
        virtual void Changed(const Control& control) override {
            assert(m_List && m_NameEdit && m_AddrEdit);

            if (dynamic_cast<const List*>(&control) == m_List) {
                size_t index{m_List->GetSelectedIndex()};
                std::pair<std::string, std::string> data{m_List->GetAt(index)};

                m_NameEdit->SetString(data.first);
                m_AddrEdit->SetString(data.second);
            }
        }
    };
    
    // ----
    // 테스트 코드
    // ----  
    List list;
    list.Add("Kim", "Seoul");
    list.Add("Lee", "Pusan");
    Edit nameEdit;
    Edit addrEdit;

    // 내부적으로 각 컨트롤의 Mediator를 설정합니다. 
    MyMediator myMediator{&list, &nameEdit, &addrEdit};

    // 최초에는 nameEdit와 addrEdit가 비었습니다.
    EXPECT_TRUE(nameEdit.GetString().empty() && addrEdit.GetString().empty());

    // #4. 선택한 항목의 데이터로 nameEdit와 addrEdit가 변경됩니다.
    list.Select(0);
    EXPECT_TRUE(nameEdit.GetString() == "Kim" && addrEdit.GetString() == "Seoul");
    list.Select(1);
    EXPECT_TRUE(nameEdit.GetString() == "Lee" && addrEdit.GetString() == "Pusan");
}