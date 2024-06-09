#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

int find(char** arr, int n, const char* str);
bool any(char** arr, int n, bool val = 1);
void strip(char* str);
void encrypt(char str[]);
void gotoxy(int x, int y = -1);
void showCursor(bool option);
void highlightText(int x, int y, string text);
void unhighlightText(int x, int y, string text);
void resizeConsoleWindow(int width, int height);
void setConsoleWindowSize(int rows, int columns);
void setConsoleUnscrollable();
void setConsoleProperties();
void showMessage(string msg, int x, int y, int delay = 0);
string wfill(string s, int len);
string formatTime(time_t secs);

#endif