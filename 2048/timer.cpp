#include "timer.h"

Timer::Timer() {
    startTime = totalTime = isPaused = isStopped = 0;
}

void Timer::start() {
    startTime = time(0);
}

void Timer::pause() {
    totalTime += time(0) - startTime;
    startTime = time(0);
    isPaused = 1;
}

void Timer::resume() {
    startTime = time(0);
    isPaused = 0;
}

void Timer::stop() {
    totalTime += time(0) - startTime;
    isStopped = 1;
}

time_t Timer::getTotalTime() {
    if (isPaused || isStopped) return totalTime;
    return time(0) - startTime;
}