#include "../include/TimeTracker.h"
#include <sstream>
#include <iomanip>

// Constructor
TimeTracker::TimeTracker() : startTime(0), totalTime(0), isRunning(false), currentTask(nullptr) {}

// Timer control
void TimeTracker::startTimer(Task* task) {
    if (!isRunning) {
        startTime = std::time(nullptr);
        isRunning = true;
        currentTask = task;
    }
}

void TimeTracker::pauseTimer() {
    if (isRunning) {
        std::time_t currentTime = std::time(nullptr);
        totalTime += (currentTime - startTime);
        isRunning = false;
        
        // Update task time spent
        if (currentTask != nullptr) {
            currentTask->addTimeSpent(currentTime - startTime);
        }
    }
}

void TimeTracker::stopTimer() {
    if (isRunning) {
        pauseTimer();
    }
    
    // Reset timer
    startTime = 0;
    totalTime = 0;
    currentTask = nullptr;
}

void TimeTracker::resumeTimer() {
    if (!isRunning && currentTask != nullptr) {
        startTime = std::time(nullptr);
        isRunning = true;
    }
}

// Getters
std::time_t TimeTracker::getElapsedTime() const {
    if (isRunning) {
        std::time_t currentTime = std::time(nullptr);
        return totalTime + (currentTime - startTime);
    }
    return totalTime;
}

std::time_t TimeTracker::getTotalTime() const {
    return totalTime;
}

bool TimeTracker::getIsRunning() const {
    return isRunning;
}

Task* TimeTracker::getCurrentTask() const {
    return currentTask;
}

// Utility methods
std::string TimeTracker::getElapsedTimeString() const {
    std::time_t elapsed = getElapsedTime();
    int hours = static_cast<int>(elapsed / 3600);
    int minutes = static_cast<int>((elapsed % 3600) / 60);
    int seconds = static_cast<int>(elapsed % 60);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

std::string TimeTracker::getTotalTimeString() const {
    int hours = static_cast<int>(totalTime / 3600);
    int minutes = static_cast<int>((totalTime % 3600) / 60);
    int seconds = static_cast<int>(totalTime % 60);
    
    std::stringstream ss;
    ss << hours << "h " << minutes << "m " << seconds << "s";
    return ss.str();
}