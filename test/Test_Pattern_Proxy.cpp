#include "gtest/gtest.h" 

namespace {

    // ----
    // #1. Subject입니다. Draw()와 크기의 Get/Set 인터페이스를 제공합니다.
    // ----
    class Shape {
    protected:
        Shape() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~Shape() = default; // 다형 소멸 하도록 public virtual
    private:
        Shape(const Shape&) = delete;
        Shape(Shape&&) = delete;
        Shape& operator =(const Shape&) = delete;
        Shape& operator =(Shape&&) = delete;          
    public:
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;

        virtual void SetWidth(int val) = 0;
        virtual void SetHeight(int val) = 0;    

        virtual void Draw() const = 0;   
    };
    // ----
    // #2. RealSubject입니다. 생성시 실제 이미지를 Load() 합니다.
    // ----
    class Image : public Shape {
        int m_Width;
        int m_Height;
    public:
        explicit Image(int w, int h, const char* filename) : m_Width{w}, m_Height{h} {
            Load(filename); // #2
        }
        virtual int GetWidth() const override {return m_Width;}
        virtual int GetHeight() const override {return m_Height;}

        virtual void SetWidth(int val) override {m_Width = val;}
        virtual void SetHeight(int val) override {m_Height = val;}    

        virtual void Draw() const {
            std::cout << "Image::Draw()" << std::endl;
        }
    private:
        void Load(const char* filename) { // #2
            std::cout << "Image::Load()" << std::endl;
        }
    };
    // ----
    // #3. Proxy입니다. Draw() 시 실제 Image를 생성하며, 그전에는 멤버 변수의 값을 이용합니다.
    // ----
    class ImageProxy : public Shape {
        int m_Width; // #3. m_Image가 만들어지지 않으면 사용합니다.
        int m_Height;   
        const char* m_Filename;     
        mutable std::unique_ptr<Image> m_Image; 
    public:
        ImageProxy(int w, int h, const char* filename) :
            m_Width{w}, 
            m_Height{h}, 
            m_Filename{filename} {
            assert(filename);
        }

        virtual int GetWidth() const override {
            if (!m_Image) {
                return m_Width; // #3
            }
            return m_Image->GetWidth();
        }
        virtual int GetHeight() const override {
            if (!m_Image) {
                return m_Height; // #3  
            }
            return m_Image->GetHeight();
        }
        virtual void SetWidth(int val) override {
            if (!m_Image) {
                m_Width = val; // #3  
                return; 
            }
            m_Image->SetWidth(val);
        }
        virtual void SetHeight(int val) override {
            if (!m_Image) {
                m_Height = val; // #3 
                return; 
            }
            m_Image->SetHeight(val);
        } 

        // #3. m_Image가 없다면 생성합니다.      
        void Draw() const {
            if(!m_Image) {
                m_Image = std::unique_ptr<Image>{new Image{m_Width, m_Height, m_Filename}};
            }

            m_Image->Draw();
        }

        bool IsValid() const {
            return m_Image ? true : false;
        }
    };
}

TEST(TestPattern, Proxy) {

    // ----
    // 테스트 코드
    // ----
    ImageProxy image{10, 20, "MyImage.png"};

    image.GetWidth(); // #3. Image를 생성하기 전에 Proxy를 이용하여 크기를 구합니다.
    image.GetHeight();

    EXPECT_TRUE(image.IsValid() == false);

    image.Draw(); // 이미지를 로딩하여 그립니다.

    EXPECT_TRUE(image.IsValid() == true); // #4
}