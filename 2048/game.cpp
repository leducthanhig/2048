#include "game.h"

Game::Game() {
	// Initalise properties and assign default values
	player = new Player;
	board = nullptr;
	undoStack = new stack<Board>;
	redoStack = new stack<Board>;
	dataSlot = 0;
	// [How do you initialise a dynamic array in C++?](https://stackoverflow.com/a/2030018)
	dataSlots = new char*[DATA_SLOT](); // Use () after char*[DATA_SLOT] to initalize each element to ""
	checkDataFiles();
	top20 = new linkedList<Player>;
	settings = new Settings;
	// If the data file is exist, read setings and the top-20 list
	fstream f("data.bin", ios::binary | ios::in);
	if (f.is_open()) {
		settings->serialize(f, 0);
		top20->serialize(f, 0);
		f.close();
	}
}

Game::~Game() {
	// Reset file attributes to write data
	// [How to hide/un-hide a file without erasing other attributes in C++ on Windows](https://stackoverflow.com/a/10387614)
	int attr = GetFileAttributes(L"data.bin");
	if (attr & FILE_ATTRIBUTE_HIDDEN) {
		SetFileAttributes(L"data.bin", attr & ~FILE_ATTRIBUTE_HIDDEN & ~FILE_ATTRIBUTE_READONLY);
	}
	// Write settings and top-20 list to a binary file
	fstream f("data.bin", ios::binary | ios::out);
	if (f.is_open()) {
		settings->serialize(f, 1);
		top20->serialize(f, 1);
		f.close();
	}
	// Set file attributes to prevent user from modifying the data file
	// [Is it possible to create a hidden txt file in C++?](https://stackoverflow.com/a/33175029)
	attr = GetFileAttributes(L"data.bin");
	if (!(attr & FILE_ATTRIBUTE_HIDDEN)) {
		SetFileAttributes(L"data.bin", attr | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);
	}
	// Free up memory
	delete player;
	delete board;
	delete undoStack;
	delete redoStack;
	delete top20;
	delete settings;
	for (int i = 0; i < DATA_SLOT; i++) {
		delete[] dataSlots[i];
	}
	delete[] dataSlots;
	player = nullptr;
	board = nullptr;
	undoStack = nullptr;
	redoStack = nullptr;
	top20 = nullptr;
	settings = nullptr;
	dataSlots = nullptr;
}
// Write player data to a binary file
void Game::saveData() {
	// [Convert std::string to LPCWSTR in C++](https://www.geeksforgeeks.org/convert-stdstring-to-lpcwstr-in-c/)
	string fileName = "data" + to_string(dataSlot) + ".bin";
	wstring fileName_wstr = wstring(fileName.begin(), fileName.end());
	LPCWSTR fileName_pwc = fileName_wstr.c_str();
	// Reset file attributes to write data
	// [How to hide/un-hide a file without erasing other attributes in C++ on Windows](https://stackoverflow.com/a/10387614)
	int attr = GetFileAttributes(fileName_pwc);
	if (attr & FILE_ATTRIBUTE_HIDDEN) {
		SetFileAttributes(fileName_pwc, attr & ~FILE_ATTRIBUTE_HIDDEN & ~FILE_ATTRIBUTE_READONLY);
	}
	// Write player data to a binary file
	fstream f(fileName, ios::binary | ios::out);
	if (f.is_open()) {
		player->serialize(f, 1);
		board->serialize(f, 1);
		undoStack->serialize(f, 1);
		redoStack->serialize(f, 1);
		f.close();
	}
	// Set file attributes to prevent user from modifying the data file
	// [Is it possible to create a hidden txt file in C++?](https://stackoverflow.com/a/33175029)
	attr = GetFileAttributes(fileName_pwc);
	if (!(attr & FILE_ATTRIBUTE_HIDDEN)) {
		SetFileAttributes(fileName_pwc, attr | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);
	}
}
// Read all data from a binary file
void Game::loadData() {
	fstream f("data" + to_string(dataSlot) + ".bin", ios::binary | ios::in);
	if (f.is_open()) {
		player->serialize(f, 0);
		board->serialize(f, 0);
		undoStack->serialize(f, 0);
		redoStack->serialize(f, 0);
		f.close();
	}
}
// Get the data slot that the player want to use
int Game::getDataSlot(bool load) {
	showDataSlots();

	int input, choice = 1;
	highlightText(40, 10 + choice, " " + to_string(choice) + " - " + wfill((dataSlots[choice - 1][0] != '\0' ? dataSlots[choice - 1] : "Empty"), 29));
	while (1) {
		// [Capture characters from standard input without waiting for enter to be pressed](https://stackoverflow.com/a/421871)
		input = _getch();
		switch (input) {
		case KEY_VIRTUAL: // To ignore some virtual keys
			input = _getch();
			break;
		case KEY_ARROW:
			input = _getch();
			if (choice < DATA_SLOT && input == KEY_DOWN_ARROW) {
				unhighlightText(40, 10 + choice, " " + to_string(choice) + " - " + wfill((dataSlots[choice - 1][0] != '\0' ? dataSlots[choice - 1] : "Empty"), 29));
				highlightText(40, 10 + choice + 1, " " + to_string(choice + 1) + " - " + wfill((dataSlots[choice][0] != '\0' ? dataSlots[choice] : "Empty"), 29));
				choice++;
			}
			else if (choice > 1 && input == KEY_UP_ARROW) {
				unhighlightText(40, 10 + choice, " " + to_string(choice) + " - " + wfill((dataSlots[choice - 1][0] != '\0' ? dataSlots[choice - 1] : "Empty"), 29));
				highlightText(40, 10 + choice - 1, " " + to_string(choice - 1) + " - " + wfill((dataSlots[choice - 2][0] != '\0' ? dataSlots[choice - 2] : "Empty"), 29));
				choice--;
			}
			break;
		case KEY_ENTER: case KEY_SPACEBAR:
			if (!load || dataSlots[choice - 1][0] != '\0') {
				if (confirm("choose this slot")) return choice;
				showDataSlots();
				highlightText(40, 10 + choice, " " + to_string(choice) + " - " + wfill((dataSlots[choice - 1][0] != '\0' ? dataSlots[choice - 1] : "Empty"), 29));
			}
			break;
		case KEY_ESCAPE:
			return -1;
		}
	}
}

void Game::showDataSlots() {
	system("cls");
	gotoxy(39, 8);
	cout << "╔══════════════════════════════════╗\n";
	gotoxy(39);
	cout << "║            DATA SLOTS            ║\n";
	gotoxy(39);
	cout << "╠══════════════════════════════════╣\n";

	for (int i = 0; i < DATA_SLOT; i++) {
		gotoxy(39);
		cout << "║ " << i + 1 << " - " << (dataSlots[i][0] != '\0' ? dataSlots[i] : "Empty");
		gotoxy(39 + 35);
		cout << "║\n";
	}
	gotoxy(39);
	cout << "╚══════════════════════════════════╝";
}
// Check if data files are exist
void Game::checkDataFiles() {
	ifstream f;
	for (int i = 0; i < DATA_SLOT; i++) {
		dataSlots[i] = new char[29]();
		f.open("data" + to_string(i + 1) + ".bin", ios::binary);
		if (f.is_open()) {
			f.read(reinterpret_cast<char*>(dataSlots[i]), 29);
			encrypt(dataSlots[i]);
		}
		f.close();
	}
}
// Update the top-20 list
int Game::updateRank() {
	player->setScore(board->getScore());
	if (player->getScore() == 0) return 0;
	while(1) {
		int pos = top20->find(player, sameName);
		if (pos != -1) {
			if (top20->getNode(pos)->getData()->getScore() >= player->getScore()) return pos + 1;
			else top20->delNode(pos);
		}
		pos = top20->find(player, muchTimeLessScore);
		if (pos == -1) {
			if (top20->getSize() == TOP_LIST_LIMIT) return 0;
			top20->addTail(player->copy());
			return top20->getSize();
		}
		else {
			top20->addNode(pos, player->copy());
			if (top20->getSize() > TOP_LIST_LIMIT) top20->delTail();
			return pos + 1;
		}
	}
}

bool Game::confirm(string msg) {
	msg = "Do you want to " + msg + "?";
	system("cls");
	gotoxy(57 - msg.length() / 2, 11);
	cout << msg << "\n\n";
	highlightText(57 - msg.length() / 4 - 1, 13, "YES");
	unhighlightText(57 + msg.length() / 4 - 1, 13, "NO");

	bool ok = 1;
	int input;
	while (1) {
		input = _getch();
		switch (input) {
		case KEY_VIRTUAL: // To ignore some virtual keys
			input = _getch();
			break;
		case KEY_ARROW:
			input = _getch();
			if (ok && input == KEY_RIGHT_ARROW) {
				unhighlightText(57 - msg.length() / 4 - 1, 13, "YES");
				highlightText(57 + msg.length() / 4 - 1, 13, "NO");
				ok = !ok;
			}
			else if (!ok && input == KEY_LEFT_ARROW) {
				unhighlightText(57 + msg.length() / 4 - 1, 13, "NO");
				highlightText(57 - msg.length() / 4 - 1, 13, "YES");
				ok = !ok;
			}
			break;
		case KEY_ENTER: case KEY_SPACEBAR:
			return ok;
		}
	}
}