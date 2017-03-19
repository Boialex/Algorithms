#include "stdafx.h"
#include <vector>
#include <string>

struct Node {
    Node();
    Node(int alphabetSize);
    ~Node();

    Node * parent;
    int left;
    int right;
    Node * suffLink;
    std::vector<Node*> next;
    enum SuffixType {
        longS, shortS, both
    } type;
};

class SuffixTreeZipped {
public:
    SuffixTreeZipped();
    SuffixTreeZipped(int alphabetSize);
    ~SuffixTreeZipped();

    void setAlphabetSize(int newSize);
    void buildTree(std::string & s);
    std::string LCS(std::string & s1, std::string & s2);
    int subsequences();
private:
    bool noEdgeFromStartPoint(char c);
    void movePointToLink(Node * currentNode);
    int subsequenceFromNode(Node * node);
    void LCSNode(Node * node, Node * & nodeMax,int & currentMax, int length, int sizeBorder);


    const int INF = 1E9;
    const char MIN_LETTER = 'a';

    int alphabetSize = 27;
    Node * joker;
    Node * root;

    Node * startPoint;
    Node * previousPoint;
    int startPointInEdgeShift;
    int charOfPassingEdge;

    std::string s;
};
