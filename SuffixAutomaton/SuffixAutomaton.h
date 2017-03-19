#include <vector>
#include <string>


class SuffixAutomaton {
public:
    SuffixAutomaton(std::string & s);
    ~SuffixAutomaton();

    long long countSubstrings();

private:
    struct Node{
        Node(int alphabetSize);
        ~Node();

        int length;
        int count;
        std::vector<Node*> next;
        Node * suffLink;
    };

    void addChar(char c);
    bool noPassage(Node * node, char c);
    long long dfs(Node * node);

    static const char MIN_LETTER = 'a';
    int alphabetSize = 26;
    Node * root;
    Node * startPoint;
};