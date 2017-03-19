// Aho-Corasik.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"


int main()
{
    Bor bor;
    bor.input();
    std::string s;
    std::fstream cin;
    cin.open("C:\\Games\\input.txt");
    cin >> s;
    cin >> s;
    bor.find(s);
    std::cin >> s;
    return 0;
}
