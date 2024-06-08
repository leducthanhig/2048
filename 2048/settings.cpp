#include "settings.h"

Settings::Settings() {
	size = 4;
	undoEnable = redoEnable = 0;
}

int Settings::getSize() {
	return size;
}

bool Settings::isUndoEnable() {
	return undoEnable;
}

bool Settings::isRedoEnable() {
	return redoEnable;
}

void Settings::setSize(int size) {
	this->size = size;
}

void Settings::setUndoEnable(bool set) {
	undoEnable = set;
}

void Settings::setRedoEnable(bool set) {
	redoEnable = set;
}

void Settings::showSettings() {
	system("cls");
	gotoxy(39, 7);
	cout << "╔════════════ SETTINGS ════════════╗\n";
	gotoxy(39);
	cout << "║                                  ║\n";
	gotoxy(39);
	cout << "║   Size of the board:   <  4  >   ║\n";
	gotoxy(39);
	cout << "║                                  ║\n";
	gotoxy(39);
	cout << "║   Enable undo:         < Off >   ║\n";
	gotoxy(39);
	cout << "║                                  ║\n";
	gotoxy(39);
	cout << "║   Enable undo / redo:  < Off >   ║\n";
	gotoxy(39);
	cout << "║                                  ║\n";
	gotoxy(39);
	cout << "╚══════════════════════════════════╝\n\n\n\n\n";
	gotoxy(39);
	cout << "        DEFAULT         SAVE";

	highlightText(39 + 27, 9, ((size < 10) ? " " + to_string(size) + " " : to_string(size) + " "));
	gotoxy(39 + 27, 11);
	cout << (undoEnable ? "On " : "Off");
	gotoxy(39 + 27, 13);
	cout << ((undoEnable && redoEnable) ? "On " : "Off");
}

void Settings::showEnableUndoRedo() {
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(size * 8 + 1 + 3 + (33 - 19) / 2, 8);
	if (!undoEnable) SetConsoleTextAttribute(console_color, 8);
	cout << "[u] Undo";
	SetConsoleTextAttribute(console_color, 15);
	cout << " / ";
	if (!redoEnable) SetConsoleTextAttribute(console_color, 8);
	cout << "[r] Redo";
	SetConsoleTextAttribute(console_color, 15);
}
//[Reading and writing classes with pointers to binary files in c++](https://stackoverflow.com/a/32833242)
void Settings::serialize(fstream& fs, bool bWrite) {
	if (bWrite) {
		fs.write(reinterpret_cast<char*>(&size), sizeof(size));
		fs.write(reinterpret_cast<char*>(&undoEnable), sizeof(undoEnable));
		fs.write(reinterpret_cast<char*>(&redoEnable), sizeof(redoEnable));
	}
	else {
		fs.read(reinterpret_cast<char*>(&size), sizeof(size));
		fs.read(reinterpret_cast<char*>(&undoEnable), sizeof(undoEnable));
		fs.read(reinterpret_cast<char*>(&redoEnable), sizeof(redoEnable));
	}
}

void Settings::processSettingEvents() {
	int input;
	int choice = 1;
	string text1, text2;
	while (1) {
		input = _getch();
		switch (input) {
		case KEY_VIRTUAL: // To ignore some virtual keys
			input = _getch();
			break;
		case KEY_ARROW:
			input = _getch();
			if (choice < 4 && input == KEY_DOWN_ARROW) {
				switch (choice) {
				case 1:
					text1 = to_string(size) + " ";
					if (size < 10) text1 = " " + text1;
					text2 = (undoEnable ? "On " : "Off");
					break;
				case 2:
					text1 = (undoEnable ? "On " : "Off");
					text2 = ((undoEnable && redoEnable) ? "On " : "Off");
					break;
				case 3:
					text1 = ((undoEnable && redoEnable) ? "On " : "Off");
					text2 = "SAVE";
					break;
				}
				unhighlightText(39 + 27, 7 + choice * 2, text1);
				choice++;
				if (choice < 4) {
					highlightText(39 + 27, 7 + choice * 2, text2);
				}
				else {
					highlightText(39 + 8 * 3, 7 + 4 * 2 + 5, text2);
				}
			}
			else if (choice > 1 && input == KEY_UP_ARROW) {
				switch (choice) {
				case 2:
					text1 = (undoEnable ? "On " : "Off");
					text2 = to_string(size) + " ";
					if (size < 10) text2 = " " + text2;
					break;
				case 3:
					text1 = ((undoEnable && redoEnable) ? "On " : "Off");
					text2 = (undoEnable ? "On " : "Off");
					break;
				case 4: case 5:
					text1 = ((choice == 5) ? "DEFAULT" : "SAVE");
					text2 = ((undoEnable && redoEnable) ? "On " : "Off");
					break;
				}
				if (choice > 3) {
					unhighlightText(39 + ((choice == 5) ? 8 : 8 * 3), 7 + 4 * 2 + 5, text1);
				}
				else {
					unhighlightText(39 + 27, 7 + choice * 2, text1);
				}
				if (choice == 5) choice--;
				choice--;
				highlightText(39 + 27, 7 + choice * 2, text2);
			}
			else if (input == KEY_LEFT_ARROW) {
				text2 = "";
				switch (choice) {
				case 1:
					if (size > 4) {
						size--;
						text2 = to_string(size) + " ";
						if (size < 10) text2 = " " + text2;
					}
					break;
				case 2:
					if (undoEnable) {
						undoEnable = 0;
						text2 = (undoEnable ? "On " : "Off");
					}
					break;
				case 3:
					if (undoEnable && redoEnable) {
						redoEnable = 0;
						text2 = ((undoEnable && redoEnable) ? "On " : "Off");
					}
					break;
				case 4:
					text1 = "SAVE"; text2 = "DEFAULT";
					break;
				}
				if (text2 != "") {
					if (choice == 2 && text2 == "Off") {
						redoEnable = 0;
						gotoxy(39 + 27, 7 + 3 * 2);
						cout << "Off";
					}
					if (choice < 4) {
						highlightText(39 + 27, 7 + choice * 2, text2);
					}
					else {
						unhighlightText(39 + ((choice == 5) ? 8 : 8 * 3), 7 + 4 * 2 + 5, text1);
						choice++;
						highlightText(39 + ((choice == 5) ? 8 : 8 * 3), 7 + 4 * 2 + 5, text2);
					}
				}
			}
			else if (input == KEY_RIGHT_ARROW) {
				text2 = "";
				switch (choice) {
				case 1:
					if (size < 10) {
						text1 = to_string(size) + " ";
						if (size < 10) text1 = " " + text1;
						size++;
						text2 = to_string(size) + " ";
						if (size < 10) text2 = " " + text2;
					}
					break;
				case 2:
					if (!undoEnable) {
						text1 = (undoEnable ? "On " : "Off");
						undoEnable = 1;
						text2 = (undoEnable ? "On " : "Off");
					}
					break;
				case 3:
					if (!(undoEnable && redoEnable)) {
						text1 = ((undoEnable && redoEnable) ? "On " : "Off");
						undoEnable = redoEnable = 1;
						text2 = ((undoEnable && redoEnable) ? "On " : "Off");
					}
					break;
				case 5:
					text1 = "DEFAULT"; text2 = "SAVE";
					break;
				}
				if (text2 != "") {
					if (choice == 3 && text2 == "On ") {
						gotoxy(39 + 27, 7 + 2 * 2);
						cout << "On ";
					}
					if (choice < 4) {
						highlightText(39 + 27, 7 + choice * 2, text2);
					}
					else {
						unhighlightText(39 + ((choice == 5) ? 8 : 8 * 3), 7 + 4 * 2 + 5, text1);
						choice--;
						highlightText(39 + ((choice == 5) ? 8 : 8 * 3), 7 + 4 * 2 + 5, text2);
					}
				}
			}
			break;
		case KEY_ENTER: case KEY_SPACEBAR:
			if (choice == 5) {
				size = 4;
				undoEnable = redoEnable = 0;
				gotoxy(39 + 27, 9);
				cout << " " << size << " ";
				gotoxy(39 + 27, 11);
				cout << "Off";
				gotoxy(39 + 27, 13);
				cout << "Off";
			}
			else if (choice == 4) {
				return;
			}
			break;
		}
	}
}
