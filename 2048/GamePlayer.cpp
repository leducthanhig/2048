#include "GamePlayer.h"
// Constructor
GamePlayer::GamePlayer() {
	// Change code page to display Unicode characters
	system("chcp 65001");
	// Set random seed
	srand(time(0));
	
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
		exit(-1);
	}
	catch (...) {
		cerr << "Unknown error!\n";
		system("pause");
		exit(-1);
	}
}
// Destructor
GamePlayer::~GamePlayer() {
	system("cls");
	gotoxy(53, 11);
	cout << "EXITING";
	for (int i = 0; i < 3; i++) {
		cout << ".";
		Sleep(250);
	}
	delete game;
	game = nullptr;
}