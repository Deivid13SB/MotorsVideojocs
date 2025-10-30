#include "Time.h"

Time::Time()
{
    startTime = Clock::now();
    lastFrame = startTime;
}

void Time::Tick()
{
    TimePoint now = Clock::now();
    std::chrono::duration<float> frameDuration = now - lastFrame;
    deltaTime = frameDuration.count();

    if (deltaTime > 0.25f)
        deltaTime = 0.25f;

    fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
    lastFrame = now;
}


double Time::Milliseconds() const
{
    auto now = Clock::now();
    std::chrono::duration<double, std::milli> elapsed = now - startTime;
    return elapsed.count();
}
