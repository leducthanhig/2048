#include "game.h"
// Draw the home menu
void Game::showHomeMenu() {
	// Resize the console window to 1024x576
	resizeConsoleWindow(1024, 576);
	system("cls");

	cout << "\n\n";
	cout << setw(65) << right << "2============0\n";
	cout << setw(65) << right << "| WELCOME TO |\n";
	cout << setw(67) << right << "4============8\n\n\n";
	cout << setw(89) << right << " _________         _________                 __        _________\n";
	cout << setw(90) << right << "/222222222\\       /000000000\\               /4/       /888888888\\\n";
	cout << setw(90) << right << "|2|     /2/       |0|     |0|              /4/        |8|     |8|\n";
	cout << setw(90) << right << "       /2/        |0|     |0|             /4/         |8|     |8|\n";
	cout << setw(90) << right << "      /2/         |0|     |0|            /4/          |8|     |8|\n";
	cout << setw(90) << right << "     /2/          |0|     |0|           /4/           |8|_____|8|\n";
	cout << setw(90) << right << "    /2/           |0|     |0|          /4/            |888888888|\n";
	cout << setw(90) << right << "   /2/            |0|     |0|         /4/  _          |8|     |8|\n";
	cout << setw(90) << right << "  /2/             |0|     |0|        /4/__|4|_        |8|     |8|\n";
	cout << setw(90) << right << " /2/              |0|     |0|       /444444444|       |8|     |8|\n";
	cout << setw(90) << right << "/2/_______        |0|_____|0|             |4|         |8|_____|8|\n";
	cout << setw(92) << right << "|222222222|       \\000000000/             |4|         \\888888888/\n\n\n";

	gotoxy(57 - 4, 21);
	cout << "NEW GAME\n";
	gotoxy(57 - 3);
	cout << "RESUME\n";
	gotoxy(57 - 3);
	cout << "TOP-20\n";
	gotoxy(57 - 4);
	cout << "SETTINGS\n";
	gotoxy(57 - 6);
	cout << "INSTRUCTIONS\n";
	gotoxy(57 - 2);
	cout << "EXIT\n";
}
// Get and check if the player name is already exist
void Game::getPlayerName() {
	while (1) {
		system("cls");

		gotoxy(40, 10);
		cout << "╔═ ENTER YOUR NAME ════════════╗\n";
		gotoxy(40);
		cout << "║                              ║\n";
		gotoxy(40);
		cout << "╚══════════════════════════════╝\n";

		showCursor(1);
		gotoxy(42, 11);
		int input, len = 0;
		char* name = new char[29]();
		while (1) {
			input = _getch();
			if (input == KEY_VIRTUAL || input == KEY_ARROW) { // To ignore some virtual keys
				input = _getch(); // To clear the buffer
			}
			else if (32 <= input && input <= 126 && len < 28) {
				cout << (char)input;
				name[len++] = input;
			}
			else if (input == '\b' && len) {
				gotoxy(41 + len);
				cout << " ";
				gotoxy(41 + len);
				name[--len] = '\0';
			}
			else if (input == KEY_ENTER && len) {
				break;
			}
		}
		showCursor(0);

		strip(name);
		player->setName(name);
		delete[] name;
		if (top20->find(player, sameName) != -1) {
			system("cls");
			gotoxy(31, 11);
			cout << "This name is already exits. Please use another name!";
			Sleep(1000);
		}
		else break;
	}
}

void Game::newGame() {
	dataSlot = 0;
	getPlayerName();
	board = new Board(settings->getSize());
	board->spawnNum();
	board->spawnNum();
}
void Game::resume() {
	dataSlot = getDataSlot(1);
	if (dataSlot == -1) return;
	
	system("cls");
	gotoxy(53, 11);
	cout << "LOADING";
	for (int i = 0; i < 3; i++) {
		cout << ".";
		Sleep(250);
	}
	board = new Board;
	loadData();
	settings->setSize(board->getSize());
}
// Show top-20 list
void Game::showTop20() {
	system("cls");
	gotoxy(52, 1);
	cout << "~ TOP 20 ~\n";
	gotoxy(26);
	cout << "╔═════╦══════════════════════════════╦═════════════╦══════════╗\n";
	gotoxy(26);
	cout << "║ Top ║             Name             ║    Score    ║   Time   ║\n";
	gotoxy(26);
	cout << "╠═════╬══════════════════════════════╬═════════════╬══════════╣\n";
	
	int cnt = 0;
	string st;
	node<Player>* tmp = top20->getHead();
	while (tmp) {
		gotoxy(26);
		cout << "║" << right << setw(3) << ++cnt;
		gotoxy(26 + 6);
		cout << "║ " << tmp->getData()->getName();
		gotoxy(26 + 37);
		cout << "║";
		st = to_string(tmp->getData()->getScore());
		gotoxy(26 + 38 + (13 - st.length()) / 2);
		cout << st;
		gotoxy(26 + 51);
		cout << "║";
		st = formatTime(tmp->getData()->getTime());
		gotoxy(26 + 52 + (10 - st.length()) / 2);
		cout << st;
		gotoxy(26 + 62);
		cout << "║\n";
		tmp = tmp->getNext();
	}
	gotoxy(26);
	cout << "╚═════╩══════════════════════════════╩═════════════╩══════════╝";
	highlightText(55, 5 + TOP_LIST_LIMIT + 2, "CLOSE");

	int input;
	while (1) {
		input = _getch();
		if (input == KEY_VIRTUAL || input == KEY_ARROW) { // To ignore some virtual keys
			input = _getch(); // To clear the buffer
		}
		else if (input == KEY_SPACEBAR || input == KEY_ENTER) break;
	}
}

void Game::showInstructions() {
	system("cls");
	gotoxy(37, 8);
	cout << "╔════════════ INSTRUCTIONS ════════════╗\n";
	gotoxy(37);
	cout << "║                                      ║\n";
	gotoxy(37);
	cout << "║   Move up:        W or UP ARROW      ║\n";
	gotoxy(37);
	cout << "║   Move down:      S or DOWN ARROW    ║\n";
	gotoxy(37);
	cout << "║   Move to left:   A or LEFT ARROW    ║\n";
	gotoxy(37);
	cout << "║   Move to right:  D or RIGHT ARROW   ║\n";
	gotoxy(37);
	cout << "║   Undo / Redo:    U / R              ║\n";
	gotoxy(37);
	cout << "║                                      ║\n";
	gotoxy(37);
	cout << "╚══════════════════════════════════════╝\n";
	highlightText(55, 20, "CLOSE");

	int input;
	while (1) {
		input = _getch();
		if (input == KEY_VIRTUAL || input == KEY_ARROW) { // To ignore some virtual keys
			input = _getch(); // To clear the buffer
		}
		else if (input == KEY_SPACEBAR || input == KEY_ENTER) break;
	}
}

void Game::showSettings() {
	settings->showSettings();
}
// Show home menu
bool Game::processHomeMenuEvents() {
	showHomeMenu();

	string choices[] = { "NEW GAME", "RESUME", "TOP-20", "SETTINGS", "INSTRUCTIONS", "EXIT" };
	int choice = 1; // 1: new game, 2: resume, 3: top-20, 4: settings, 5: instructions, 6: exit
	int input;
	void (Game::*funcs[])() = { &Game::newGame, &Game::resume, &Game::showTop20, &Game::showSettings, &Game::showInstructions };

	highlightText(57 - choices[choice - 1].length() / 2, 20 + choice, choices[choice - 1]);

	bool canResume = any(dataSlots, DATA_SLOT);
	if (!canResume) {
		HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		gotoxy(57 - choices[1].length() / 2, 20 + 2);
		SetConsoleTextAttribute(console_color, 8);
		cout << choices[1];
		SetConsoleTextAttribute(console_color, 15);
	}

	while (1) {
		// [Capture characters from standard input without waiting for enter to be pressed](https://stackoverflow.com/a/421871)
		input = _getch();
		switch (input) {
		case KEY_VIRTUAL: // To ignore some virtual keys
			input = _getch();
			break;
		case KEY_ARROW:
			input = _getch();
			if (choice < 6 && input == KEY_DOWN_ARROW) {
				unhighlightText(57 - choices[choice - 1].length() / 2, 20 + choice, choices[choice - 1]);
				// Skip option "RESUME"
				if (!canResume && choice == 1) choice++;
				highlightText(57 - choices[choice].length() / 2, 20 + choice + 1, choices[choice]);
				choice++;
			}
			else if (choice > 1 && input == KEY_UP_ARROW) {
				unhighlightText(57 - choices[choice - 1].length() / 2, 20 + choice, choices[choice - 1]);
				// Skip option "RESUME"
				if (!canResume && choice == 3) choice--;
				highlightText(57 - choices[choice - 2].length() / 2, 20 + choice - 1, choices[choice - 2]);
				choice--;
			}
			break;
		case KEY_ENTER: case KEY_SPACEBAR:
			if (choice == 6) return 0;
			(this->*funcs[choice - 1])();
			if (choice < 3 && dataSlot != -1) return 1;
			showHomeMenu();
			if (!canResume) {
				HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
				gotoxy(57 - choices[1].length() / 2, 20 + 2);
				SetConsoleTextAttribute(console_color, 8);
				cout << choices[1];
				SetConsoleTextAttribute(console_color, 15);
			}
			highlightText(57 - choices[choice - 1].length() / 2, 20 + choice, choices[choice - 1]);
			break;
		}
	}
}