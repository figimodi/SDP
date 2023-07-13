#include "leaderboard.hpp"

int main(int argc, char const *argv[])
{
    Leaderboard l;

    l.addPlayer("pippo", 10);
    l.addPlayer("ariano", 5);
    l.addPlayer("mario", 6);

    l.updateScore("ariano", 6);

    l.printTopPlayers(2);
}
