#include <set>
#include "player.hpp"

class Leaderboard {
    private:
        set<Player> tier;

    public:
        Leaderboard();
        void addPlayer(const std::string& name, int score);
        void removePlayer(const std::string& name);
        void updateScore(const std::string& name, int newScore);
        void printTopPlayers(int n);
};