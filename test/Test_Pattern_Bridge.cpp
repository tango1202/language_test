#include "gtest/gtest.h" 


namespace {

    class RenderImpl; // #2
    
    // ----
    // #1. 추상부 입니다. m_Impl을 이용합니다.
    // ----
    class Render {
        std::unique_ptr<RenderImpl> m_Impl;
    public:
        explicit Render(std::unique_ptr<RenderImpl> impl) : m_Impl{std::move(impl)} {
            assert(m_Impl);
        }
    private:
        Render(const Render&) = delete; 
        Render(Render&&) = delete; 
        Render& operator =(const Render&) = delete; 
        Render& operator =(Render&&) = delete;   
    public:
        // #5. 구현부를 변경합니다.
        void SetImpl(std::unique_ptr<RenderImpl> impl) { 
            assert(impl);
            m_Impl = std::move(impl);
        }

        // #2. RenderImpl에 컴파일 종속성이 있어 선언과 정의를 분리했습니다. 
        void DrawLine(int x1, int y1, int x2, int y2);
        void DrawImage(int l, int t, const char* filename);
    };

    // ----
    // Client 입니다. 추상부인 Render만 사용하며, RenderImpl에 독립적입니다.
    // ----
    class Rectangle {
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;
    public:
        Rectangle(int l, int t, int w, int h) : m_Left{l}, m_Top{t}, m_Width{w}, m_Height{h} {}

            // #6. 어떤 렌더링 엔진을 사용하는지 고민하지 않고 본연의 드로잉 코드만 작성합니다.
        void Draw(Render& render) const { 
            int right{m_Left + m_Width};
            int bottom{m_Top + m_Height};

            render.DrawLine(m_Left, m_Top, right, m_Top);
            render.DrawLine(right, m_Top, right, bottom);
            render.DrawLine(right, bottom, m_Left, bottom);
            render.DrawLine(m_Left, bottom, m_Left, m_Top);
        }
    };
    // ----
    // #3. 구현부의 추상 클래스입니다.
    // ----
    class RenderImpl {
    protected:
        RenderImpl() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected    
    public:
        virtual ~RenderImpl() = default; // 다형 소멸 하도록 public virtual
    private:
        RenderImpl(const RenderImpl&) = delete; 
        RenderImpl(RenderImpl&&) = delete; 
        RenderImpl& operator =(const RenderImpl&) = delete; 
        RenderImpl& operator =(RenderImpl&&) = delete;   
    public:
        virtual void DrawLineImpl(int x1, int y1, int x2, int y2) = 0;
        virtual void DrawImageImpl(int l, int t, const char* filename) = 0;
    };

    // ----
    // #2. Render 정의부입니다. RenderImpl에 컴파일 종속성이 있어 선언과 정의를 분리했습니다. 
    // ----
    void Render::DrawLine(int x1, int y1, int x2, int y2) {
        assert(m_Impl);
        m_Impl->DrawLineImpl(x1, y1, x2, y2);
    }
    void Render::DrawImage(int l, int t, const char* filename) {
        assert(m_Impl);
        m_Impl->DrawImageImpl(l, t, filename);
    }

    // ----
    // #4. RenderImpl의 실제 구현부 입니다.
    // ----
    class GDIRenderImpl : public RenderImpl {
    public:
        virtual void DrawLineImpl(int x1, int y1, int x2, int y2) override {
            std::cout << "GDIRenderImpl::DrawLineImpl()" << std::endl;
        }
        virtual void DrawImageImpl(int l, int t, const char* filename) override {
            std::cout << "GDIRenderImpl::DrawImageImpl()" << std::endl;
        }
    };
    class WPFRenderImpl : public RenderImpl {
    public:
        virtual void DrawLineImpl(int x1, int y1, int x2, int y2) override {
            std::cout << "WPFRenderImpl::DrawLineImpl()" << std::endl;
        }
        virtual void DrawImageImpl(int l, int t, const char* filename) override {
            std::cout << "WPFRenderImpl::DrawImageImpl()" << std::endl;
        }
    };   
}

TEST(TestPattern, Bridge) {
    // ----
    // 테스트 코드
    // ----        
    Render render{std::unique_ptr<RenderImpl>{new GDIRenderImpl{}}};
    Rectangle rect{0, 0, 10, 20};

    // GDIRenderImpl로 실행합니다.
    rect.Draw(render);

    // #5. 런타임에 구현부를 WPFRenderImpl로 변경하여 실행할 수 있습니다.
    render.SetImpl(std::unique_ptr<RenderImpl>{new WPFRenderImpl{}}); 
    rect.Draw(render);
}