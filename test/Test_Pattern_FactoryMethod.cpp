#include "gtest/gtest.h" 


namespace {
    // ----
    // #1. 프레임워크에서 제공하는 부모 클래스입니다.
    // ----
    class Doc {
    protected:
        Doc() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        Doc(const Doc&) = delete; 
        Doc(Doc&&) = delete; 
        Doc& operator =(const Doc&) = delete; 
        Doc& operator =(Doc&&) = delete;   
    public:
        virtual ~Doc() = default; // 다형 소멸 하도록 public virtual

        bool IsDirty() const {return false;} // 테스트용으로 그냥 false를 리턴합니다.
        virtual void Load() = 0;
        virtual void Save() = 0;
    };

    class App {
    private:
        std::unique_ptr<Doc> m_Doc;
    protected:
        App() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    private:
        App(const App&) = delete;
        App(const App&&) = delete;
        App& operator =(const App&) = delete;
        App& operator =(App&&) = delete;   
    public:
        virtual ~App() = default; // 다형 소멸 하도록 public virtual

    protected:
        // 자식 개체에서만 호출 가능하도록 protected입니다.
        // #2. 자식 개체에서 어떤 Doc을 생성할지 결정합니다.
        virtual std::unique_ptr<Doc> CreateDoc() = 0;

    public:   
        void LoadDoc() {
            
            // 더럽혀 졌다면, 불러오지 않습니다.
            if (m_Doc && m_Doc->IsDirty()) {
                std::cout << "Please. Save Doc." << std::endl;
                return;
            }

            std::unique_ptr<Doc> doc{CreateDoc()};
            
            doc->Load();

            m_Doc = std::move(doc);
        }
        
        void SaveDoc() {
            if (!m_Doc) {
                m_Doc = CreateDoc(); // Doc이 만들어지지 않았자면 지연 생성합니다.
            }

            // 더렵혀 지지 않았다면 저장하지 않습니다.
            if (!m_Doc->IsDirty()) {
                std::cout << "Already Saved." << std::endl;
                return;
            }

            m_Doc->Save();
        }
    };
    // ----
    // 자식 클래스입니다.
    // ----
    class MyDoc : public Doc { // #2
    public:
        MyDoc() = default;

        virtual void Load() override {
            std::cout << "MyDoc : Load()" << std::endl;
        }
        virtual void Save() override {
            std::cout << "MyDoc : Save()" << std::endl;
        }    
    };

    class MyApp : public App { // #3
    public:
        MyApp() = default;

    protected:    
        // #3. 자식 개체에서 어떤 Doc을 생성할지 결정합니다.
        virtual std::unique_ptr<Doc> CreateDoc() override {
            return std::unique_ptr<Doc>(new MyDoc{});
        }
    };

}

TEST(TestPattern, FactoryMethod) {

    {
        MyApp app;

        app.LoadDoc();
        app.SaveDoc();
    }
}