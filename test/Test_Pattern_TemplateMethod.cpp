#include "gtest/gtest.h" 

TEST(TestPattern, TemplateMethod) {
    // ----
    // #1. Load()의 뼈대를 제공합니다. 자식 개체에서 DoSaveMessage(), DoPathNameMessage(), DoSave(), DoLoad()를 구현해야 합니다.
    // ----
    class App {
        const char* m_PathName;
        bool m_Dirty;
    protected:
        explicit App(const char* pathName) : m_PathName{pathName}, m_Dirty{false} {} // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~App() = default; // 다형 소멸 하도록 public virtual
    private:
        App(const App&) = delete;
        App(const App&&) = delete;
        App& operator =(const App&) = delete;
        App& operator =(App&&) = delete; 
    public:
        void Load(const char* pathName) {
            if (IsDirty()) { // 수정되었으면 저장이 필요합니다.
                if (DoSaveMessage()) { // 사용자에게 저장할지 물어봅니다.
                    if (m_PathName == nullptr) {
                        m_PathName = DoPathNameMessage(); // 사용자에게 파일명을 물어봅니다.

                        if (m_PathName == nullptr) { // 사용자가 여전히 파일명을 정해 주지 않았다면 저장할 수 없습니다.
                            std::cout << "Cannot Save." << std::endl;
                            return;
                        }
                    }
                    DoSave(m_PathName);
                }
                else {
                    // 저장하지 않고 Load 합니다.
                }
            }

            DoLoad(pathName); // 불러옵니다.
            m_PathName = pathName;
            SetDirty(false);
        }
         
        bool IsDirty() const {return m_Dirty;}       
        void SetDirty(bool dirty) {m_Dirty = dirty;}
    protected:
        // 기존 것을 저장할지 말지 물어봅니다.
        virtual bool DoSaveMessage() const = 0;

        // 파일명을 입력받습니다.
        virtual const char* DoPathNameMessage() const = 0;

        virtual void DoSave(const char * pathName) = 0;
        virtual void DoLoad(const char * pathName) = 0;
    };

    // ----
    // #2. App을 구체 구현한 개체입니다.
    // ----
    class MyApp : public App {
    public:
        explicit MyApp(const char* pathName) : App{pathName} {}
    protected:
       
        virtual bool DoSaveMessage() const override {
            std::cout << "MyApp::DoSaveMessage()" << std::endl;
            return true;    
        }

        // 파일명을 입력받습니다.
        virtual const char* DoPathNameMessage() const override {
            std::cout << "MyApp::DoPathNameMessage()" << std::endl;
            return "C:/MyOldFile.txt";
        }

        virtual void DoSave(const char * pathName) {
            std::cout << "MyApp::DoSave()" << std::endl;
        }
        virtual void DoLoad(const char * pathName) {
            std::cout << "MyApp::DoLoad()" << std::endl;
        }        
    };

    // ----
    // 테스트 코드
    // ----
    MyApp myApp(nullptr);
    myApp.SetDirty(true);

    myApp.Load("C:/MyFile.txt"); // #3. DoSaveMessage(), DoPathNameMessage()를 실행한 후 DoSave()하고 DoLoad()합니다.
}