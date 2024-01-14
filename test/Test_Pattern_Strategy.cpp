#include "gtest/gtest.h" 

TEST(TestPattern, Strategy) {

    // ----
    // #1. WriteRectangle() 함수로 Rectangle 정보를 저장합니다.
    // ----
    class IWriter {
    protected:
        IWriter() = default; // 인터페이스여서 상속해서만 사용하도록 protected
        ~IWriter() = default; // 인터페이스여서 다형 소멸을 하지 않으므로 protected non-virtual
    private:
        IWriter(const IWriter&) = delete;
        IWriter(IWriter&&) = delete;
        IWriter& operator =(const IWriter&) = delete;
        IWriter& operator =(IWriter&&) = delete;
    public:
        virtual int WriteRectangle(int left, int top, int width, int height) const = 0;
    };

    // ----
    // #2. Xml 또는 Json으로 저장합니다.
    // ----
    class XmlWriter : public IWriter {
    public:
        XmlWriter() = default;

        virtual int WriteRectangle(int left, int top, int width, int height) const override {
            return 0;
        }
    };
    class JsonWriter : public IWriter {
    public:
        JsonWriter() = default;

        virtual int WriteRectangle(int left, int top, int width, int height) const override {
            return 1;
        }
    };    

    // ---
    // #3. Save()시 설정된 IWriter에 따라 다른 전략으로 저장합니다.
    // ---
    class Rectangle {
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;

        const IWriter* m_Writer;
    public:
        Rectangle(int l, int t, int w, int h) : m_Left{l}, m_Top{t}, m_Width{w}, m_Height{h} {}

        int GetLeft() const {return m_Left;}
        int GetTop() const {return m_Top;}
        int GetWidth() const {return m_Width;}
        int GetHeight() const {return m_Height;}

        void SetWriter(const IWriter* writer) {m_Writer = writer;} // #3
        int Save() {
            if (m_Writer) {
                return m_Writer->WriteRectangle(m_Left, m_Top, m_Width, m_Height); // #3
            }
            return -1;
        }
    };

    // ----
    // 테스트 코드
    // ----
    Rectangle rectangle{0, 0, 10, 20};

    XmlWriter xmlWriter;
    rectangle.SetWriter(&xmlWriter);
    EXPECT_TRUE(rectangle.Save() == 0); // XmlWriter::WriteRectangle()이 실행됩니다.

    JsonWriter jsonWriter;
    rectangle.SetWriter(&jsonWriter); // #4. 런타임에 전략을 변경합니다.
    EXPECT_TRUE(rectangle.Save() == 1); // JsonWriter::WriteRectangle()이 실행됩니다.
}