#include "2048.h"

// Constructor of struct Player
Player::Player(char name[], int score) {
	strcpy_s(this->name, name);
	this->score = score;
}

// Constructor of struct Game
Game::Game() {
	size = bestScore = score = cnt = 0;
	arr = NULL;
	state = 'n';
	undo = new stack<Game*>;
	redo = new stack<Game*>;
	rank = new linkedList<Player*>;

	ifstream f("data.bin", ios::binary);
	if (f.is_open()) {
		f.read((char*)&state, sizeof(char));
		f.read((char*)&size, sizeof(int));
		f.read((char*)&bestScore, sizeof(int));
		f.read((char*)&playerName, sizeof(playerName));

		int size;
		f.read((char*)&size, sizeof(int));
		char name[29];
		int score;
		for (int i = 0; i < size; i++) {
			f.read((char*)&name, sizeof(name));
			f.read((char*)&score, sizeof(int));
			rank->addTail(new Player(name, score));
		}
		f.close();
	}
}

// Destructor of struct Game
Game::~Game() {
	if (arr) {
		for (int i = 0; i < size; i++) {
			if (arr[i] != NULL) {
				delete[] arr[i];
			}
		}
		delete[] arr;
	}
	delete undo;
	delete redo;
	delete rank;
}

// Initialise the array
void Game::init() {
	arr = new int* [size];
	for (int i = 0; i < size; i++) {
		// [How do you initialise a dynamic array in C++?](https://stackoverflow.com/a/2030018)
		arr[i] = new int[size](); // Use () after int[size] to initalize each element to 0
	}
	state = 'p';
	spawnNum();
	spawnNum();
}

// Move the cursor to a specific position
void Game::gotoxy(int x, int y) {
	//[How can I print a string to the console at specific coordinates in C++?](https://stackoverflow.com/a/1670911)
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Game::drawFrame() {
	int width = 1024.0 * (size * 8 + 1 + 3) / 112, height = 576.0 * (size * 4 + 4 + 2) / 29;
	MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, TRUE);

	system("cls");

	cout << right << "╔";
	for (int i = 0; i < size - 1; i++) {
		cout << "════════";
	}
	cout << "═══════╗\n";
	
	cout << "║ Best:" << setw(8 * size - 3) << "║\n";
	cout << "║ Score:" << setw(8 * size - 4) << "║\n";

	cout << "╠";
	for (int i = 0; i < size - 1; i++) {
		cout << "═══════╦";
	}
	cout << "═══════╣\n";

	for (int i = 0; i < size; i++) {
		for (int i = 0; i < 3; i++) {
			cout << "║";
			for (int i = 0; i < size; i++) {
				cout << setw(10) << "║";
			}
			cout << "\n";
		}

		if (i < size - 1) {
			cout << "╠";
			for (int i = 0; i < size - 1; i++) {
				cout << "═══════╬";
			}
			cout << "═══════╣\n";
		}
	}

	cout << "╚";
	for (int i = 0; i < size - 1; i++) {
		cout << "═══════╩";
	}
	cout << "═══════╝\n";
}

// Draw the home menu
void Game::drawHome() {
	MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
	
	system("cls");
	cout << "\n\n";
	cout << setw(64) << right << "2============0\n";
	cout << setw(64) << right << "| WELCOME TO |\n";
	cout << setw(66) << right << "4============8\n\n\n";
	
	cout << setw(88) << right << " _________         _________                 __        _________\n";
	cout << setw(89) << right << "/222222222\\       /000000000\\               /4/       /888888888\\\n";
	cout << setw(89) << right << "|2|     /2/       |0|     |0|              /4/        |8|     |8|\n";
	cout << setw(89) << right << "       /2/        |0|     |0|             /4/         |8|     |8|\n";
	cout << setw(89) << right << "      /2/         |0|     |0|            /4/          |8|     |8|\n";
	cout << setw(89) << right << "     /2/          |0|     |0|           /4/           |8|_____|8|\n";
	cout << setw(89) << right << "    /2/           |0|     |0|          /4/            |888888888|\n";
	cout << setw(89) << right << "   /2/            |0|     |0|         /4/  _          |8|     |8|\n";
	cout << setw(89) << right << "  /2/             |0|     |0|        /4/__|4|_        |8|     |8|\n";
	cout << setw(89) << right << " /2/              |0|     |0|       /444444444|       |8|     |8|\n";
	cout << setw(89) << right << "/2/_______        |0|_____|0|             |4|         |8|_____|8|\n";
	cout << setw(91) << right << "|222222222|       \\000000000/             |4|         \\888888888/\n\n\n";

	cout << setw(60) << right << "PLAY\n";
	cout << setw(60) << right << "RANK\n";
	cout << setw(60) << right << "INFO\n";
	cout << setw(60) << right << "QUIT\n";
}

// Show home menu
bool Game::home() {
	drawHome();

	string choices[] = { "PLAY", "RANK", "INFO", "QUIT" };
	int choice = 1, ch;
	bool accepted = 0;
	
	highlightText(55, 20 + choice, choices[choice - 1]);

	while (1) {
		// [Capture characters from standard input without waiting for enter to be pressed](https://stackoverflow.com/a/421871)
		if (!accepted) ch = tolower(_getch());
		accepted = 0;
		switch (ch) {
		case 0: // To ignore some virtual keys
			ch = _getch();
			break;
		case 224: 
			MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
			ch = _getch();
			if (choice < 4 && ch == KEY_DOWN_ARROW) {
				unhighlightText(55, 20 + choice, choices[choice - 1]);
				highlightText(55, 20 + choice + 1, choices[choice]);
				choice++;
			}
			else if (choice > 1 && ch == KEY_UP_ARROW) {
				unhighlightText(55, 20 + choice, choices[choice - 1]);
				highlightText(55, 20 + choice - 1, choices[choice - 2]);
				choice--;
			}
			break;
		case 32: case 13:
			ch = tolower(choices[choice - 1][0]);
			accepted = 1;
			break;
		case 'p':
			getPlayerName();
			if (state != 'n' && state != 'l') { // state == win || state == playing && can open date file
				system("cls");
				gotoxy(27, 11);
				cout << "Do you want to continue the before game or play a new game?";
				gotoxy(40, 13);
				cout << "[c] CONTINUE	    [n] NEW GAME";
				while (1) {
					ch = tolower(_getch());
					if (ch == 'c') { // continue
						system("cls");
						gotoxy(53, 11);
						cout << "LOADING";
						for (int i = 0; i < 3; i++) {
							cout << ".";
							Sleep(250);
						}
						loadData();
						return 1;
					}
					else if (ch == 'n') { // new game
						state = 'n';
						break;
					}
				}
			}
			// state == lose || cannot open data file && play new game
			system("cls");
			gotoxy(33, 11);
			cout << "Which difficulty do you want to play? [2 -> 9]";
			while (1) {
				ch = tolower(_getch());
				if ('2' <= ch && ch <= '9') break;
			}
			size = ch - '0';
			return 1;
		case 'r': 
			showRank();
			drawHome();
			highlightText(55, 20 + choice, choices[choice - 1]);
			break;
		case 'i':
			showInfo();
			drawHome();
			highlightText(55, 20 + choice, choices[choice - 1]);
			break;
		case 'q': case KEY_ESCAPE: return 0;
		}
	}
}

// Check if the user can move one step further
bool Game::canMove() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((i < size - 1 && arr[i][j] == arr[i + 1][j]) || (j < size - 1 && arr[i][j] == arr[i][j + 1])) return 1;
		}
	}
	return 0;
}

// Show the updated array and scores
void Game::updateState() {
	int width = 1024.0 * (size * 8 + 1 + 3) / 112, height = 576.0 * (size * 4 + 4 + 2) / 29;
	MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, TRUE);
	
	// [Colorizing text and console background in C++](https://www.geeksforgeeks.org/colorizing-text-and-console-background-in-c)
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);

	gotoxy(8, 1);
	cout << bestScore;
	gotoxy(9, 2);
	cout << score;
	
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			gotoxy(1 + 8 * j, 5 + 4 * i);
			cout << "       ";
			gotoxy(1 + 8 * j, 5 + 4 * i);
			if (arr[i][j] != 0) {
				SetConsoleTextAttribute(console_color, int(log2(arr[i][j])) % 15 + 1);
				int space = 4 + (int(log10(arr[i][j])) + 1) / 2;
				cout << setw(space) << right << arr[i][j];
				SetConsoleTextAttribute(console_color, 15); // Reset the background color
			}
		}
	}
}

// Randomly spawn a random number (only 2 or 4)
void Game::spawnNum() {
	int i, j;
	do {
		i = rand() % size;
		j = rand() % size;
	} while (arr[i][j] != 0);
	arr[i][j] = (rand() % 2 + 1) * 2;
	cnt++;
}

// Merge from up to down
bool Game::upToDown() {
	int mark, changed = 0;
	for (int j = 0; j < size; j++) {
		mark = size - 1;
		for (int i = size - 2; i >= 0; i--) {
			if (arr[i][j] != 0) {
				if (arr[mark][j] == 0) {
					arr[mark][j] = arr[i][j];
					arr[i][j] = 0;
					changed = 1;
				}
				else if (arr[mark][j] == arr[i][j]) {
					if (arr[i][j] == 1024) { // This means that the player will reach 2048 in this step
						state = 'w';
					}
					score += arr[i][j] * 2;
					arr[mark--][j] *= 2;
					arr[i][j] = 0;
					changed = 1;
					cnt--;
				}
				else { // arr[i][mark] != arr[i][j]
					arr[--mark][j] = arr[i][j];
					if (mark > i) {
						arr[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed;
}

// Merge from down to up
bool Game::downToUp() {
	int mark, changed = 0;
	for (int j = 0; j < size; j++) {
		mark = 0;
		for (int i = 1; i < size; i++) {
			if (arr[i][j] != 0) {
				if (arr[mark][j] == 0) {
					arr[mark][j] = arr[i][j];
					arr[i][j] = 0;
					changed = 1;
				}
				else if (arr[mark][j] == arr[i][j]) {
					if (arr[i][j] == 1024) { // This means that the player will reach 2048 in this step
						state = 'w';
					}
					score += arr[i][j] * 2;
					arr[mark++][j] *= 2;
					arr[i][j] = 0;
					changed = 1;
					cnt--;
				}
				else { // arr[i][mark] != arr[i][j]
					arr[++mark][j] = arr[i][j];
					if (mark < i) {
						arr[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed;
}

// Merge from left to right
bool Game::leftToRight() {
	int mark, changed = 0;
	for (int i = 0; i < size; i++) {
		mark = size - 1;
		for (int j = size - 2; j >= 0; j--) {
			if (arr[i][j] != 0) {
				if (arr[i][mark] == 0) {
					arr[i][mark] = arr[i][j];
					arr[i][j] = 0;
					changed = 1;
				}
				else if (arr[i][mark] == arr[i][j]) {
					if (arr[i][j] == 1024) { // This means that the player will reach 2048 in this step
						state = 'w';
					}
					score += arr[i][j] * 2;
					arr[i][mark--] *= 2;
					arr[i][j] = 0;
					changed = 1;
					cnt--;
				}
				else { // arr[i][mark] != arr[i][j]
					arr[i][--mark] = arr[i][j];
					if (mark > j) {
						arr[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed;
}

// Merge from right to left
bool Game::rightToLeft() {
	int mark, changed = 0;
	for (int i = 0; i < size; i++) {
		mark = 0;
		for (int j = 1; j < size; j++) {
			if (arr[i][j] != 0) {
				if (arr[i][mark] == 0) {
					arr[i][mark] = arr[i][j];
					arr[i][j] = 0;
					changed = 1;
				}
				else if (arr[i][mark] == arr[i][j]) {
					if (arr[i][j] == 1024) { // This means that the player will reach 2048 in this step
						state = 'w';
					}
					score += arr[i][j] * 2;
					arr[i][mark++] *= 2;
					arr[i][j] = 0;
					changed = 1;
					cnt--;
				}
				else { // arr[i][mark] != arr[i][j]
					arr[i][++mark] = arr[i][j];
					if (mark < j) {
						arr[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed;
}

// Process player input events
int Game::processEvents() {
	updateState();

	int ch;
	bool changed;
	Game* backup;
	while (1) {
		changed = 0;
		ch = tolower(_getch());
		if (ch == 0 || ch == 224) { // Check if the input key is one of virtual keys
			ch = _getch();
			// Ignore if not an arrow key
			if (ch != KEY_DOWN_ARROW && ch != KEY_UP_ARROW && ch != KEY_LEFT_ARROW && ch != KEY_RIGHT_ARROW) continue;
		}
		backup = save();
		
		switch (ch) {
		case 'w': case KEY_UP_ARROW:
			changed = downToUp();
			break;
		case 'a': case KEY_LEFT_ARROW:
			changed = rightToLeft();
			break;
		case 'd': case KEY_RIGHT_ARROW:
			changed = leftToRight();
			break;
		case 's': case KEY_DOWN_ARROW:
			changed = upToDown();
			break;
		case 'z': // Undo
			undoIml(backup);
			break;
		case 'y': // Redo
			redoIml(backup);
			break;
		case 'n': // New game
			delete backup;
			return 1;
		case 'h': // Return home
			delete backup;
			return 0;
		case 'q': case KEY_ESCAPE: // Quit game
			delete backup;
			return -1;
		default:
			delete backup;
			continue;
		}
		if (changed) {
			bestScore = max(bestScore, score);
			spawnNum();
			updateState();
			undo->push(backup);
			redo->clear();
			
			if (cnt == size * size && !canMove()) return lose();

			if (state == 'w') {
				int ret = win();
				if (ret != 2) return ret;
			}
		}
		else { // not changed
			if (backup != redo->top() && backup != undo->top()) {
				delete backup;
			}
		}
	}
}

int Game::lose() {
	state = 'l';
	
	system("cls");
	MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
	
	gotoxy((112 - 16) / 2 - 2, 30 / 3);
	cout << "╔";
	gotoxy((112 - 16) / 2 + 16 + 1, 30 / 3);
	cout << "╗";
	for (int i = 0; i < 5; i++) {
		gotoxy((112 - 16) / 2 - 2, 30 / 3 + i + 1);
		cout << "║";
		gotoxy((112 - 16) / 2 + 16 + 1, 30 / 3 + i + 1);
		cout << "║";
	}
	gotoxy((112 - 16) / 2 - 2, 30 / 3 + 6);
	cout << "╚"; 
	for (int i = 0; i < 18; i++) cout << "═";
	cout << "╝";
	
	gotoxy((112 - 16) / 2, 30 / 3);
	cout << "UNABLE TO MERGE!";
	gotoxy((112 - 8) / 2, 30 / 3 + 2);
	cout << "NEW GAME";
	gotoxy((112 - 4) / 2, 30 / 3 + 3);
	cout << "HOME";
	gotoxy((112 - 4) / 2, 30 / 3 + 4);
	cout << "QUIT";

	string choices[] = { "NEW GAME", "HOME", "QUIT" };
	int choice = 1, ch;
	bool accepted = 0;

	highlightText((112 - choices[choice - 1].length()) / 2, 30 / 3 + 1 + choice, choices[choice - 1]);

	while (1) {
		if (!accepted) ch = tolower(_getch());
		accepted = 0;
		switch (ch) {
		case 0: // To ignore some virtual keys
			ch = _getch();
			break;
		case 224:
			MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
			ch = _getch();
			if (choice < 3 && ch == KEY_DOWN_ARROW) {
				unhighlightText((112 - choices[choice - 1].length()) / 2, 30 / 3 + 1 + choice, choices[choice - 1]);
				highlightText((112 - choices[choice].length()) / 2, 30 / 3 + 1 + choice + 1, choices[choice]);
				choice++;
			}
			else if (choice > 1 && ch == KEY_UP_ARROW) {
				unhighlightText((112 - choices[choice - 1].length()) / 2, 30 / 3 + 1 + choice, choices[choice - 1]);
				highlightText((112 - choices[choice - 2].length()) / 2, 30 / 3 + 1 + choice - 1, choices[choice - 2]);
				choice--;
			}
			break;
		case 32: case 13:
			ch = tolower(choices[choice - 1][0]);
			accepted = 1;
			break;
		case 'n': return 1;
		case 'h': return 0;
		case 'q': case KEY_ESCAPE: return -1;
		}
	}
}

int Game::win() {
	state = 'p';

	system("cls");
	MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
	
	gotoxy((112 - 33) / 2 - 2, 30 / 3);
	cout << "╔";
	gotoxy((112 - 33) / 2 + 33 + 1, 30 / 3);
	cout << "╗";
	for (int i = 0; i < 6; i++) {
		gotoxy((112 - 33) / 2 - 2, 30 / 3 + i + 1);
		cout << "║";
		gotoxy((112 - 33) / 2 + 33 + 1, 30 / 3 + i + 1);
		cout << "║";
	}
	gotoxy((112 - 33) / 2 - 2, 30 / 3 + 7);
	cout << "╚";
	for (int i = 0; i < 35; i++) cout << "═";
	cout << "╝";

	gotoxy((112 - 33) / 2, 30 / 3);
	cout << "Congratulations on reaching 2048!";
	gotoxy((112 - 8) / 2, 30 / 3 + 2);
	cout << "CONTINUE";
	gotoxy((112 - 8) / 2, 30 / 3 + 3);
	cout << "NEW GAME";
	gotoxy((112 - 4) / 2, 30 / 3 + 4);
	cout << "HOME";
	gotoxy((112 - 4) / 2, 30 / 3 + 5);
	cout << "QUIT";
	
	string choices[] = { "CONTINUE", "NEW GAME", "HOME", "QUIT" };
	int choice = 1, ch, width, height;
	bool accepted = 0;

	highlightText((112 - choices[choice - 1].length()) / 2, 30 / 3 + 1 + choice, choices[choice - 1]);

	while (1) {
		if (!accepted) ch = tolower(_getch());
		accepted = 0;
		switch (ch) {
		case 0: // To ignore some virtual keys
			ch = _getch();
			break;
		case 224:
			MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
			ch = _getch();
			if (choice < 4 && ch == KEY_DOWN_ARROW) {
				unhighlightText((112 - choices[choice - 1].length()) / 2, 30 / 3 + 1 + choice, choices[choice - 1]);
				highlightText((112 - choices[choice].length()) / 2, 30 / 3 + 1 + choice + 1, choices[choice]);
				choice++;
			}
			else if (choice > 1 && ch == KEY_UP_ARROW) {
				unhighlightText((112 - choices[choice - 1].length()) / 2, 30 / 3 + 1 + choice, choices[choice - 1]);
				highlightText((112 - choices[choice - 2].length()) / 2, 30 / 3 + 1 + choice - 1, choices[choice - 2]);
				choice--;
			}
			break;
		case 32: case 13:
			ch = tolower(choices[choice - 1][0]);
			accepted = 1;
			break;
		case 'c':
			width = 1024.0 * (size * 8 + 1 + 3) / 112; height = 576.0 * (size * 4 + 4 + 2) / 29;
			MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, TRUE);
			drawFrame();
			updateState();
			return 2;
		case 'n': return 1;
		case 'h': return 0;
		case 'q': case KEY_ESCAPE: return -1;
		}
	}
}

// Create a backup version of the current state
Game* Game::save() {
	Game* game = new Game;
	game->size = size;
	game->arr = new int* [size];
	for (int i = 0; i < size; i++) {
		game->arr[i] = new int[size];
		for (int j = 0; j < size; j++) {
			game->arr[i][j] = arr[i][j];
		}
	}
	game->cnt = cnt;
	game->score = score;
	game->bestScore = bestScore;
	return game;
}

// Load the state from a backup version
void Game::load(Game* game) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			arr[i][j] = game->arr[i][j];
		}
	}
	cnt = game->cnt;
	score = game->score;
	bestScore = game->bestScore;
	delete game;
}

void Game::undoIml(Game* game) {
	if (!undo->empty()) {
		redo->push(game);
		load(undo->pop_r());
		updateState();
	}
	else {
		system("cls");
		gotoxy((size * 8 + 2 - 15) / 2, (size * 4 + 4) * 0.45);
		cout << "Unable to undo!";
		Sleep(500);
		system("cls");
		drawFrame();
		updateState();
	}
}

void Game::redoIml(Game* game) {
	if (!redo->empty()) {
		undo->push(game);
		load(redo->pop_r());
		updateState();
	}
	else {
		system("cls");
		gotoxy((size * 8 + 2 - 15) / 2, (size * 4 + 4) * 0.45);
		cout << "Unable to redo!";
		Sleep(500);
		system("cls");
		drawFrame();
		updateState();
	}
}

// Write all important data to a binary file
void Game::saveData() {
	// Reset file attributes to write data
	int attr = GetFileAttributesW(L"data.bin");
	if (attr & FILE_ATTRIBUTE_HIDDEN) {
		SetFileAttributesW(L"data.bin", attr & ~FILE_ATTRIBUTE_HIDDEN);
	}
	
	ofstream f("data.bin", ios::binary);
	
	// Writing the current state to file
	f.write((char*)&state, sizeof(char));
	f.write((char*)&size, sizeof(int));
	f.write((char*)&bestScore, sizeof(int));
	f.write((char*)&playerName, sizeof(playerName));

	f.write((char*)&rank->size, sizeof(int));
	node<Player*>* tmp = rank->head;
	while (tmp) {
		f.write((char*)&tmp->data->name, sizeof(tmp->data->name));
		f.write((char*)&tmp->data->score, sizeof(int));
		tmp = tmp->next;
	}
	
	if (state != 'l') {
		f.write((char*)&cnt, sizeof(int));
		f.write((char*)&score, sizeof(int));
		
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				f.write((char*)&arr[i][j], sizeof(int));
			}
		}
	
		// Writing the all states in the undo stack to file
		stack<Game*>* st = new stack<Game*>;
		int cnt = 0;
		while (!undo->empty()) {
			st->push(undo->pop_r());
			cnt++;
		}

		f.write((char*)&cnt, sizeof(int));
	
		Game* game;
		while (!st->empty()) {
			game = st->pop_r();
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					f.write((char*)&game->arr[i][j], sizeof(int));
				}
			}
			f.write((char*)&game->cnt, sizeof(int));
			f.write((char*)&game->score, sizeof(int));
			f.write((char*)&game->bestScore, sizeof(int));
			delete game;
		}

		// Writing the all states in the redo stack to file
		cnt = 0;
		while (!redo->empty()) {
			st->push(redo->pop_r());
			cnt++;
		}

		f.write((char*)&cnt, sizeof(int));

		while (!st->empty()) {
			game = st->pop_r();
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					f.write((char*)&game->arr[i][j], sizeof(int));
				}
			}
			f.write((char*)&game->cnt, sizeof(int));
			f.write((char*)&game->score, sizeof(int));
			f.write((char*)&game->bestScore, sizeof(int));
			delete game;
		}
		delete st;
	}
	f.close();

	// Set file attributes to prevent user from modifying the data file
	// [Is it possible to create a hidden txt file in C++?](https://stackoverflow.com/a/33175029)
	attr = GetFileAttributes(L"data.bin");
	if (!(attr & FILE_ATTRIBUTE_HIDDEN)) {
		SetFileAttributesW(L"data.bin", attr | FILE_ATTRIBUTE_HIDDEN);
	}
}

// Read all data from a binary file
void Game::loadData() {
	ifstream f("data.bin", ios::binary);
	if (!f.is_open()) return;

	// Skip read data
	f.seekg(sizeof(state) + sizeof(size) + sizeof(bestScore) + sizeof(cnt) + sizeof(playerName) + (sizeof(playerName) + sizeof(score)) * rank->size);
	
	if (state != 'l') {
		// Reading the current state from file
		f.read((char*)&this->cnt, sizeof(int));
		f.read((char*)&this->score, sizeof(int));
		
		arr = new int* [size];
		for (int i = 0; i < size; i++) {
			arr[i] = new int[size];
			for (int j = 0; j < size; j++) {
				f.read((char*)&arr[i][j], sizeof(int));
			}
		}

		// Reading the all states to the undo stack from file
		int cnt;
		f.read((char*)&cnt, sizeof(int));

		Game* game;
		for (int i = 0; i < cnt; i++) {
			game = new Game;
			game->arr = new int* [size];
			for (int i = 0; i < size; i++) {
				game->arr[i] = new int[size];
				for (int j = 0; j < size; j++) {
					f.read((char*)&game->arr[i][j], sizeof(int));
				}
			}
			f.read((char*)&game->cnt, sizeof(int));
			f.read((char*)&game->score, sizeof(int));
			f.read((char*)&game->bestScore, sizeof(int));
			undo->push(game);
		}

		// Reading the all states to the redo stack from file
		f.read((char*)&cnt, sizeof(int));

		for (int i = 0; i < cnt; i++) {
			game = new Game;
			game->arr = new int* [size];
			for (int i = 0; i < size; i++) {
				game->arr[i] = new int[size];
				for (int j = 0; j < size; j++) {
					f.read((char*)&game->arr[i][j], sizeof(int));
				}
			}
			f.read((char*)&game->cnt, sizeof(int));
			f.read((char*)&game->score, sizeof(int));
			f.read((char*)&game->bestScore, sizeof(int));
			redo->push(game);
		}
	}
	f.close();
}

// Get and check if the player name is already exist
void Game::getPlayerName() {
	while (1) {
		system("cls");
		
		gotoxy(40, 10);
		cout << "╔═ ENTER YOUR NAME ════════════╗\n";
		gotoxy(40, 11);
		cout << "║                              ║\n";
		gotoxy(40, 12);
		cout << "╚══════════════════════════════╝\n";

		showCursor(1);
		playerName[0] = '\0';
		gotoxy(42, 11);
		int ch, len;
		while (1) {
			MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, (GetSystemMetrics(SM_CYSCREEN) - 576) / 2, 1024, 576, TRUE);
			
			ch = _getch();
			len = strlen(playerName);
			if (ch == 0 || ch == 224) { // To ignore some virtual keys
				ch = _getch(); // To clear the buffer
			}
			else if (32 <= ch && ch <= 126 && len < 28) {
				cout << (char)ch;
				playerName[len] = ch;
				playerName[len + 1] = '\0';
			}
			else if (ch == '\b' && len) {
				playerName[len - 1] = '\0';
				len--;
				gotoxy(42 + len, 11);
				cout << " ";
				gotoxy(42 + len, 11);
			}
			else if (ch == 13 && len) {
				break;
			}
		}
		showCursor(0);

		node<Player*>* tmp = rank->head;
		while (tmp && strcmp(tmp->data->name, playerName)) {
			tmp = tmp->next;
		}
		if (tmp) {
			system("cls");
			gotoxy(30, 11);
			cout << "This name is already exits. Please use another name!";
			Sleep(1000);
		}
		else break;
	}
}

// Update the current player to the rank
void Game::updateRank() {
	if (score == 0) return;
	while(1) {
		int pos = 0;
		node<Player*>* tmp = rank->head;
		while (tmp && tmp->data->score >= score && strcmp(tmp->data->name, playerName)) {
			tmp = tmp->next;
			pos++;
		}
		if (tmp && !strcmp(tmp->data->name, playerName)) {
			if (score > tmp->data->score) {
				rank->delNode(pos);
			}
			else return;
		}
		else {
			rank->addNode(pos, new Player(playerName, score));
			break;
		}
	}
	if (rank->size > 10) rank->delTail();
}

// Show rank
void Game::showRank() {
	system("cls");
	cout << "\n\n\n";
	cout << setw(64) << right << "~ RANK ~\n\n";
	cout << setw(184) << "╔═════╦══════════════════════════════╦═══════════╗\n";
	cout << setw(92)  << "║ Top ║             Name             ║   Score   ║\n";
	cout << setw(184) << "╠═════╬══════════════════════════════╬═══════════╣\n";

	int cnt = 0;
	node<Player*>* tmp = rank->head;
	while (tmp) {
		cout << setw(36) << "║" << setw(3) << ++cnt << setw(6) << "║ " << left << setw(29) << tmp->data->name
			<< "║" << right << setw(10) << tmp->data->score << " ║\n";
		tmp = tmp->next;
	}

	cout << setw(186) << "╚═════╩══════════════════════════════╩═══════════╝\n\n\n";
	for (int i = 0; i < 10 - rank->size; i++) cout << "\n";
	cout << setw(65) << "[c] CLOSE\n\n\n";
	
	int ch;
	while (1) {
		ch = tolower(_getch());
		if (ch == 0 || ch == 224) { // To ignore some virtual keys
			ch = _getch(); // To clear the buffer
		}
		else if (ch == 'c' || ch == KEY_ESCAPE) break;
	}
}

void Game::showInfo() {
	system("cls");
	cout << right << "\n\n\n\n\n";
	cout << setw(130) << "╔════════════ INSTRUCTIONS ════════════╗\n";
	cout << setw(82) << "║                                      ║\n";
	cout << setw(82) << "║   Move up:        W or UP ARROW      ║\n";
	cout << setw(82) << "║   Move down:      S or DOWN ARROW    ║\n";
	cout << setw(82) << "║   Move to left:   A or LEFT ARROW    ║\n";
	cout << setw(82) << "║   Move to right:  D or RIGHT ARROW   ║\n";
	cout << setw(82) << "║   Undo / Redo:    Z / Y              ║\n";
	cout << setw(82) << "║   Play new game:  N                  ║\n";
	cout << setw(82) << "║   Go home:        H                  ║\n";
	cout << setw(82) << "║   Quit game:      Q or ESCAPE        ║\n";
	cout << setw(82) << "║                                      ║\n";
	cout << setw(162) << "╚══════════════════════════════════════╝\n\n\n\n\n";
	cout << setw(65) << "[c] CLOSE\n\n\n";

	int ch;
	while (1) {
		ch = tolower(_getch());
		if (ch == 0 || ch == 224) { // To ignore some virtual keys
			ch = _getch(); // To clear the buffer
		}
		else if (ch == 'c' || ch == KEY_ESCAPE) break;
	}
}

void Game::showCursor(bool option) {
	// [How to hide console cursor in c?](https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c)
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(handle, &info);
	info.bVisible = option;
	SetConsoleCursorInfo(handle, &info);
}

void Game::highlightText(int x, int y, string text) {
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(x, y);
	SetConsoleTextAttribute(console_color, 240);
	cout << text;
	SetConsoleTextAttribute(console_color, 15); // Reset the background color
}

void Game::unhighlightText(int x, int y, string text) {
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(x, y);
	SetConsoleTextAttribute(console_color, 15);
	cout << text;
}
