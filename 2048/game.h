#ifndef GAME_H
#define GAME_H

#include "2048.h"
#include "stack.h"
#include "player.h"
#include "board.h"
#include "settings.h"
#include "timer.h"

class Game {
	// Player data
	Player* player;
	Board* board;
	stack<Board>* undoStack, * redoStack;
	// Game data
	int dataSlot;
	char** dataSlots;
	linkedList<Player>* top20;
	Settings* settings;

	int updateRank();
	int getDataSlot(bool load);
	void showDataSlot();
	void checkDataFiles();
	void saveData();
	void loadData();
	bool confirm(string msg);

	void getPlayerName();
	void newGame();
	void resume();
	void showTop20();
	void showInstructions();
	void showSettings();
	void showHomeMenu();

	Board* saveState();
	void loadState(Board* state);
	void undo(Board* state);
	void redo(Board* state);
	void refresh();
	void showPauseMenu();
	void showLoseMessage();
	void showWinMessage();
	int processPauseMenu(int when);
	unsigned long long getBestScore();

public:
	Game();
	~Game();
	bool processHomeMenuEvents();
	bool processInGameEvents();
};

#endif