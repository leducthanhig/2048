#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer {
	time_t startTime, totalTime, addedTime;
	bool isPaused, isStopped;

public:
	Timer();
	void start();
	void pause();
	void resume();
	void stop();
	void addSecs(time_t secs);
	time_t getTotalTime();
};

#endif