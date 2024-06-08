#include "utilities.h"

int find(char** arr, int n, const char* str) {
	for (int i = 0; i < n; i++) {
		if (strcmp(arr[i], str) == 0) return i;
	}
	return -1;
}

void strip(char* str) {
	int l = 0, r = strlen(str) - 1;
	while (l <= r && str[l] == ' ') l++;
	while (l <= r && str[r] == ' ') r--;
	for (int i = 0; i < r - l + 1; i++) {
		str[i] = str[i + l];
	}
	str[r - l + 1] = '\0';
}

void encrypt(char str[]) {
	for (int i = 0; i < strlen(str); i++) {
		str[i] ^= 127;
	}
}

bool any(char** arr, int n, bool val) {
	for (int i = 0; i < n; i++) {
		if (arr[i][0] != '\0') return 1;
	}
	return 0;
}
// Move the cursor to a specific position
void gotoxy(int x, int y) {
	//[How can I print a string to the console at specific coordinates in C++?](https://stackoverflow.com/a/1670911)
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (y == -1) { // Use current y position
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(handle, &csbi);
		y = csbi.dwCursorPosition.Y;
	}
	COORD coord{ x, y };
	SetConsoleCursorPosition(handle, coord);
}

void showCursor(bool option) {
	// [How to hide console cursor in c?](https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c)
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(handle, &info);
	info.bVisible = option;
	SetConsoleCursorInfo(handle, &info);
}

void highlightText(int x, int y, string text) {
	// [Colorizing text and console background in C++](https://www.geeksforgeeks.org/colorizing-text-and-console-background-in-c)
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(x, y);
	SetConsoleTextAttribute(handle, 240);
	cout << text;
	SetConsoleTextAttribute(handle, 15); // Reset the background color
}

void unhighlightText(int x, int y, string text) {
	// [Colorizing text and console background in C++](https://www.geeksforgeeks.org/colorizing-text-and-console-background-in-c)
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(x, y);
	SetConsoleTextAttribute(handle, 15);
	cout << text;
}
// Set the console window size in pixels and align to the center
void resizeConsoleWindow(int width, int height) {
	// [How to change window size in C++?](https://stackoverflow.com/a/26597657)
	MoveWindow(GetConsoleWindow(), (GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, TRUE);
	// Make the console unscrollable
	setConsoleUnscrollable();
}
// Set the console window size in characters and align to the center
void setConsoleWindowSize(int rows, int columns) {
	// Set console window size using virtual terminal sequences
	// Copilot + [Console Virtual Terminal Sequences](https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#window-width)
	cout << "\x1b[8;" << rows << ";" << columns << "t"; // ESC [ 8 ; rows ; columns t
	Sleep(5); // Wait for the console window to redraw and update its layout
	// Get the current console window size in pixels to align to the center
	// [How to get width and height from CreateWindowEx() window? C++](https://stackoverflow.com/a/615555)
	RECT rect;
	GetWindowRect(GetConsoleWindow(), &rect);
	int width = rect.right - rect.left, height = rect.bottom - rect.top;
	resizeConsoleWindow(width, height);
}
// Set the console screen buffer size to match the window size for making the console unscrollable
void setConsoleUnscrollable() {
	// [Set console window size on Windows](https://stackoverflow.com/questions/25912721/set-console-window-size-on-windows)
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	COORD newSize{ csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
	SetConsoleScreenBufferSize(handle, newSize);
}
// Make the console not to be resized and closed
void setConsoleProperties() {
	// Set window title using virtual terminal sequences
	// Copilot + [Console Virtual Terminal Sequences](https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#window-width)
	cout << "\x1b]0;2048 Game\x07";
	// Make the console not to be resized
	// [Letting the mouse pass through Windows C++](https://stackoverflow.com/a/50245502)
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_EXSTYLE, GetWindowLong(consoleWindow, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
	// Make the console not to be closed
	// [How to change console window style at runtime?](https://stackoverflow.com/a/41173938)
	// [Window Styles](https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles)
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_SYSMENU);
}

string wfill(string s, int len) {
	len -= s.length();
	while (len--) {
		s += " ";
	}
	return s;
}

string formatTime(time_t secs) {
	string res;
	int h = secs / 3600,
		m = (secs % 3600) / 60,
		s = (secs % 3600) % 60;
	if (h < 10) res += '0';
	res += to_string(h) + ':';
	if (m < 10) res += '0';
	res += to_string(m) + ':';
	if (s < 10) res += '0';
	res += to_string(s);
	return res;
}
