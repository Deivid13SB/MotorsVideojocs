#pragma once

#include <chrono>


class Time
{
public:
    Time();
    ~Time() = default;

 
    void Tick();

    
    float DeltaTime() const { return deltaTime; }

    
    float FPS() const { return fps; }

    double Milliseconds() const;

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint startTime;
    TimePoint lastFrame;

    float deltaTime = 0.0f; 
    float fps = 0.0f;
};
