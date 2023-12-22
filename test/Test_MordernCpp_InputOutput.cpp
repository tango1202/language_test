#include "gtest/gtest.h" 

TEST(TestMordern, InputOutput) {

    {
        std::cout << 100 << std::endl; // 100 기본적으로 양수인 경우 +를 표시하지 않습니다.
        std::cout << std::showpos << 100 << std::endl; // +100 showpos를 사용하여 +를 표시합니다.
        std::cout << 100 << std::endl; // +100 기본 형태로 사용해도 +를 표시합니다.

        std::cout << std::resetiosflags(std::ios_base::showpos); // showpos 설정을 초기화합니다.
        std::cout << 100 << std::endl; // 100을 표시합니다.
    }
    // 서식 조정자
    {
        std::cout << true << std::endl; // 기본적으로 0, 1로 표시합니다.
        std::cout << std::boolalpha << true << std::endl; // true, flase로 표시합니다.
        std::cout << std::noboolalpha << true << std::endl; // 0, 1로 표시합니다.

        std::cout << std::showpoint << 3. << std::endl; // 3.00000 소수점을 표시합니다.
        std::cout << std::noshowpoint << 3. << std::endl; // 3 소수점을 표시하지 않습니다.
    
        std::cout << std::showpos << 100 << std::endl; // +100 으로 표시합니다.
        std::cout << std::noshowpos << 100 << std::endl; // 100 으로 표시합니다.

        std::cout << std::resetiosflags(std::ios_base::boolalpha | std::ios_base::showpoint | std::ios_base::showpos);
    }
    {
        std::cout << std::left << std::setfill('*') << std::setw(5) << -10 << std::endl; // -10**
        std::cout << std::right << std::setfill('*') << std::setw(5) << -10 << std::endl; // **-10
        std::cout << std::internal << std::setfill('*') << std::setw(5) << -10 << std::endl; // -**10 

        std::cout << std::resetiosflags(std::ios_base::adjustfield); 
        std::cout << std::setfill(' '); // fill은 서식 플래그가 없어서 초기값을 설정합니다. 
    }
  
    {
        // 0XAB를 표시합니다.
        std::cout 
            << std::hex // 16진수로 표시합니다.
            << std::showbase // `0`(*8진수*), `0x`(*16진수*)의 진법 접두어를 표시합니다.
            << std::uppercase // 16진수나 부동 소수점의 지수 표기(`e`)에서 사용하는 알파벳을 대문자로 표시합니다.
            << 0xab 
        << std::endl;   
        
        std::cout << std::resetiosflags(std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase);
          
    }

    {
        // [][][]3.142을 표시합니다.(앞쪽 공백 3개)
        std::cout
            << std::setw(8) // 최소 너비입니다.
            << std::setprecision(4) // 소수점을 포함한 소수점 이하 자리수입니다.
            << 3.141592
        << std::endl; 

        std::cout << std::setw(0) << std::setprecision(6); // setw, setprecision은 서식 플래그가 없어서 초기값을 설정합니다. 
    }     

}

