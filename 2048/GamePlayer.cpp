#include "GamePlayer.h"

GamePlayer::GamePlayer() {
	// Make the console not to be resized and closed
	setConsoleProperties();
	// Change code page to display Unicode characters
	system("chcp 65001");
	// Set random seed
	srand(time(0));
	// Hide the console cursor
	showCursor(0);
	
	try {
		// This loop only broken when processHomeMenuEvents or processInGameEvents function return 0 
		// which means the player has chosen to exit
		while (1) {
			game = new Game;
			if (!game->processHomeMenuEvents()) break;
			if (!game->processInGameEvents()) break;
			delete game;
			game = nullptr;
		}
	}
	catch (const bad_alloc& e) {
		cerr << "Error: " << e.what() << endl;
		system("pause");
	}
	catch (...) {
		cerr << "Unexpected error!\n";
		system("pause");
	}
}

GamePlayer::~GamePlayer() {
	showMessage("EXITING", 57, 13);
	delete game;
	game = nullptr;
}