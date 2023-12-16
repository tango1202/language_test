#include "gtest/gtest.h" 


namespace Diagonostics_1 {
    int f(int a, int b, int* c) {
        int result = a + b + *c;

        return result;
    }
}
namespace Diagonostics_2 {
    int f(int a, int b, int* c) {

        // 사전 가정
        assert(0 <= a && a < 10);
        assert(10 <= b && b < 20);
        assert(c != NULL);
        assert(20 <= *c && *c < 30);

        if (!(0 <= a && a < 10)) {
            throw std::range_error("Fail : a");
        }
        if (!(10 <= b && b < 20)) {
            throw std::range_error("Fail : b");
        }
        if (!(c != NULL)) {
            throw std::invalid_argument("Fail : c != NULL");
        }  
        if (!(20 <= *c && *c < 30)) {
            throw std::range_error("Fail : c");
        }    

        int result = a + b + *c;

        // 사후 가정
        assert(30 <= result && result <= 9 + 19 + 29);

        return result;
    }
}


TEST(TestLegacyCpp, Diagonostics) {

    {
        // int a = 10;
        // assert(a == 10); // 참이므로 표시 안함
        // assert(a == 11); // 거짓이므로 표시함

        // // 문자열 포인터 값은 0이 아닌 임의의 수치값인 참이므로,
        // assert(a == 10 && "Test Message"); // 참 && 참 은 참이므로 표시 안함
        // assert(a == 11 && "Test Message"); // 거짓 && 참은 거짓이므로 표시함
    }
}
