// Test_B.cpp에서
// #include <iostream>

// extern int g_A;
// int g_B = g_A; // (△) 비권장. 컴파일 단계에선 일단 0으로 초기화 하고, 나중에 링크 단계에서 g_A의 값으로 초기화 합니다.
                  // g_A가 초기화 되었다는 보장이 없기에 링크 순서에 따라 0 또는 10이 됩니다.

// int main() {
//     std::cout << "g_A : " << g_A << std::endl;
//     std::cout << "g_B : " << g_B << std::endl; // (△) 비권장. 0이 나올 수도 있고, 10이 나올 수도 있습니다.

//     return 0;
// }

// Test_B.cpp에서
#include <iostream>

#if 202002L <= __cplusplus // C++20~ 
extern constinit int g_A_20;
int g_B_20 = g_A_20; // 컴파일 타임에 초기화 됩니다.

int main() {
    std::cout << "g_A_20 : " << g_A_20 << std::endl;
    std::cout << "g_B_20 : " << g_B_20 << std::endl; // 항상 10이 나옵니다.

    return 0;
}
#endif