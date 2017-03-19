//#pragma comment(linker, "/STACK:512777216")

#include "Reversi.h"

Place::Place() {
    c = 0;
    n = -1;
}

Place::Place(char newC, int newN) {
    c = newC;
    n = newN;
}

Reversi::Reversi()
{
    left = 60;
    
    direction.resize(8);
    direction[0] = std::make_pair(1, 0);
    direction[1] = std::make_pair(1, 1);
    direction[2] = std::make_pair(0, 1);
    direction[3] = std::make_pair(-1, 1);
    direction[4] = std::make_pair(-1, 0);
    direction[5] = std::make_pair(-1, -1);
    direction[6] = std::make_pair(0, -1);
    direction[7] = std::make_pair(1, -1);

    this->colour = 1;
}

void Reversi::startGame()
{
    std::string input;
    std::cin >> input;
    //std::fstream cout;
    //cout.open("C:\\Users\\lenovo\\Desktop\\clients\\output.txt");
    //cout.clear();
    if (input[0] == 'i') {
        std::string colour;
        std::cin >> colour;
        if (colour[0] == 'b') {
            myDesk[4][4] = myDesk[5][5] = 2;
            myDesk[4][5] = myDesk[5][4] = 1;
        }
        else {
            myDesk[4][4] = myDesk[5][5] = 1;
            myDesk[4][5] = myDesk[5][4] = 2;
        }
    }

    std::cin >> input;
    while (input[0] != 'b' && input[0] != 'w' &&
        input[0] != 'l' && input[0] != 'd') {
        char c = input[0];
        if (left <= endShpil) {
            watch = 12;
        }
        Place move;
        --left;
        switch (c)
        {
        case 'm':
            std::cin >> move.c >> move.n;
            reDraw(move, 2);
            clearLogs();
            break;
        case 't':
            time = clock();
            int balance;
            move = findMove(this->colour, 0, balance, INT_MIN, INT_MAX);
            reDraw(move, this->colour);
            clearLogs();
            
            if (slow) {
                watch = watch - 2;
                slow = false;
            }
            std::cout << "move " << move.c << ' ' << move.n << std::endl;
            if (double(clock() - time) / CLOCKS_PER_SEC < timeLeft - 2.4 && left > endShpil) {
                watch = watch + 2;
            }
            
            break;
        }
        std::cin >> input;
        //cout << watch << std::endl;
    }
}

void Reversi::reDraw(Place move, int colour)
{
    if (move.c == 0) {
        return;
    }
    bool sameColour = (this->colour == colour);
    int colour1, colour2;
    int count = 0;
    colour1 = colour;
    colour2 = (colour == 1 ? 2 : 1);
    int difference = 0;
    for (int i = 0; i < 8; ++i) {
        int k = 1;
        while (myDesk[int(move.c - 'a') + 1 + k * direction[i].first][move.n + k * direction[i].second] == colour2) {
            ++k;
        }
        if (k != 1 && myDesk[int(move.c - 'a') + 1 + k * direction[i].first][move.n + k * direction[i].second] == colour1) {
            --k;
            for (; k > 0; --k) {
                int m = int(move.c - 'a') + 1 + k * direction[i].first;
                int n = move.n + k * direction[i].second;
                myDesk[m][n] = colour1;
                difference += prices[m][n];
                logs.push(Change(Place(char(m + 'a' - 1), n), colour2, colour1));
            }
        }
    }
    difference = 2 * difference;

    int m = int(move.c - 'a') + 1;
    myDesk[m][move.n] = colour1;
    difference += prices[m][move.n];

    if (sameColour) {
        balance += difference;
    }
    else {
        balance -= difference;
    }

    logs.push(Change(move, 0, colour1));
    logs.push(Change(Place(), 0, 0));
}

Place Reversi::findMove(int colour, int depth, int & score, int alpha, int beta)
{
    if (depth == watch) {
        score = balance;
        denyChanges();
        return Place();
    }
    int antiColour = (colour == 1 ? 2 : 1);
    std::vector<Place> possibleMoves;
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            if (myDesk[i][j] == 0) {
                for (int v = 0; v < 8; ++v) {
                    int k = 1;
                    while (myDesk[i + k * direction[v].first][j + k * direction[v].second] == antiColour) {
                        ++k;
                    }
                    if (k != 1 && myDesk[i + k * direction[v].first][j + k * direction[v].second] == colour) {
                        possibleMoves.push_back(Place(char(i + 'a' - 1), j));
                    }
                }
            }
        }
    }
    if (possibleMoves.size() == 0) {
        possibleMoves.push_back(Place(0, 0));
    }
    Place bestMove;
    bool sameColour = (this->colour == colour);
    int minMax = (sameColour ? INT_MIN : INT_MAX);
    for (int i = 0; i < possibleMoves.size(); ++i) {
        reDraw(possibleMoves[i], colour);

        int newScore;
        findMove(antiColour, depth + 1, newScore, alpha, beta);
        if (sameColour && minMax < newScore ||
             !sameColour && minMax >= newScore) {
            bestMove = possibleMoves[i];
            minMax = newScore;
        }

        if (sameColour) {
            alpha = std::max(minMax, alpha);
        }
        else {
            beta = std::min(minMax, beta);
        }
        if (beta < alpha) {
            break;
        }
        
        if (double(clock() - time) / CLOCKS_PER_SEC > timeLeft) {
            if (minMax == INT_MIN || minMax == INT_MAX) {
                int minMax = (sameColour ? INT_MAX : INT_MIN);
            }
            slow = true;
            break;
        }
        
    }
    score = minMax;
    if (depth != 0) {
        denyChanges();
    }
    if (bestMove.n == -1) {
        return possibleMoves[rand() % possibleMoves.size()];
    }
    else {
        return bestMove;
    }
}

void Reversi::denyChanges()
{
    int k = 0;
    bool sameColour = true;
    if (!logs.empty())  {
        logs.pop();
    }
    if (!logs.empty() && logs.top().move.n != -1) {
        Change change = logs.top();
        sameColour = (change.newColour == this->colour);
        if (sameColour) {
            balance -= prices[change.move.c - 'a' + 1][change.move.n];
        }
        else {
            balance += prices[change.move.c - 'a' + 1][change.move.n];
        }
        myDesk[change.move.c - 'a' + 1][change.move.n] = change.oldColour;
        logs.pop();
    }
    int diffrence = 0;
    while (!logs.empty() && logs.top().move.n != -1) {
        Change change = logs.top();
        myDesk[change.move.c - 'a' + 1][change.move.n] = change.oldColour;
        diffrence += prices[change.move.c - 'a' + 1][change.move.n];
        logs.pop();
    }
    if (sameColour) {
        balance -= 2 * diffrence;
    }
    else {
        balance += 2 * diffrence;
    }
}

void Reversi::clearLogs()
{
    while (!logs.empty()) {
        logs.pop();
    }
}

