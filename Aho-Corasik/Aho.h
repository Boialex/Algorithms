#pragma once

#include <vector>
#include <string>
#include "stdafx.h"

struct BorVertex;

class Bor {

public:

    Bor();
    
    void input();
    void addWord(std::string & s, int count);
    void find(std::string & s);
    //void addVertex();

private:

    struct BorVertex {
        std::vector<BorVertex*> next;
        char c;
        BorVertex * parent;
        BorVertex * suffLink;
        BorVertex * terminalLink;
        bool endOfWord;
        std::vector<int> patternsEnding;
    };

    void countSufLinks();
    BorVertex * newVertex(BorVertex * parent, char c);
    void countLink(BorVertex * v);

    BorVertex * root;
    int patternSize;
    int patternLength;
};