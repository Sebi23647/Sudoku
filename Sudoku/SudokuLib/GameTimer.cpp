
#include "GameTimer.h"
#include <chrono>

GameTimer::GameTimer() : running(false), accumulatedSeconds(0), startTime(std::chrono::steady_clock::now()) {}

void GameTimer::Start() {
    if (!running) {
        startTime = std::chrono::steady_clock::now();
        running = true;
    }
}

void GameTimer::Stop() {
    if (running) {
        auto now = std::chrono::steady_clock::now();
        accumulatedSeconds += static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count());
        running = false;
    }
}

void GameTimer::Reset() { running = false; accumulatedSeconds = 0; }

int GameTimer::GetElapsedSeconds() const {
    if (!running) return accumulatedSeconds;
    auto now = std::chrono::steady_clock::now();
    int current = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count());
    return accumulatedSeconds + current;
}