#include "gtest/gtest.h" 

namespace {
    // App 에서 문서 저장시 호출합니다.
    class ISaveListener {
    protected:
        ~ISaveListener() {}  // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

    public:
        virtual void SaveDoc() = 0;
    };
    class App {
    private:
        bool m_Dirty; // 내용이 수정되면 true입니다.
        std::wstring m_PathName;
        ISaveListener* m_SaveListener;
    public:
        App(bool dirty, const std::wstring& pathName, ISaveListener* saveListener) : 
            m_Dirty(dirty),
            m_PathName(pathName),
            m_SaveListener(saveListener) {}
    public:
        bool IsDirty() const {return m_Dirty;}
        void SetDirty(bool val) {m_Dirty = val;}
        const std::wstring& GetPathName() const {return m_PathName;}

        void Save() {

            // 파일명이 없다면 다른 이름으로 저장
            if (GetPathName().empty()) {
                SaveAs();
                return;;
            }

            // 저장할 필요가 있는지 확인
            if (!IsDirty()) {return;}

            // 문서 내용 저장
            if (m_SaveListener != nullptr) {
                m_SaveListener->SaveDoc();
            }

            // 저장되었다고 플래그 설정
            SetDirty(false);
        }

        // 다른 이름으로 저장합니다.
        void SaveAs() {}
    };

    // App 을 상속받아 만들었고, ISaveListener를 App에 전달합니다.
    class MyApp : 
        public App,
        public ISaveListener {
    public:
        MyApp(bool dirty, const std::wstring& pathName) :
            App(dirty, pathName, this) {} // 부모인 App에 this를 전달하여 ISaveListener를 전달합니다.
    public:
        // MyApp의 문서 내용을 저장합니다.
        virtual void SaveDoc() override {
            std::cout <<"MyApp::SaveDoc()"<<std::endl;  
        }
    };
}

TEST(TestPrinciple, Hollywood) {
    MyApp myApp(true, L"test.txt");
    myApp.Save(); // MyApp::SaveDoc()       

    // YourApp yourApp(true, L"test.txt");
    // yourApp.Save(); // YourApp::SaveDoc() 출력  
}