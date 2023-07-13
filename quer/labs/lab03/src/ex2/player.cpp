#include <string>
#include "player.hpp"

using namespace std;

Player::Player(string name, int score) {
    this->name = name;
    this->score = score;
}

void Player::setName(string newName) {
    this->name = newName;
}

string Player::getName() const {
    return this->name;
}

void Player::setScore(int newScore) {
    this->score = newScore;
}

int Player::getScore() {
    return this->score;
}
