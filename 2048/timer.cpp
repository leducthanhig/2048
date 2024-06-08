#include "timer.h"

Timer::Timer() {
    startTime = totalTime = addedTime = isPaused = isStopped = 0;
}

void Timer::start() {
    startTime = time(0);
}

void Timer::pause() {
    if (isPaused) return;
    totalTime += time(0) - startTime;
    startTime = time(0);
    isPaused = 1;
}

void Timer::resume() {
    if (!isPaused) return;
    startTime = time(0);
    isPaused = 0;
}

void Timer::stop() {
    if (isStopped) return;
    totalTime += time(0) - startTime;
    isStopped = 1;
}

void Timer::addSecs(time_t secs) {
    addedTime += secs;
}

time_t Timer::getTotalTime() {
    if (isPaused || isStopped) return totalTime + addedTime;
    return time(0) - startTime + addedTime;
}