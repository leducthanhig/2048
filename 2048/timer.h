#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer {
	time_t startTime, totalTime;
	bool isPaused, isStopped;

public:
	Timer();
	void start();
	void pause();
	void resume();
	void stop();
	time_t getTotalTime();
};

#endif