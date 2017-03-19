// Strings.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

std::vector<int> zFunction(std::string & s)
{
    std::vector<int> z;
    z.resize(s.length());
    int left = 0, right = 0;
    for (int i = 1; i < s.length(); ++i) {
        z[i] = std::max(0, std::min(right - i, z[i - left]));
        while (i + z[i] < s.length() && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] >= right) {
            left = i;
            right = i + z[i];
        }
    }
    return z;
}

std::vector<int> prefixFunction(std::string & s)
{
    std::vector<int> p;
    p.resize(s.length());
    p[0] = 0;
    for (int i = 1; i < s.length(); ++i) {
        int k = p[i - 1];
        while (k > 0 && s[i] != s[k]) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            ++k;
        }
        p[i] = k;
    }
    return p;
}

void ZKMP(std::string & pattern, std::string text)
{
    pattern += '#';
    std::vector<int> z;
    int currentZ = 0;;
    z.resize(pattern.length());
    int left = 0, right = 0;
    for (int i = 1; i < pattern.length(); ++i) {
        z[i] = std::max(0, std::min(right - i, z[i - left]));
        while (i + z[i] < pattern.length() && pattern[z[i]] == pattern[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] >= right) {
            left = i;
            right = i + z[i];
        }
    }
    left = right = 0;
    for (int i = 0; i < text.length(); ++i) {
        currentZ = std::max(0, std::min(right - i, z[i - left]));
        while (i + currentZ < text.length() && pattern[currentZ] == text[i + currentZ]) {
            ++currentZ;
        }
        if (i + currentZ >= right) {
            left = i;
            right = i + currentZ;
        }
        if (currentZ == pattern.length() - 1) {
            std::cout << i << ' ';
        }
    }
    std::cout << std::endl;
}

void KMP(std::string & pattern, std::string text)
{
    std::vector<int> p;
    pattern += '#';
    int currentValue = 0;
    p.resize(pattern.length());
    p[0] = 0;
    for (int i = 1; i < pattern.length(); ++i) {
        int k = p[i - 1];
        while (k > 0 && pattern[i] != pattern[k]) {
            k = p[k - 1];
        }
        if (pattern[i] == pattern[k]) {
            ++k;
        }
        p[i] = k;
    }
    for (int i = 0; i < text.length(); ++i) {
        int k = currentValue;
        while (k > 0 && text[i] != pattern[k]) {
            k = p[k - 1];
        }
        if (text[i] == pattern[k]) {
            ++k;
        }
        currentValue = k;
        if (currentValue == pattern.length() - 1) {
            std::cout << i + 2 - pattern.length() << ' ';
        }
    }
    std::cout << std::endl;
}

std::vector<int> prefFromZ(std::vector<int> & z)
{
    std::vector<int> p(z.size(), 0);
    for (int i = 1; i < z.size(); ++i) {
        for (int j = z[i] - 1; j >= 0 && p[i + j] == 0; --j) {
            p[i + j] = j + 1;
        }
    }
    return p;
}

std::string stringFromPrefix(std::vector<int> & p) {
    std::string s = "a";
    std::vector<bool> used(26, false);
    for (int i = 1; i < p.size(); ++i) {
        if (p[i] == 0) {
            //s += s[0] + 1;
            int k = p[i - 1];
            while (k > 0) {
                used[int(s[k]) - 97] = true;
                k = p[k - 1];
            }
            used[int(s[0]) - 97] = true;
            int minChar = 0;
            while (used[minChar]) {
                ++minChar;
            }
            s += char(minChar + 97);
            used.assign(26, false);
        }
        else {
            s += s[p[i] - 1];
        }
    }
    return s;
}

std::vector<int> zFromPref(std::vector<int> & p)
{
    return zFunction(stringFromPrefix(p));
}

std::string stringFromZ(std::vector<int> & z) 
{
    return stringFromPrefix(prefFromZ(z));
}

std::string longestPolindrom(std::string & s)
{
    std::vector<int> polindrom(s.length(), 0);
    int maxLeft = s.length();
    int len = 0;
    for (int j = 0; j < 2; ++j) {
        int left = 0, right = -1;
        for (int i = 0; i < s.length(); ++i) {
            int k;
            if (i <= right) {
                k = std::min(polindrom[left + right - i] - j, right - i) + 1;
            } else {
                k = 1;
            }
            while (i + k - 1 < s.length() && i + j >= k && s[i + k - 1] == s[i - k + j]) {
                ++k;
            }
            polindrom[i] = --k;
            if (i + k > right) {
                left = i - k + j;
                right = i + k - 1;
            }
            if (polindrom[i] * 2 - j > len) {
                len = polindrom[i] * 2 - j;
                maxLeft = i - polindrom[i] + j;
            }
        }
    }
    return s.substr(maxLeft, len);
}

int main()
{
   // std::string pattern, text;
   // std::cin >> pattern;
   // std::cin >> text;
   // KMP(pattern, text);
   // ZKMP(pattern, text);
   // std::vector<int> code;
   // for (int i = 0; i < 5; ++i) {
   //     int n;
   //     std::cin >> n;
   //     code.push_back(n);
   // }
   // std::cout << stringFromPrefix(code);
    /*std::string s;
    std::cin >> s;
    std::cout << longestPolindrom(s) << std::endl;*/
    std::fstream cin;
    cin.open("C:\\Games\\input.txt");
    int x;
    std::vector<int> input;
    while (!cin.eof()) {
        cin >> x;
        input.push_back(x);
    }
    input.pop_back();
    std::cout << stringFromZ(input) << std::endl;
    std::cin >> x;
    return 0;
}


/*
void findStringWithZ(std::string & pattern, std::string text)
{
    std::vector<int> ans = zFunction(pattern + '#' + text);
    for (int i = pattern.length() + 1; i < text.length(); ++i) {
        if (ans[i] == pattern.length()) {
            std::cout << i << ' ';
        }
    }
    std::cout << std::endl;
}

void findStringWithPrefix(std::string & pattern, std::string text)
{
    std::vector<int> ans = prefixFunction(pattern + '#' + text);
    int pl = pattern.length();
    int tl = text.length();
    int count = 0;
    for (int i = 0; i < tl; ++i) {
        if (ans[pl + i + 1] == pl) {
            ans[count++] = i;
            std::cout << i << ' ';
        }
    }
    std::cout << std::endl;
}*/