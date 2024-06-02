#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

void strip(char* str);
void encrypt(char str[]);
bool any(char** arr, int n, bool val = 1);
void gotoxy(int x, int y = -1);
void showCursor(bool option);
void highlightText(int x, int y, string text);
void unhighlightText(int x, int y, string text);
void resizeConsoleWindow(int width, int height);
string wfill(string s, int len);
string formatTime(time_t secs);

#endif