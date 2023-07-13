#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>

using namespace std;

class Player {
    private:
        string name;
        int score;

    public:
        Player(string name, int score);
        void setName(string newName);
        string getName() const;
        void setScore(int newScore);
        int getScore();
        bool operator<(const Player &other) const {
            if(score != other.score)
                return score > other.score;
            return name < other.name;
        }
};

#endif