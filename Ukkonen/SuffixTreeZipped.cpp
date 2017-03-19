#include "stdafx.h"
#include "SuffixTreeZipped.h"

Node::Node()
{
    parent = NULL;
    suffLink = NULL;
    left = right = 0;
}

Node::Node(int alphabetSize) 
{
    parent = NULL;
    suffLink = NULL;
    next.assign(alphabetSize, NULL);
    left = right = 0;
}

Node::~Node()
{
    suffLink = NULL;
    parent = NULL;
    for (int i = 0; i < next.size(); ++i) {
        delete next[i];
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------

SuffixTreeZipped::SuffixTreeZipped()
{
    alphabetSize = 28;
    s = "";
    startPoint = new Node;
}
  
SuffixTreeZipped::~SuffixTreeZipped()
{
    delete root;
}

void SuffixTreeZipped::setAlphabetSize(int newSize)
{
    alphabetSize = newSize + 1;
}

void SuffixTreeZipped::buildTree(std::string & s)
{
    s += 'z' + 1;
    this->s = s;
    joker = new Node(alphabetSize);
    root = new Node(alphabetSize);
    for (int i = 0; i < alphabetSize; ++i) {
        joker->next[i] = root;
    }
    root->suffLink = joker;
    root->left = -1;
    startPoint = root;
    previousPoint = NULL;
    startPointInEdgeShift = 0;
    charOfPassingEdge = -1;

    for (int i = 0; i < s.length(); ++i) {
        char c = s[i] - MIN_LETTER;
        previousPoint = NULL;
        while (noEdgeFromStartPoint(c)) {
            if (startPointInEdgeShift != 0) {
                Node * newNode = new Node(alphabetSize);
                char cutChar = s[startPoint->next[charOfPassingEdge]->left + startPointInEdgeShift] - MIN_LETTER;
                newNode->next[cutChar] = startPoint->next[charOfPassingEdge];
                newNode->left = startPoint->next[charOfPassingEdge]->left;
                newNode->right = newNode->left + startPointInEdgeShift;
                newNode->parent = startPoint;

                startPoint->next[charOfPassingEdge]->left = newNode->right;
                startPoint->next[charOfPassingEdge]->parent = newNode;
                startPoint->next[charOfPassingEdge] = newNode;

                startPoint = newNode;
                startPointInEdgeShift = 0;
                charOfPassingEdge = -1;
            }
            if (previousPoint != NULL) {
                previousPoint->suffLink = startPoint;
            }
            previousPoint = startPoint;

            startPoint->next[c] = new Node(alphabetSize);
            startPoint->next[c]->parent = startPoint;
            startPoint->next[c]->left = i;
            startPoint->next[c]->right = INF;

            movePointToLink(startPoint);
            if (startPointInEdgeShift == 0) {
                previousPoint->suffLink = startPoint;
                previousPoint = NULL;
            }
        }
        if (startPointInEdgeShift == 0) {
            if (startPoint->next[c]->right - startPoint->next[c]->left == 1) {
                startPoint = startPoint->next[c];
                startPointInEdgeShift = 0;
                charOfPassingEdge = -1;
            } else {
                startPointInEdgeShift = 1;
                charOfPassingEdge = c;
            }
        } else {
            ++startPointInEdgeShift;
            int edgeLength = startPoint->next[charOfPassingEdge]->right - startPoint->next[charOfPassingEdge]->left;
            if (edgeLength == startPointInEdgeShift) {
                startPoint = startPoint->next[charOfPassingEdge];
                startPointInEdgeShift = 0;
                charOfPassingEdge = -1;
            }
        }
    }
}

bool SuffixTreeZipped::noEdgeFromStartPoint(char c)
{
    if (startPointInEdgeShift == 0) {
        if (startPoint->next[c] == NULL) {
            return true;
        } else {
            return false;
        }
    } else {
        char edgeChar = s[startPoint->next[charOfPassingEdge]->left + startPointInEdgeShift];
        if (edgeChar - MIN_LETTER == c) {
            return false;
        } else {
            return true;
        }
    }
}


void SuffixTreeZipped::movePointToLink(Node * currentNode)
{
    if (currentNode->suffLink != NULL) {
        startPoint = currentNode->suffLink;
        return;
    }
    startPoint = currentNode->parent->suffLink;
    int left = currentNode->left;
    int right = currentNode->right;
    int k = right - left;
    char c = s[left] - MIN_LETTER;
    int length = startPoint->next[c]->right - startPoint->next[c]->left;
    while (length <= right - left) {
        startPoint = startPoint->next[c];
        left += length;
        c = s[left] - MIN_LETTER;
        length = startPoint->next[c]->right - startPoint->next[c]->left;
    }
    charOfPassingEdge = c;
    startPointInEdgeShift = right - left;
}

int SuffixTreeZipped::subsequences() 
{
    int sum = 0;
    for (int i = 0; i < root->next.size(); ++i) {
        if (root->next[i] != NULL) {
            sum += subsequenceFromNode(root->next[i]);
        }
    }
    return sum;
}

int SuffixTreeZipped::subsequenceFromNode(Node * node)
{
    int sum = 0;
    for (int i = 0; i < node->next.size(); ++i) {
        if (node->next[i] != NULL) {
            sum += subsequenceFromNode(node->next[i]);
        }
    }
    if (sum == 0) {
        return s.length() - node->left;
    } else {
        return sum + node->right - node->left;
    }
}

std::string SuffixTreeZipped::LCS(std::string & s1, std::string & s2) 
{
    char c = 'z' + 2;
    std::string concut = s1 + c + s2;
    buildTree(concut);
    int maxLength = 0;
    Node * nodeMax = NULL;
    LCSNode(root, nodeMax, maxLength, 0, s2.length());
    std::string ans;
    while (nodeMax != root) {
        for (int i = nodeMax->right - 1; i >= nodeMax->left; --i) {
            ans = s[i] + ans;
        }
        nodeMax = nodeMax->parent;
    }
    return ans;
}

void SuffixTreeZipped::LCSNode(Node * node, Node * & nodeMax, int & currentMax, int length, int sizeBorder)
{
    bool hasLongS = false;
    bool hasShortS = false;
    bool hasBoth = false;
    for (int i = 0; i < node->next.size(); ++i) {
        if (node->next[i] != NULL) {
            int nodeLength = std::min(node->next[i]->right, int(s.length())) - node->next[i]->left;
            LCSNode(node->next[i], nodeMax, currentMax, nodeLength + length, sizeBorder);
            if (node->next[i]->type == Node::SuffixType::both) {
                hasBoth = true;
            }
            if (node->next[i]->type == Node::SuffixType::longS) {
                hasLongS = true;
            }
            if (node->next[i]->type == Node::SuffixType::shortS) {
                hasShortS = true;
            }
        }
    }
    if (!hasBoth && !hasLongS && !hasShortS) {
        if (length > sizeBorder + 1) {
            node->type = Node::SuffixType::longS;
        } else {
            node->type = Node::SuffixType::shortS;
        }
        return;
    }
    if (hasBoth || (hasLongS && hasShortS)) {
        node->type = Node::SuffixType::both;
        if (length > currentMax) {
            nodeMax = node;
            currentMax = length;
        }
        return;
    }
    if (hasLongS) {
        node->type = Node::SuffixType::longS;
        return;
    }
    if (hasShortS) {
        node->type = Node::SuffixType::shortS;
        return;
    }
}