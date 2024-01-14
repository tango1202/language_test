#include "gtest/gtest.h" 

#include <typeinfo.h>

namespace {

    // ----
    // #1. Flyweight입니다. Draw() 인터페이스를 제공합니다.
    // ----
    class Brush {
    protected:
        Brush() = default; // 다형 소멸을 제공하는 추상 클래스. 상속해서만 사용하도록 protected
    public:
        virtual ~Brush() = default; // 다형 소멸 하도록 public virtual   
    private:
        Brush(const Brush&) = delete;
        Brush(Brush&&) = delete;
        Brush& operator =(const Brush&) = delete;
        Brush& operator =(Brush&&) = delete;          
    public:
        virtual void Draw(int x, int y, int w, int h) const = 0;    

        // #5. 연관 컨테이너에서 사용하기 위해 < 구현이 필요합니다.
        virtual bool operator <(const Brush& other) const = 0;
    };
    // ----
    // #2. Flyweight를 구체화한 클래스입니다.
    // ----
    class SolidBrush : public Brush {
        const char* m_Color;
    public:
        explicit SolidBrush(const char* color) : m_Color{color} {
            assert(m_Color);    
        } 
        
        virtual void Draw(int x, int y, int w, int h) const override {
            std::cout << "SolidBrush : " << m_Color << std::endl;
        }  

        // #5
        virtual bool operator <(const Brush& other) const override {
            const std::type_info& thisType{typeid(*this)};
            const std::type_info& otherType{typeid(other)};

            if (thisType.before(otherType)) return true;
            if (otherType.before(thisType)) return false;

            // 타입이 같은 경우 < 검사
            return *this < dynamic_cast<const SolidBrush&>(other);
        }      
    private:
        // #5
        bool operator <(const SolidBrush& other) const {
            return m_Color < other.m_Color;
        }
    };
    class GradientBrush : public Brush {
        const char* m_StartColor;
        const char* m_LastColor;
    public:
        GradientBrush(const char* startColor, const char * lastColor) :
            m_StartColor{startColor}, 
            m_LastColor{lastColor} {
            assert(m_StartColor && m_LastColor);
        }

        virtual void Draw(int x, int y, int w, int h) const override {
            std::cout << "GradientBrush : " << m_StartColor << ", " << m_LastColor << std::endl;
        }

        // #5
        virtual bool operator <(const Brush& other) const override {
            const std::type_info& thisType{typeid(*this)};
            const std::type_info& otherType{typeid(other)};

            if (thisType.before(otherType)) return true;
            if (otherType.before(thisType)) return false;

            // 타입이 같은 경우 < 검사
            return *this < dynamic_cast<const GradientBrush&>(other);
        }  
    private:
        // #5
        bool operator <(const GradientBrush& other) const {
            if (m_StartColor < other.m_StartColor) return true;
            if (other.m_StartColor < m_StartColor) return false;

            return m_LastColor < other.m_LastColor;
        }           
    };
    // ----
    // #3. FlyweightFactory 입니다.
    // ----
    class BrushFactory {
        private:
        using BrushComparer = bool (*)(std::shared_ptr<Brush>, std::shared_ptr<Brush>);
        std::set<std::shared_ptr<Brush>, BrushComparer> m_Pool; // #3
    public:
        BrushFactory() : m_Pool{Compare} {}
        ~BrushFactory() = default; 
    private:
        BrushFactory(const BrushFactory&) = delete;
        BrushFactory(BrushFactory&&) = delete;
        BrushFactory& operator =(const BrushFactory&) = delete;
        BrushFactory& operator =(BrushFactory&&) = delete;          
    public:
        // #4. 주어진 브러쉬가 있으면 기존것을 사용하고, 그렇지 않으면 새롭게 추가합니다.
        std::shared_ptr<Brush> GetSolidBrush(const char* color) {
            assert(color);
            
            std::shared_ptr<Brush> item{new SolidBrush{color}};

            auto result = m_Pool.insert(item);
            
            return *result.first; // 삽입되지 않았다면 기존 것을 리턴하고, 삽입되었다면 삽입한 것을 리턴합니다.
        }
        // #4
        std::shared_ptr<Brush> GetGradientBrush(const char* startColor, const char* lastColor) {
            assert(startColor && lastColor);

            std::shared_ptr<Brush> item{new GradientBrush{startColor, lastColor}};

            auto result = m_Pool.insert(item);
            
            return *result.first; // 삽입되지 않았다면 기존 것을 리턴하고, 삽입되었다면 삽입한 것을 리턴합니다.        
        }

        size_t GetCount() const {
            return m_Pool.size();
        }
    private:
        // Brush를 대소 비교합니다.
        static bool Compare(std::shared_ptr<Brush> left, std::shared_ptr<Brush> right) {
            assert(left && right);
            return *left < *right;
        }
    };

    class Rectangle {
        int m_Left;
        int m_Top;
        int m_Width;
        int m_Height;

        std::shared_ptr<Brush> m_Brush; // #6. BrushFactory에서 관리되는 Brush 이거나 사용자가 임의로 생성한 Brush 입니다.
    public:
        Rectangle(int l, int t, int w, int h) : m_Left{l}, m_Top{t}, m_Width{w}, m_Height{h} {}

        void SetBrush(std::shared_ptr<Brush> brush) {
            m_Brush = brush;
        }    
        void Draw() const { 
            if (!m_Brush) { // 브러쉬가 없다면 그리지 않습니다.
                return;    
            }

            m_Brush->Draw(m_Left, m_Top, m_Width, m_Height);
        }
    };    
}


TEST(TestPattern, Flyweight) {
    // ----
    // 테스트 코드
    // ----
    BrushFactory factory;

    Rectangle rectangle{1, 1, 10, 20};

    rectangle.SetBrush(factory.GetSolidBrush("red"));
    rectangle.Draw();

    rectangle.SetBrush(factory.GetSolidBrush("black"));
    rectangle.Draw();

    rectangle.SetBrush(factory.GetSolidBrush("red")); // #7. 이전에 "red"를 사용해서, 기존 것을 리턴합니다.
    rectangle.Draw();

    rectangle.SetBrush(factory.GetGradientBrush("red", "black"));
    rectangle.Draw();

    EXPECT_TRUE(factory.GetCount() == 3); // #7. 4개 Brush중 `red`는 기존 것을 사용하므로, 3개만 추가되었습니다.       

    rectangle.SetBrush(std::shared_ptr<Brush>{new SolidBrush{"red"}}); // #8. 공유되지 않은 Brush를 사용하려면 BrushFactory를 이용하지 않고 직접 생성합니다. 
}