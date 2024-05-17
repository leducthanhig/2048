#include "GamePlayer.h"

// Constructor
GamePlayer::GamePlayer() {
	// Change code page to display Unicode characters
	system("chcp 65001");
	srand(time(0));
	
	int request = 0; // [-1] exit, [0] go home, [1] new game
	while (1) {
		game = new Game;
		game->showCursor(0);
		if (request == 0 && !game->home()) {
			request = -1;
		}
		
		if (request != -1) {
			if (game->state == 'n' || game->state == 'l' || request == 1) game->init();
			game->drawFrame();
			request = game->processEvents();

			game->updateRank();
			if (game->state != 'n') game->saveData();
			delete game;
		} 
		else break;
	}
}

// Destructor
GamePlayer::~GamePlayer() {
	delete game;
}
