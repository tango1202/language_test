#include "gtest/gtest.h" 

#include <set>

TEST(TestPattern, State) {

    // ---
    // #1. 편집기로 편집할 개체입니다.
    // ---
    class Rectangle {
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;
    public:
        Rectangle(int l, int t, int w, int h) : m_Left{l}, m_Top{t}, m_Width{w}, m_Height{h} {}

        int GetLeft() const {return m_Left;}
        int GetTop() const {return m_Top;}
        int GetWidth() const {return m_Width;}
        int GetHeight() const {return m_Height;}

        void Move(int deltaX, int deltaY) {
            m_Left += deltaX;
            m_Top += deltaY;
        }
        void Resize(int deltaWidth, int deltaHeight) {
            m_Width += deltaWidth;
            m_Height += deltaHeight;
        }
    };

    // ---
    // #2. 주어진 좌표 위치를 나타냅니다. 각 위치에 따라 선택인지, 이동인지, 크기 조정 인지 상태를 결정합니다.
    // ---
    enum class HitTestStyle {None, Inner, Border};
    
    // ---
    // #3. Rectangle을 관리하며, HitTest()를 이용하여 x, y위치에 따른 HitTestStyle을 구합니다.
    // ---
    class Doc {
        std::vector<Rectangle> m_Rectangles;
    public:
        Doc() = default;
        ~Doc() = default;
    private:
        Doc(const Doc&) = delete; 
        Doc(Doc&&) = delete; 
        Doc& operator =(const Doc&) = delete; 
        Doc& operator =(Doc&&) = delete;
    public:
        void Add(const Rectangle& rectangle) {
            m_Rectangles.push_back(rectangle);
        }

        size_t GetCount() const {return m_Rectangles.size();}

        Rectangle& GetAt(size_t index) {
            assert(index < m_Rectangles.size());

            return m_Rectangles[index];
        }
        // #3. 선택한 항목에 rectangle이 있으면 index와 HitTestStyle을 리턴합니다.
        std::pair<int, HitTestStyle> HitTest(int x, int y) const {

            int i{0};
            for (const auto& rectangle : m_Rectangles) {
                int l{rectangle.GetLeft()};
                int t{rectangle.GetTop()};
                int r{rectangle.GetLeft() + rectangle.GetWidth()};
                int b{rectangle.GetTop() + rectangle.GetHeight()};

                // 사각형 테두리에 있는 경우
                if (l <= x && x < r && y == t || 
                    l <= x && x < r && y == b ||
                    x == l && t <= y && y < b || 
                    x == r && t <= y && y < b) {
                    return std::make_pair(i, HitTestStyle::Border);
                } 
                // 사각형 내부에 있는 경우
                else if (l < x && x < r && t < y && y < b) {
                    return std::make_pair(i, HitTestStyle::Inner);
                }
                ++i;
            }
            return std::make_pair(-1, HitTestStyle::None);
        }        
    };
    // ---
    // #4. State의 추상 클래스입니다. 상태에 따라 LButtonDown(), MouseMove(), LButtonUp()시 다른 동작을 합니다.
    // ---
    class MouseEditor {
    protected:
        MouseEditor() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~MouseEditor() = default; // 다형 소멸 하도록 public virtual
    private:
        MouseEditor(const MouseEditor&) = delete;
        MouseEditor(MouseEditor&&) = delete;
        MouseEditor& operator =(const MouseEditor&) = delete;
        MouseEditor& operator =(MouseEditor&&) = delete;   
    public:   
        virtual void LButtonDown(int x, int y) = 0; // #4
        virtual void MouseMove(int x, int y) = 0; // #4
        virtual void LButtonUp(int x, int y) = 0; // #4  
    };

    // ---
    // #5. 선택 상태일 경우 MouseEditor 입니다. 마우스 위치의 Rectangle을 선택합니다.
    // ---
    class SelectEditor : public MouseEditor {
        const Doc& m_Doc;
        std::set<int> m_SelectedIndices; // 중복된 인덱스는 추가되지 않습니다.
    public:
        SelectEditor(const Doc& doc) : m_Doc{doc} {}
     
        virtual void LButtonDown(int x, int y) override {
            Insert(x, y);
        }  
        virtual void MouseMove(int x, int y) override {
            Insert(x, y);
        }
        virtual void LButtonUp(int x, int y) override {
            Insert(x, y);
        } 
    private:
        void Insert(int x, int y) {
            std::pair<int, HitTestStyle> result{m_Doc.HitTest(x, y)};
            
            if (result.first != -1) {
                m_SelectedIndices.insert(result.first);
            }
        }          
    };
    // ---
    // #5. 이동 상태일 경우 MouseEditor 입니다. 마우스 위치의 Rectangle을 이동합니다.
    // ---    
    class MoveEditor : public MouseEditor {
        Rectangle& m_Rectangle;
        int m_StartX;
        int m_StartY;
    public: 
        MoveEditor(Rectangle& rectangle) : m_Rectangle{rectangle} {}
        
        virtual void LButtonDown(int x, int y) override {
            m_StartX = x;
            m_StartY = y;
        }  
        virtual void MouseMove(int x, int y) override {}
        virtual void LButtonUp(int x, int y) override {
            int deltaX{x - m_StartX};
            int deltaY{y - m_StartY};
            
            m_Rectangle.Move(deltaX, deltaY);
        }        
    };
    // ---
    // #5. 크기 조정 상태일 경우 MouseEditor 입니다. 마우스 위치의 Rectangle을 크기 조정합니다.
    // ---      
    class ResizeEditor : public MouseEditor {
        Rectangle& m_Rectangle;
        int m_StartX;
        int m_StartY;
    public: 
        ResizeEditor(Rectangle& rectangle) : m_Rectangle{rectangle} {}
        
        virtual void LButtonDown(int x, int y) override {
            m_StartX = x;
            m_StartY = y;
        }  
        virtual void MouseMove(int x, int y) override {}
        virtual void LButtonUp(int x, int y) override {
            int deltaX{x - m_StartX};
            int deltaY{y - m_StartY};
            
            m_Rectangle.Resize(deltaX, deltaY);
        }        
    };
    // ----
    // #6. LButtonDown() 시 마우스 위치에 따라 다른 상태의 MouseEditor를 사용합니다. 상태 전환은 CreateMouseEditor()에서 처리됩니다.
    // ----
    class EditContext {
        Doc& m_Doc;
        std::unique_ptr<MouseEditor> m_MouseEditor;
    public:
        EditContext(Doc& doc) : m_Doc{doc} {}
        ~EditContext() = default;
    private:
        EditContext(const EditContext&) = delete; 
        EditContext(EditContext&&) = delete; 
        EditContext& operator =(const EditContext&) = delete; 
        EditContext& operator =(EditContext&&) = delete;        
    public:
        void LButtonDown(int x, int y) {
            std::pair<int, HitTestStyle> result{m_Doc.HitTest(x, y)};
            m_MouseEditor = CreateMouseEditor(result.first, result.second); // #6. 상태 전환합니다.

            m_MouseEditor->LButtonDown(x, y);

        }
        void MouseMove(int x, int y) {
            if (!m_MouseEditor) {
                return;
            }

            m_MouseEditor->MouseMove(x, y);
        }
        void LButtonUp(int x, int y) {

            if (!m_MouseEditor) {
                return;
            }

            m_MouseEditor->LButtonUp(x, y);            
            m_MouseEditor.reset(nullptr);
        }
    private:
        // 마우스 위치에 따라 다른 상태의 MouseEditor를 생성합니다.
        std::unique_ptr<MouseEditor> CreateMouseEditor(int selectedIndex, HitTestStyle style) {
            switch (style) {
            case HitTestStyle::None:
                return std::unique_ptr<MouseEditor>{new SelectEditor{m_Doc}};
            case HitTestStyle::Inner:
                return std::unique_ptr<MouseEditor>{new MoveEditor{m_Doc.GetAt(selectedIndex)}};
            case HitTestStyle::Border:
                return std::unique_ptr<MouseEditor>{new ResizeEditor{m_Doc.GetAt(selectedIndex)}};
            }
            return std::unique_ptr<MouseEditor>{new SelectEditor{m_Doc}}; // 알 수 없는 HitTest이면 그냥 SelectEditor를 사용합니다.
        }
    }; 

    // ----
    // 테스트 코드
    // ----
    Doc doc;
    doc.Add(Rectangle{0, 0, 10, 10});

    EditContext editContext{doc};

    // #7. 내부 영역을 드래그하여 Rectangle을 크기 조정 합니다.
    editContext.LButtonDown(5, 10); // 하단 테두리 영역입니다.
    editContext.LButtonUp(7, 15); // 2, 5 만큼 크기를 조정합니다. 

    EXPECT_TRUE(doc.GetAt(0).GetWidth() == 10 + (7 - 5));
    EXPECT_TRUE(doc.GetAt(0).GetHeight() == 10 + (15 - 10));

    // #7. 테두리 영역을 드래그하여 Rectangle을 이동시킵니다.
    editContext.LButtonDown(1, 1); // 사각형 내부입니다.
    editContext.LButtonUp(6, 3); // 5, 2 만큼 이동합니다.

    EXPECT_TRUE(doc.GetAt(0).GetLeft() == 0 + (6 - 1));
    EXPECT_TRUE(doc.GetAt(0).GetTop() == 0 + (3 - 1));
}