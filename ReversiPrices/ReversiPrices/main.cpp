//#pragma comment(linker, "/STACK:512777216")
#include "Reversi.h"
#include <ctime>

int main()
{
    srand(clock());
    Reversi newGame;
    newGame.startGame();
    return 0;
}