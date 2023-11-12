// Test_A.cpp에서
// int f() {return 10;}
// int g_A = f(); // 전역 변수. 런타임에 f() 함수를 이용해서 초기화 합니다.

constexpr int f_11() {return 10;} // 컴파일 타임 상수 입니다.
constinit int g_A_20 = f_11(); // 컴파일 타임에 초기화 됩니다.