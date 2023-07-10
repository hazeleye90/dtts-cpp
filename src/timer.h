#pragma once
#include <cstdint>

class Timer
{
public:
    Timer();
    void start();
    void stop();
    void pause();
    void unpause();
    uint32_t getTicks();
    bool isStarted() const { return mStarted; }
    bool isPaused() const { return mPaused && mStarted; }

private:
    uint32_t mStartTicks;
    uint32_t mPauseTicks;
    bool mStarted;
    bool mPaused;
};