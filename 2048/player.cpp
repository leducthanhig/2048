#include "player.h"

Player::Player() {
    strcpy_s(name, "");
    score = 0;
    time = 0;
}

Player::Player(const char* name, unsigned long long score, time_t time) {
    strcpy_s(this->name, name);
    this->score = score;
    this->time = time;
}

Player* Player::copy() {
    return new Player(name, score, time);
}

const char* Player::getName() {
    return name;
}

unsigned long long Player::getScore() {
    return score;
}

time_t Player::getTime() {
    return time;
}

void Player::setName(char* name) {
    strcpy_s(this->name, name);
}

void Player::setScore(unsigned long long score) {
    this->score = score;
}
void Player::setTime(time_t time) {
    this->time = time;
}
//[Reading and writing classes with pointers to binary files in c++](https://stackoverflow.com/a/32833242)
void Player::serialize(fstream& fs, bool bWrite) {
    if (bWrite) {
        encrypt(name);
        fs.write(reinterpret_cast<char*>(name), sizeof(name));
        fs.write(reinterpret_cast<char*>(&score), sizeof(score));
        fs.write(reinterpret_cast<char*>(&time), sizeof(time));
    }
    else {
        fs.read(reinterpret_cast<char*>(name), sizeof(name));
        fs.read(reinterpret_cast<char*>(&score), sizeof(score));
        fs.read(reinterpret_cast<char*>(&time), sizeof(time));
        encrypt(name);
    }
}

bool muchTimeLessScore(Player* pl1, Player* pl2) {
    if (pl1->getScore() == pl2->getScore()) return (pl1->getTime() > pl2->getTime());
    return (pl1->getScore() < pl2->getScore());
}

bool sameName(Player* pl1, Player* pl2) {
    return (strcmp(pl1->getName(), pl2->getName()) == 0);
}
