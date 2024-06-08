#ifndef BOARD_H
#define BOARD_H

#include "2048.h"

class Board {
	int** data, size, nonZeroCells;
	unsigned long long score;

public:
	Board();
	Board(int size);
	~Board();
	unsigned long long getScore();
	int getSize();
	int moveUpToDown();
	int moveDownToUp();
	int moveLeftToRight();
	int moveRightToLeft();
	Board* copy();
	void copy(Board* board);
	void setSize(int size);
	void spawnNum();
	void drawFrame();
	void showUpdatedBoard(unsigned long long bestScore);
	void serialize(fstream& fs, bool bWrite);
	bool canMove();
	bool isFull();
};

#endif