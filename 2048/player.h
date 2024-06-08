#ifndef PLAYER_H
#define PLAYER_H

#include "2048.h"

class Player {
	char name[29];
	unsigned long long score;
	time_t time;

public:
	Player();
	Player(const char* name, unsigned long long score, time_t time);
	const char* getName();
	unsigned long long getScore();
	time_t getTime();
	Player* copy();
	void setName(char* name);
	void setScore(unsigned long long score);
	void setTime(time_t time);
	void serialize(fstream& fs, bool bWrite);
};

bool muchTimeLessScore(Player* pl1, Player* pl2);
bool sameName(Player* pl1, Player* pl2);

#endif