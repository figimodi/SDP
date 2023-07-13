#include <set>
#include <string>
#include <iostream>
#include "leaderboard.hpp"

Leaderboard::Leaderboard() {

}

void Leaderboard::addPlayer(const std::string& name, int score) {
    this->tier.emplace(name, score);
}

void Leaderboard::removePlayer(const std::string& name) {
    auto it = this->tier.begin();

    for(; it != this->tier.end(); it++)
    {
        if((*it).getName() == name)
        {
            this->tier.erase(it);
            break;
        }
    }
}

void Leaderboard::updateScore(const std::string& name, int newScore) {
    auto it = this->tier.begin();

    for(; it != this->tier.end(); it++)
    {
        if((*it).getName() == name)
        {
            this->tier.erase(it);
            addPlayer(name, newScore);
            break;
        }
    }
}

void Leaderboard::printTopPlayers(int n) {
    auto it = this->tier.begin();

    for(; it != this->tier.end(); it++)
    {
        std::cout << (*it).getName() << std::endl;
        n--;
        if (n == 0)
            break;
    }
}