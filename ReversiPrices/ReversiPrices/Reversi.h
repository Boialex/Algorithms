#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <stack>

struct Place {
    Place();
    Place(char newC, int newN);

    bool operator ==(Place & move)
    {
        return (c == move.c) && (n == move.n);
    }

    char c;
    int n;
};

struct Change {
    Change(Place move, int oldColour, int newColour) : move(move), oldColour(oldColour),
        newColour(newColour) {};

    Place move;
    int oldColour;
    int newColour;
};

class Reversi {
public:
    Reversi();

    void startGame();

private:
    void reDraw(Place move, int colour);
    Place findMove(int colour, int depth, int & scort, int aplha, int beta);

    void denyChanges();
    void clearLogs();
    
    std::vector < std::pair<int, int> >  direction;

    int colour;
    int watch = 6;
    int endShpil = 12;
    bool slow = false;
    int time;
    int myDesk[10][10];
    int left = 60;

    std::stack<Change> logs;
    
    int balance = 0;
    double timeLeft = 2.980;
};

static const int prices[10][10] =
{
    { 0,  0,   0,  0,  0,  0,  0,   0,  0, 0 },
    { 0, 99,  -8,  8,  6,  6,  8,  -8, 99, 0 },
    { 0, -8, -24, -4, -3, -3, -4, -24, -8, 0 },
    { 0,  8,  -4,  7,  4,  4,  7,  -4,  8, 0 },
    { 0,  6,  -3,  4,  0,  0,  4,  -3,  6, 0 },
    { 0,  6,  -3,  4,  0,  0,  4,  -3,  6, 0 },
    { 0,  8,  -4,  7,  4,  4,  7,  -4,  8, 0 },
    { 0, -8, -24, -4, -3, -3, -4, -24, -8, 0 },
    { 0, 99,  -8,  8,  6,  6,  8,  -8, 99, 0 },
    { 0,  0,   0,  0,  0,  0,  0,   0,  0, 0 },
};