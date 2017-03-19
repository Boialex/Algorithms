#include "SuffixAutomaton.h"

SuffixAutomaton::Node::Node(int alphabetSize)
{
    length = 0;
    count = 0;
    next.assign(alphabetSize, NULL);
    suffLink = NULL;
}

SuffixAutomaton::Node::~Node()
{
    if (this == NULL) {
        return;
    }
    suffLink = NULL;
    for (int i = 0; i < next.size(); ++i) {
        next[i] = dynamic_cast<Node*>(next[i]);
        if (next[i] != NULL)
            delete next[i];
    }
}

//-------------------------------------------------------------------------------------------------------------

SuffixAutomaton::SuffixAutomaton(std::string & s)
{
    alphabetSize = 26;
    root = new Node(alphabetSize);

    startPoint = root;
    for (int i = 0; i < s.length(); ++i) {
        addChar(s[i] - MIN_LETTER);
    }
}

SuffixAutomaton::~SuffixAutomaton()
{
    //delete root;
}

void SuffixAutomaton::addChar(char c)
{
    Node * newNode = new Node(alphabetSize);
    newNode->length = startPoint->length + 1;
    Node * p = startPoint;
    startPoint = newNode;
    while (p != NULL && noPassage(p, c)) {
        p->next[c] = newNode;
        p = p->suffLink;
    }
    if (p == NULL) {
        newNode->suffLink = root;
        return;
    }
    Node * q = p->next[c];
    if (p->length + 1 == q->length) {
        newNode->suffLink = q;
    }
    else {
        Node * clone = new Node(alphabetSize);
        clone->next = q->next;
        clone->length = p->length + 1;
        clone->suffLink = q->suffLink;
        newNode->suffLink = clone;
        q->suffLink = clone;

        while (p != NULL && p->next[c] == q) {
            p->next[c] = clone;
            p = p->suffLink;
        }
    }
}

bool SuffixAutomaton::noPassage(Node * node, char c)
{
    if (node->next[c] == NULL) {
        return true;
    }
    else {
        return false;
    }
}

long long SuffixAutomaton::countSubstrings()
{
    return dfs(root) - 1;
}

long long SuffixAutomaton::dfs(Node * node)
{
    if (node->count != 0) {
        return node->count;
    }
    else {
        ++(node->count);
        for (int i = 0; i < node->next.size(); ++i) {
            if (node->next[i] != NULL) {
                node->count += dfs(node->next[i]);
            }
        }
    }
    return node->count;
}