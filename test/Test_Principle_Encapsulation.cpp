#include "gtest/gtest.h" 

namespace {
    class Degree {
    private:
        float m_Value;
    public:
        explicit Degree(float val = 0) :
            m_Value(Constrain(val)) {}
    public:
        const Degree& operator =(float val) {
            m_Value = Constrain(val); 
            return *this; 
        }
        void operator +=(const Degree& other) { m_Value = Constrain(m_Value + other.GetValue()); }
        void operator +=(float val) { m_Value = Constrain(m_Value + val); }
        void operator -=(const Degree& other) { m_Value = Constrain(m_Value - other.GetValue()); }
        void operator -=(float val) { m_Value = Constrain(m_Value - val); }
        const Degree operator -() const { return Degree(-m_Value); }

        float GetValue() const { return m_Value; }
    private:
        // 0~360 값으로 강제함
        static float Constrain(float val) {
            float min = 0.F;
            float max = 360.F;

            // 범위 바깥이라면 0~360 사이로 보정
            if (val < min || max <= val) {
                // 몫
                int quatient = static_cast<int>(val) / static_cast<int>(max);

                // 360의 배수값은 뺀 나머지값으로 조정
                val = val - max * quatient;

                // 음수라면 양수화 함
                if (val < min) {
                    val = max + val; // val은 음수이며 -360 보다 작은값임
                }
            }

            assert(!(val < min));
            assert(!(max < val));

            return val;
        }
    };

    // ==
    inline bool operator ==(const Degree& left, const Degree& right) {
        if (left.GetValue() != right.GetValue()) return false;
        return true;
    }
    inline bool operator ==(const Degree& left, float right) {
        return left == Degree(right);
    }
    inline bool operator ==(const float left,  const Degree& right) {
        return right == left;
    }
    // !=
    inline bool operator !=(const Degree& left, const Degree& right) {
        return !(left == right);
    }
    inline bool operator !=(const Degree& left, float right) {
        return left != Degree(right);
    }
    inline bool operator !=(const float left,  const Degree& right) {
        return right != left;
    }
    // <, <=
    inline bool operator <(const Degree& left, const Degree& right) {
        return left.GetValue() < right.GetValue();
    }
    inline bool operator <=(const Degree& left, const Degree& right) {
        return left.GetValue() <= right.GetValue();
    }
    // +
    inline Degree operator +(const Degree& left, const Degree& right) {
        Degree result(left);
        result += right;
        return result;
    }
    inline Degree operator +(const Degree& left, float right) {
        Degree result(left);
        result += right;
        return result;
    }
    // -
    inline Degree operator -(const Degree& left, const Degree& right) {
        Degree result(left);
        result -= right;
        return result;
    }
    inline Degree operator -(const Degree& left, float right) {
        Degree result(left);
        result -= right;
        return result;
    }
    class Shape {
    private:
        Degree m_Angle; // 디폴트로 0도
    public:  
        const Degree& GetAngle() const { return m_Angle; }

        // delta 만큼 더 회전시킵니다.
        void Rotate(const Degree& delta) {
            // 회전시킵니다. Degree 클래스이니 알아서 보정합니다. 
            // assert 검사할 필요도 없습니다.
            m_Angle += delta;
        }
    };    
}

TEST(TestPrinciple, Encapsulation) {
    Degree _360(360.F);
    EXPECT_FLOAT_EQ(_360.GetValue(), 0.F);
        
    Degree _370(370.F);
    EXPECT_FLOAT_EQ(_370.GetValue(), 10.F);

    Degree _Minus10(-10);  
    EXPECT_FLOAT_EQ(_Minus10.GetValue(), 350.F); 

    // == 만 테스트
    EXPECT_TRUE(Degree(370.F) == Degree(10.F));
    EXPECT_TRUE(Degree(370.F) == 10.F);
    EXPECT_TRUE(10.F == Degree(370.F));  

    // Shape 테스트
    Shape shape;
    
    shape.Rotate(Degree(350.F));
    EXPECT_FLOAT_EQ(shape.GetAngle().GetValue(), 350.F);

    shape.Rotate(Degree(20.F));
    EXPECT_FLOAT_EQ(shape.GetAngle().GetValue(), 10.F); // 350 + 20 = 370도, 즉 10도

    shape.Rotate(Degree(-20.F));
    EXPECT_FLOAT_EQ(shape.GetAngle().GetValue(), 350.F); // 10 - 20 = -10도, 즉 350도
}