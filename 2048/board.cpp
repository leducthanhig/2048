#include "board.h"

Board::Board() {
	data = nullptr;
	size = score = nonZeroCells = 0;
}

Board::Board(int size) {
	data = new int*[size];
	for (int i = 0; i < size; i++) {
		// [How do you initialise a dynamic array in C++?](https://stackoverflow.com/a/2030018)
		data[i] = new int[size](); // Use () after int[size] to initalize each element to 0
	}
	this->size = size;
	score = nonZeroCells = 0;
}

Board::~Board() {
	for (int i = 0; i < size; i++) {
		delete[] data[i];
		data[i] = nullptr;
	}
	delete[] data;
	data = nullptr;
}

void Board::setSize(int size) {
	this->size = size;
}

int Board::getSize() {
	return size;
}

unsigned long long Board::getScore() {
	return score;
}

void Board::copy(Board* board) {
	score = board->score;
	nonZeroCells = board->nonZeroCells;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			this->data[i][j] = board->data[i][j];
		}
	}
}
// Randomly spawn a random number (only 2 or 4)
void Board::spawnNum() {
	int i, j;
	do {
		i = rand() % size;
		j = rand() % size;
	} while (data[i][j] != 0);
	data[i][j] = (rand() % 2 + 1) * 2;
	nonZeroCells++;
}
// Check if the user can move one step further
bool Board::canMove() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((i < size - 1 && data[i][j] == data[i + 1][j]) || (j < size - 1 && data[i][j] == data[i][j + 1])) return 1;
		}
	}
	return 0;
}
// Merge from up to down
int Board::moveUpToDown() {
	int mark, changed = 0;
	bool win = 0;
	for (int j = 0; j < size; j++) {
		mark = size - 1;
		for (int i = size - 2; i >= 0; i--) {
			if (data[i][j] != 0) {
				if (data[mark][j] == 0) {
					data[mark][j] = data[i][j];
					data[i][j] = 0;
					changed = 1;
				}
				else if (data[mark][j] == data[i][j]) {
					if (data[i][j]*2 == WIN_THRESHOLD) { // This means that the player will reach 2048 in this step
						win = 1;
					}
					score += data[i][j] * 2;
					data[mark--][j] *= 2;
					data[i][j] = 0;
					changed = 1;
					nonZeroCells--;
				}
				else { // data[i][mark] != data[i][j]
					data[--mark][j] = data[i][j];
					if (mark > i) {
						data[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed + win;
}
// Merge from down to up
int Board::moveDownToUp() {
	int mark, changed = 0;
	bool win = 0;
	for (int j = 0; j < size; j++) {
		mark = 0;
		for (int i = 1; i < size; i++) {
			if (data[i][j] != 0) {
				if (data[mark][j] == 0) {
					data[mark][j] = data[i][j];
					data[i][j] = 0;
					changed = 1;
				}
				else if (data[mark][j] == data[i][j]) {
					if (data[i][j]*2 == WIN_THRESHOLD) { // This means that the player will reach 2048 in this step
						win = 1;
					}
					score += data[i][j] * 2;
					data[mark++][j] *= 2;
					data[i][j] = 0;
					changed = 1;
					nonZeroCells--;
				}
				else { // data[i][mark] != data[i][j]
					data[++mark][j] = data[i][j];
					if (mark < i) {
						data[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed + win;
}
// Merge from left to right
int Board::moveLeftToRight() {
	int mark, changed = 0;
	bool win = 0;
	for (int i = 0; i < size; i++) {
		mark = size - 1;
		for (int j = size - 2; j >= 0; j--) {
			if (data[i][j] != 0) {
				if (data[i][mark] == 0) {
					data[i][mark] = data[i][j];
					data[i][j] = 0;
					changed = 1;
				}
				else if (data[i][mark] == data[i][j]) {
					if (data[i][j]*2 == WIN_THRESHOLD) { // This means that the player will reach 2048 in this step
						win = 1;
					}
					score += data[i][j] * 2;
					data[i][mark--] *= 2;
					data[i][j] = 0;
					changed = 1;
					nonZeroCells--;
				}
				else { // data[i][mark] != data[i][j]
					data[i][--mark] = data[i][j];
					if (mark > j) {
						data[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed + win;
}
// Merge from right to left
int Board::moveRightToLeft() {
	int mark, changed = 0;
	bool win = 0;
	for (int i = 0; i < size; i++) {
		mark = 0;
		for (int j = 1; j < size; j++) {
			if (data[i][j] != 0) {
				if (data[i][mark] == 0) {
					data[i][mark] = data[i][j];
					data[i][j] = 0;
					changed = 1;
				}
				else if (data[i][mark] == data[i][j]) {
					if (data[i][j]*2 == WIN_THRESHOLD) { // This means that the player will reach 2048 in this step
						win = 1;
					}
					score += data[i][j] * 2;
					data[i][mark++] *= 2;
					data[i][j] = 0;
					changed = 1;
					nonZeroCells--;
				}
				else { // data[i][mark] != data[i][j]
					data[i][++mark] = data[i][j];
					if (mark < j) {
						data[i][j] = 0;
						changed = 1;
					}
				}
			}
		}
	}
	return changed + win;
}

Board* Board::copy() {
	Board* copy = new Board(size);
	copy->score = score;
	copy->nonZeroCells = nonZeroCells;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			copy->data[i][j] = data[i][j];
		}
	}
	return copy;
}

bool Board::isFull() {
	return (nonZeroCells == size * size);
}

void Board::drawFrame() {
	system("cls");
	setConsoleWindowSize(size * 4 + 1, size * 8 + 1 + 3 + 34 + 3);

	cout << right << "╔";
	for (int i = 0; i < size - 1; i++) {
		cout << "═══════╦";
	}
	cout << "═══════╗\n";

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
	cout << "═══════╝";

	gotoxy(size * 8 + 1 + 3 + (33 - 10) / 2, 2);
	cout << "Best Score";
	gotoxy(size * 8 + 1 + 3 + (33 - 5) / 2, 5);
	cout << "Score";
	gotoxy(size * 8 + 1 + 3, 10);
	cout << "Press [p] or ESC to pause the game";
}

void Board::showUpdatedBoard(unsigned long long bestScore) {
	// [Colorizing text and console background in C++](https://www.geeksforgeeks.org/colorizing-text-and-console-background-in-c)
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(size * 8 + 1 + 3 + (33 - to_string(bestScore).length()) / 2, 3);
	cout << bestScore;
	gotoxy(size * 8 + 1 + 3, 6);
	cout << setw(34) << " ";
	gotoxy(size * 8 + 1 + 3 + (33 - to_string(score).length()) / 2);
	cout << score;

	int colorPalette[] = { 31, 47, 79, 95, 159, 175, 207, 16, 32, 64, 80, 144, 160, 176, 192, 27, 43, 75, 171, 203, 219, 165, 181 };
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (data[i][j] != 0) {
				SetConsoleTextAttribute(console_color, colorPalette[int(log2(data[i][j])) - 1]);
				gotoxy(1 + 8 * j, 2 + 4 * i - 1);
				cout << "       ";
				gotoxy(1 + 8 * j, 2 + 4 * i);
				cout << "       ";
				gotoxy(1 + 8 * j, 2 + 4 * i + 1);
				cout << "       ";
				
				int space = 4 + to_string(data[i][j]).length() / 2;
				gotoxy(1 + 8 * j, 2 + 4 * i);
				cout << setw(space) << right << data[i][j];
				SetConsoleTextAttribute(console_color, 15); // Reset the background color
			}
			else {
				gotoxy(1 + 8 * j, 2 + 4 * i - 1);
				cout << "       ";
				gotoxy(1 + 8 * j, 2 + 4 * i);
				cout << "       ";
				gotoxy(1 + 8 * j, 2 + 4 * i + 1);
				cout << "       ";
			}
		}
	}
}
//[Reading and writing classes with pointers to binary files in c++](https://stackoverflow.com/a/32833242)
void Board::serialize(fstream& fs, bool bWrite) {
	if (bWrite) {
		fs.write(reinterpret_cast<char*>(&size), sizeof(size));
		fs.write(reinterpret_cast<char*>(&score), sizeof(score));
		fs.write(reinterpret_cast<char*>(&nonZeroCells), sizeof(nonZeroCells));
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				fs.write(reinterpret_cast<char*>(&data[i][j]), sizeof(data[i][j]));
			}
		}
	}
	else {
		fs.read(reinterpret_cast<char*>(&size), sizeof(size));
		fs.read(reinterpret_cast<char*>(&score), sizeof(score));
		fs.read(reinterpret_cast<char*>(&nonZeroCells), sizeof(nonZeroCells));
		data = new int*[size];
		for (int i = 0; i < size; i++) {
			data[i] = new int[size];
			for (int j = 0; j < size; j++) {
				fs.read(reinterpret_cast<char*>(&data[i][j]), sizeof(data[i][j]));
			}
		}
	}
}