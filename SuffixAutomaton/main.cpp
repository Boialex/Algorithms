#include "SuffixAutomaton.h"
#include <iostream>

int main() {
    std::string s;
    std::cin >> s;
    SuffixAutomaton t(s);
    std::cout << t.countSubstrings() << std::endl;
    return 0;
}