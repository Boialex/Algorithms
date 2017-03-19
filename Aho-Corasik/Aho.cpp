#include "stdafx.h"

#include <iostream>
#include <queue>
#include <fstream>
#include "Aho.h"

Bor::Bor()
{
    root = new BorVertex;
    root->c = '@';
    root->endOfWord = false;
    root->parent = NULL;
    root->suffLink = root;
    root->next.assign(26, root);
}

void Bor::input()
{
    std::fstream cin;
    cin.open("C:\\Games\\input.txt");
    std::string s = "", mainS;
    cin >> mainS;
    int count = 0;
    int beginS = -1;
    for (int i = 0; i < mainS.length(); ++i) {
        if (mainS[i] == '?') {
            if (beginS != -1) {
                addWord(static_cast<std::string>(mainS.substr(beginS, i - beginS)), i);
                beginS = -1;
                //s = "";
                ++count;
            }
        }
        else {
            if (beginS == -1) {
                beginS = i;
            }
            //s += mainS[i];
        }
    }
    if (mainS[mainS.length() - 1] == '?') {
        patternSize = count;
    } else {
        patternSize = ++count;
    }
    patternLength = mainS.length();
    addWord(s, mainS.length());
    cin.close();

    countSufLinks();
}

void Bor::addWord(std::string & s, int count)
{
    if (s == "") {
        return;
    }
    BorVertex * v = root;
    for (int i = 0; i < s.length(); ++i) {
        char c = s[i] - 'a';
        if (v->next[c] == NULL || (v == root && v->next[c] == root)) {
            BorVertex * newV = newVertex(v, c);
            v->next[c] = newV;
        }
        v = v->next[c];
    }
    v->endOfWord = true;
    v->patternsEnding.push_back(count);
}

Bor::BorVertex * Bor::newVertex(BorVertex * parent, char c)
{
    BorVertex * v = new BorVertex;
    v->parent = parent;
    v->c = c;
    v->next.assign(26, NULL);
    v->suffLink = NULL;
    v->terminalLink = NULL;
    return v;
}

void Bor::countLink(BorVertex * v)
{
    if (v == root) {
        v->suffLink = v;
        v->terminalLink = NULL;
        return;
    }
    if (v->parent == root) {
        v->suffLink = root;
        v->terminalLink = NULL;
        return;
    }
    BorVertex * link = v->parent->suffLink;
    BorVertex * terminalLink = NULL;
    while (link->next[v->c] == NULL) {
        link = link->suffLink;
    }
    link = link->next[v->c];
 
    terminalLink = link;
    while (terminalLink->endOfWord == false && terminalLink != root) {
        terminalLink = terminalLink->suffLink;
    }
    v->suffLink = link;
    v->terminalLink = terminalLink;
}

void Bor::countSufLinks()
{
    std::queue<BorVertex*> bfs;
    bfs.push(root);
    while (!bfs.empty()) {
        BorVertex * v = bfs.front();
        bfs.pop();
        for (int i = 0; i < v->next.size(); ++i) {
            if (v->next[i] != NULL && v->next[i] != root) {
                bfs.push(v->next[i]);
            }
        }
        countLink(v);
    }
}

void Bor::find(std::string & s)
{
    std::vector<int> ans(s.length(), 0);
    BorVertex * v = root;

    for (int i = 0; i < s.length(); ++i) {
        char c = s[i] - 'a';
        if (v->next[c] != NULL) {
            v = v->next[c];
        } else {
            v = v->suffLink;
            while (v->next[c] == NULL) {
                v = v->suffLink;
            }
            v = v->next[c];
        }

        BorVertex * u = v;
        while (u != NULL) {
            if (u->endOfWord == true) {
                for (int j = 0; j < u->patternsEnding.size(); ++j) {
                    if (i + 1 >= u->patternsEnding[j]  && int(s.length()) - 1 - i + u->patternsEnding[j] >= patternLength) {
                        ++ans[i - u->patternsEnding[j] + 1];
                    }
                }
            }
            u = u->terminalLink;
        }
    }
    
    for (int i = 0; i < ans.size(); ++i) {
        if (ans[i] == patternSize) {
            std::cout << i << ' ';
        }
    }
    std::cout << std::endl;

}
