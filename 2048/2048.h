#ifndef _2048_H_
#define _2048_H_

#include "stack.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <windows.h>
using namespace std;

#define KEY_UP_ARROW 72
#define KEY_DOWN_ARROW 80
#define KEY_LEFT_ARROW 75
#define KEY_RIGHT_ARROW 77
#define KEY_ESCAPE 27

struct Player {
	char name[29];
	int score;

	Player(char name[], int score);
};

struct Game {
	int size, bestScore, score, **arr, cnt;
	char state; // [n] not init yet, [l] lose, [w] win, [p] playing
	char playerName[29];
	stack<Game*> *undo, *redo;
	linkedList<Player*>* rank;

	Game();
	~Game();
	void init();
	void gotoxy(int x, int y);
	void drawFrame();
	void drawHome();
	void spawnNum();
	bool home();
	bool canMove();
	bool upToDown();
	bool downToUp();
	bool leftToRight();
	bool rightToLeft();
	int processEvents();
	int lose();
	int win();
	Game* save();
	void load(Game* game);
	void undoIml(Game* game);
	void redoIml(Game* game);
	void saveData();
	void loadData();
	void getPlayerName();
	void updateState();
	void updateRank();
	void showRank();
	void showInfo();
	void showCursor(bool option);
	void highlightText(int x, int y, string text);
	void unhighlightText(int x, int y, string text);
};

#endif