// Ukkonen.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "SuffixTreeZipped.h"


int _tmain(int argc, _TCHAR* argv[])
{
    std::string s1, s2;
    std::cin >> s1;
    std::cin >> s2; 
    SuffixTreeZipped t;
    std::cout << t.LCS(s1, s2) << std::endl;
    return 0;
}

