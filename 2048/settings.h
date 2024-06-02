#ifndef SETTINGS_H
#define SETTINGS_H

#include "2048.h"

class Settings {
	int size;
	bool undoEnable, redoEnable;

public:
	Settings();
	int getSize();
	bool isUndoEnable();
	bool isRedoEnable();
	void setSize(int size);
	void setUndoEnable(bool set);
	void setRedoEnable(bool set);
	void showSettings();
	void showEnableUndoRedo();
	void serialize(fstream& fs, bool bWrite);
};

#endif