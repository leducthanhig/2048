#include "game.h"
// Create a backup version of the current state
Board* Game::saveState() {
	return board->copy();
}
// Load the state from a backup version
void Game::loadState(Board* state) {
	board->copy(state);
	delete state;
}
// Implement the undo process
void Game::undo(Board* state) {
	if (!undoStack->empty()) {
		redoStack->push(state);
		loadState(undoStack->pop_r());
		board->showUpdatedBoard(getBestScore());
	}
	else {
		system("cls");
		gotoxy((settings->getSize() * 8 + 1 + 3 + 34 + 3 + 2 - 14) / 2, (settings->getSize() * 4 + 1) * 0.45);
		cout << "Unable to undo!";
		Sleep(500);
		board->drawFrame();
		board->showUpdatedBoard(getBestScore());
		settings->showEnableUndoRedo();
	}
}
// Implement the redo process
void Game::redo(Board* state) {
	if (!redoStack->empty()) {
		undoStack->push(state);
		loadState(redoStack->pop_r());
		board->showUpdatedBoard(getBestScore());
	}
	else {
		system("cls");
		gotoxy((settings->getSize() * 8 + 1 + 3 + 34 + 3 + 2 - 14) / 2, (settings->getSize() * 4 + 1) * 0.45);
		cout << "Unable to redo!";
		Sleep(500);
		board->drawFrame();
		board->showUpdatedBoard(getBestScore());
		settings->showEnableUndoRedo();
	}
}

void Game::refresh() {
	updateRank();
	delete board;
	player->setTime(0);
	undoStack->clear();
	redoStack->clear();
	board = new Board(settings->getSize());
	board->spawnNum();
	board->spawnNum();
	board->drawFrame();
	board->showUpdatedBoard(getBestScore());
	settings->showEnableUndoRedo();
}

void Game::showPauseMenu() {
	system("cls");
	setConsoleWindowSize(30, 114);

	gotoxy((114 - 10) / 2 - 2, 9);
	cout << "╔";
	gotoxy((114 - 10) / 2 + 10 + 1);
	cout << "╗\n";
	for (int i = 0; i < 6; i++) {
		gotoxy((114 - 10) / 2 - 2);
		cout << "║";
		gotoxy((114 - 10) / 2 + 10 + 1);
		cout << "║\n";
	}
	gotoxy((114 - 10) / 2 - 2);
	cout << "╚";
	for (int i = 0; i < 12; i++) cout << "═";
	cout << "╝";

	gotoxy((114 - 10) / 2, 9);
	cout << "PAUSE MENU";
	gotoxy((114 - 8) / 2, 9 + 2);
	cout << "CONTINUE\n";
	gotoxy((114 - 8) / 2);
	cout << "NEW GAME\n";
	gotoxy((114 - 4) / 2);
	cout << "HOME\n";
	gotoxy((114 - 4) / 2);
	cout << "EXIT\n";
}

unsigned long long Game::getBestScore() {
	if (top20->getSize() == 0 || top20->getHead()->getData()->getScore() < board->getScore()) return board->getScore();
	return top20->getHead()->getData()->getScore();
}

int Game::processPauseMenu(int when) {
	if (when == 1) showWinMessage();
	else if (when == 0) showPauseMenu();
	else {
		showLoseMessage();
		loadState(undoStack->pop_r());
	}
	string choices[] = { "CONTINUE", "NEW GAME", "HOME", "EXIT" };
	int choice = 1; // 1: continue, 2: new game, 3: home, 4: exit
	int input;

	highlightText((114 - choices[choice - 1].length()) / 2, 9 + 1 + choice, choices[choice - 1]);

	while (1) {
		input = _getch();
		switch (input) {
		case KEY_VIRTUAL: // To ignore some virtual keys
			input = _getch();
			break;
		case KEY_ARROW:
			input = _getch();
			if (choice < 4 && input == KEY_DOWN_ARROW) {
				unhighlightText((114 - choices[choice - 1].length()) / 2, 9 + 1 + choice, choices[choice - 1]);
				highlightText((114 - choices[choice].length()) / 2, 9 + 1 + choice + 1, choices[choice]);
				choice++;
			}
			else if (choice > 1 && input == KEY_UP_ARROW) {
				unhighlightText((114 - choices[choice - 1].length()) / 2, 9 + 1 + choice, choices[choice - 1]);
				highlightText((114 - choices[choice - 2].length()) / 2, 9 + 1 + choice - 1, choices[choice - 2]);
				choice--;
			}
			break;
		case KEY_ENTER: case KEY_SPACEBAR:
			switch (choice) {
			case 1:
				if (when == -1) {
					system("cls");
					gotoxy((114 - 65) / 2, 11);
					cout << "You will be rolled back to the previous step to continue playing!";
					Sleep(1200);
				}
				board->drawFrame();
				board->showUpdatedBoard(getBestScore());
				settings->showEnableUndoRedo();
				return 3;
			case 2: 
				if (when == 0) updateRank();
				return 2;
			case 3:
				if (when == 0) updateRank();
				if (confirm("save game")) {
					if (dataSlot != 0) {
						system("cls");
						gotoxy(54, 11);
						cout << "SAVING";
						for (int i = 0; i < 3; i++) {
							cout << ".";
							Sleep(250);
						}
						saveData();
					}
					else {
						do {
							dataSlot = getDataSlot(0);
							if (dataSlot != -1) {
								system("cls");
								gotoxy(54, 11);
								cout << "SAVING";
								for (int i = 0; i < 3; i++) {
									cout << ".";
									Sleep(250);
								}
								saveData();
							}
						} while (dataSlot == -1);
					}
				}
				return 1;
			case 4: 
				if (confirm("exit game")) {
					if (when == 0) updateRank();
					if (confirm("save game")) {
						if (dataSlot != 0) saveData();
						else {
							do {
								dataSlot = getDataSlot(0);
								if (dataSlot != -1) {
									system("cls");
									gotoxy(54, 11);
									cout << "SAVING";
									for (int i = 0; i < 3; i++) {
										cout << ".";
										Sleep(250);
									}
									saveData();
								}
							} while (dataSlot == -1);
						}
					}
					return 0;
				}
				else {
					if (when == 1) showWinMessage();
					else if (when == -1) showLoseMessage();
					else showPauseMenu();
					highlightText((114 - choices[choice - 1].length()) / 2, 9 + 1 + choice, choices[choice - 1]);
				}
				break;
			}
			break;
		}
	}
}

void Game::showLoseMessage() {
	system("cls");
	setConsoleWindowSize(30, 114);
	
	int rank = updateRank();

	gotoxy((114 - 16) / 2 - 2, 9 - (rank ? 2 : 0));
	cout << "╔";
	gotoxy((114 - 16) / 2 + 16 + 1);
	cout << "╗\n";
	for (int i = 0; i < 6 + (rank ? 2 : 0); i++) {
		gotoxy((114 - 16) / 2 - 2);
		cout << "║";
		gotoxy((114 - 16) / 2 + 16 + 1);
		cout << "║\n";
	}
	gotoxy((114 - 16) / 2 - 2);
	cout << "╚";
	for (int i = 0; i < 18; i++) cout << "═";
	cout << "╝";

	gotoxy((114 - 16) / 2, 9 - (rank ? 2 : 0));
	cout << "UNABLE TO MERGE!";
	
	if (rank) {
		gotoxy((114 - 16) / 2, 7 + 2);
		cout << "YOUR RANK IS:" << right << setw(3) << rank;
	}
	gotoxy((114 - 8) / 2, 9 + 2);
	cout << "CONTINUE\n";
	gotoxy((114 - 8) / 2);
	cout << "NEW GAME\n";
	gotoxy((114 - 4) / 2);
	cout << "HOME\n";
	gotoxy((114 - 4) / 2);
	cout << "EXIT\n";
}

void Game::showWinMessage() {
	system("cls");
	setConsoleWindowSize(30, 114);

	int rank = updateRank();

	gotoxy((114 - 33) / 2 - 2, 9 - (rank ? 2 : 0));
	cout << "╔";
	gotoxy((114 - 33) / 2 + 33 + 1);
	cout << "╗\n";
	for (int i = 0; i < 6 + (rank ? 2 : 0); i++) {
		gotoxy((114 - 33) / 2 - 2);
		cout << "║";
		gotoxy((114 - 33) / 2 + 33 + 1);
		cout << "║\n";
	}
	gotoxy((114 - 33) / 2 - 2);
	cout << "╚";
	for (int i = 0; i < 35; i++) cout << "═";
	cout << "╝";

	gotoxy((114 - 33) / 2, 9 - (rank ? 2 : 0));
	cout << "CONGRATULATIONS ON REACHING 2048!";
	
	if (rank) {
		gotoxy((114 - 16) / 2, 7 + 2);
		cout << "YOUR RANK IS:" << right << setw(3) << rank;
	}
	gotoxy((114 - 8) / 2, 9 + 2);
	cout << "CONTINUE\n";
	gotoxy((114 - 8) / 2);
	cout << "NEW GAME\n";
	gotoxy((114 - 4) / 2);
	cout << "HOME\n";
	gotoxy((114 - 4) / 2);
	cout << "EXIT\n";
}
// Process player input events
bool Game::processInGameEvents() {
	Timer* timer = new Timer;
	timer->start();
	timer->addSecs(player->getTime());
	board->drawFrame();
	board->showUpdatedBoard(getBestScore());
	settings->showEnableUndoRedo();

	int input;
	int ret; // 0: exit, 1: home, 2: refresh, 3: continue
	int changed; // 0: not change, 1: changed, 2: won
	Board* backup;
	while (1) {
		changed = 0;
		input = tolower(_getch());
		if (input == 0 || input == 224) { // Check if the input key is one of virtual keys
			input = _getch();
			// Ignore if not an arrow key
			if (input != KEY_DOWN_ARROW && input != KEY_UP_ARROW && input != KEY_LEFT_ARROW && input != KEY_RIGHT_ARROW) continue;
		}
		backup = saveState();

		switch (input) {
		case 'w': case KEY_UP_ARROW:
			changed = board->moveDownToUp();
			break;
		case 'a': case KEY_LEFT_ARROW:
			changed = board->moveRightToLeft();
			break;
		case 'd': case KEY_RIGHT_ARROW:
			changed = board->moveLeftToRight();
			break;
		case 's': case KEY_DOWN_ARROW:
			changed = board->moveUpToDown();
			break;
		case 'u': // Undo
			if (settings->isUndoEnable()) undo(backup);
			break;
		case 'r': // Redo
			if (settings->isRedoEnable()) redo(backup);
			break;
		case 'p': case KEY_ESCAPE: // Pause game
			timer->pause();
			player->setTime(timer->getTotalTime());
			ret = processPauseMenu(0);
			timer->resume();
			if (ret != 3) delete timer;
			if (ret == 2) {
				timer = new Timer;
				timer->start();
				refresh();
			}
			else if (ret == 0 || ret == 1) return ret;
			break;
		default:
			delete backup;
			continue;
		}
		if (changed) {
			board->spawnNum();
			board->showUpdatedBoard(getBestScore());
			undoStack->push(backup);
			redoStack->clear();

			if ((board->isFull() && !board->canMove()) || changed == 2) {
				timer->pause();
				player->setTime(timer->getTotalTime());
				ret = processPauseMenu(changed == 2 ? 1 : -1);
				timer->resume();
				if (ret != 3) delete timer;
				if (ret == 2) {
					timer = new Timer;
					timer->start();
					refresh();
				}
				else if (ret == 0 || ret == 1) return ret;
			}
		}
		else { // not changed
			if (backup != redoStack->top() && backup != undoStack->top()) {
				delete backup;
			}
		}
	}
}