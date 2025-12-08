#pragma once
#include <chrono>

class GameTimer {
public:
    GameTimer();

    void Start();
    void Stop();
    void Reset();
    int GetElapsedSeconds() const;

private:
    std::chrono::steady_clock::time_point startTime;
    bool running;
    int accumulatedSeconds;
};