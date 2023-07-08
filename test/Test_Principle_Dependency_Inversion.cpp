#include "gtest/gtest.h" 

namespace {
    class IWriter {
    protected:
        ~IWriter() {} // 상속받지만, 다형적으로 사용하지 않아 non-virtual 입니다.
    public:
        virtual void WriteIntVal(const std::wstring& attrName, int val) const = 0;
    };

    class XmlWriter : 
        public IWriter {
    public:
        virtual void WriteIntVal(const std::wstring& attrName, int val) const override { /* xml로 저장합니다 */ }
    };
    class JsonWriter : 
        public IWriter {
    public:
        virtual void WriteIntVal(const std::wstring& attrName, int val) const override { /* json로 저장합니다 */ }
    };

    class Shape {
    private:
        int m_X;
        int m_Y;
        const IWriter* m_Writer; // 인터페이스에 의존합니다.
    public:
        Shape() {} // 이제 writer를 생성자에서 설정하지 않습니다.
    public:
        // 런타임에 writer를 변경합니다.
        void SetWriter(const IWriter* writer) {
            assert(writer != nullptr);
            m_Writer = writer;  
        }
        void Save() const {
            if (m_Writer != nullptr) {
                m_Writer->WriteIntVal(L"x", m_X);
                m_Writer->WriteIntVal(L"y", m_Y);
            }
        }
    };
}

TEST(TestPrinciple, DependencyInversion) {
    XmlWriter xmlWriter;
    JsonWriter jsonWriter;

    Shape shape;
    shape.SetWriter(&xmlWriter);
    shape.Save(); // xml 으로 저장
    
    shape.SetWriter(&jsonWriter);
    shape.Save(); // json 으로 저장
}