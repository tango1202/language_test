import MyModule_20; // MyModule_20.Sub1, MyModule_20.Sub2를 사용할 수 있습니다.
import MyModule_20.Sub3; // (△) 비권장. export되었기에 main에서도 사용할 수 있습니다.

int main() {
    Sub1();
    Sub2(); 
    Sub3(); // (△) 비권장. export되었기에 main에서도 사용할 수 있습니다.
    return 0;
}