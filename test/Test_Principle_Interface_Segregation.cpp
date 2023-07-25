#include "gtest/gtest.h" 

namespace {
    // 단위 특성들을 분리하여 날씬해졌습니다
    class IAnimal {
    public:
        virtual ~IAnimal() {} // 다형적 소멸

    public:
        virtual void Born() = 0;
        virtual void Dead() = 0;
    };

    // 인터페이스들이 분리되어 간소화 되었습니다.
    class IEatable {
    protected:
        ~IEatable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

    public:
        virtual void Eat() = 0;
    };
    class IWalkable {
    protected:
        ~IWalkable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

    public:
        virtual void Walk() = 0;
    };
    class ISwimmable {
    protected:
        ~ISwimmable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

    public:
        virtual void Swim() = 0;
    };
    class IFlyable {
    protected:
        ~IFlyable() {} // 상속할 수 있지만, 다형적으로 사용하지 않아 non-virtual 입니다.

    public:
        virtual void Fly() = 0;
    };

    // 분리된 인터페이스들중 필요한 것만으로 구성합니다.
    class Dog :
        public IAnimal,
        public IEatable,
        public IWalkable,
        public ISwimmable {
    public:        
        virtual void Born() override { /*구현해야 함*/ }
        virtual void Dead() override { /*구현해야 함*/ }
        virtual void Eat() override { /*구현해야 함*/ }
        virtual void Walk() override { /*구현해야 함*/ }
        virtual void Swim() override { /*구현해야 함*/ }
    };
}

TEST(TestPrinciple, InerfaceSegregation) {
    Dog dog;
    dog.Born();
    dog.Dead();
    dog.Eat();
    dog.Walk();
    dog.Swim();
}