#include "timer.h"
#include <SDL.h>

Timer::Timer() : 
	mStartTicks(0), 
	mPauseTicks(0), 
	mStarted(false), 
	mPaused(false) 
{
}

void Timer::start()
{
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks();
    mPauseTicks = 0;
}

void Timer::stop()
{
    mStarted = false;
    mPaused = false;
    mStartTicks = 0;
    mPauseTicks = 0;
}

void Timer::pause() 
{
    if (mStarted && !mPaused)
    {
        mPaused = true;
        mPauseTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void Timer::unpause() 
{
    if (mStarted && mPaused)
    {
        mPaused = false;
        mStartTicks = SDL_GetTicks() - mPauseTicks;
        mPauseTicks = 0;
    }
}

uint32_t Timer::getTicks() 
{ 
    if (mStarted)
    {
        return mPaused ? mPauseTicks : (SDL_GetTicks() - mStartTicks);
    }
    return 0;
}
