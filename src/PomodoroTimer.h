#pragma once
#include <chrono>

class PomodoroTimer {
    int remaining{1500};
    bool running{false};
    std::chrono::steady_clock::time_point lastTick;
public:
    void start() { running = true; lastTick = std::chrono::steady_clock::now(); }
    void stop() { running = false; remaining = 1500; }
    int getRemainingTime() {
        if (running) {
            auto now = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - lastTick).count();
            remaining -= (int)diff;
            lastTick = now;
            if (remaining < 0) remaining = 0;
        }
        return remaining;
    }
};
