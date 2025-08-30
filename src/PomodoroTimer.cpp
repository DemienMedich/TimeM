#include "../include/PomodoroTimer.h"
#include <sstream>
#include <iomanip>

// Constructor
PomodoroTimer::PomodoroTimer(int workDuration, int breakDuration, 
                             int longBreakDuration, int sessionsBeforeLongBreak)
    : workDuration(workDuration), breakDuration(breakDuration),
      longBreakDuration(longBreakDuration), sessionsBeforeLongBreak(sessionsBeforeLongBreak),
      completedSessions(0), isWorkSession(true), isRunning(false),
      startTime(0), pausedTime(0), remainingTime(workDuration * 60) {}

// Timer control
void PomodoroTimer::startSession() {
    if (!isRunning) {
        startTime = std::time(nullptr);
        isRunning = true;
        
        // Set the remaining time based on session type
        if (isWorkSession) {
            remainingTime = workDuration * 60;
        } else {
            if (completedSessions % sessionsBeforeLongBreak == 0 && completedSessions > 0) {
                remainingTime = longBreakDuration * 60;
            } else {
                remainingTime = breakDuration * 60;
            }
        }
    }
}

void PomodoroTimer::pauseSession() {
    if (isRunning) {
        std::time_t currentTime = std::time(nullptr);
        std::time_t elapsedTime = currentTime - startTime;
        
        if (elapsedTime < remainingTime) {
            remainingTime -= elapsedTime;
        } else {
            remainingTime = 0;
        }
        
        pausedTime = currentTime;
        isRunning = false;
    }
}

void PomodoroTimer::resumeSession() {
    if (!isRunning && pausedTime > 0) {
        startTime = std::time(nullptr);
        isRunning = true;
    }
}

void PomodoroTimer::stopSession() {
    isRunning = false;
    startTime = 0;
    pausedTime = 0;
    
    // Reset the remaining time based on session type
    if (isWorkSession) {
        remainingTime = workDuration * 60;
    } else {
        if (completedSessions % sessionsBeforeLongBreak == 0 && completedSessions > 0) {
            remainingTime = longBreakDuration * 60;
        } else {
            remainingTime = breakDuration * 60;
        }
    }
}

void PomodoroTimer::resetSession() {
    stopSession();
    completedSessions = 0;
    isWorkSession = true;
}

// Session management
bool PomodoroTimer::isSessionComplete() const {
    if (!isRunning) {
        return false;
    }
    
    std::time_t currentTime = std::time(nullptr);
    std::time_t elapsedTime = currentTime - startTime;
    
    return elapsedTime >= remainingTime;
}

void PomodoroTimer::switchSession() {
    if (isWorkSession) {
        // Work session completed, switch to break
        isWorkSession = false;
        completedSessions++;
    } else {
        // Break session completed, switch to work
        isWorkSession = true;
    }
    
    stopSession();
    startSession();
}

// Getters
int PomodoroTimer::getWorkDuration() const {
    return workDuration;
}

int PomodoroTimer::getBreakDuration() const {
    return breakDuration;
}

int PomodoroTimer::getLongBreakDuration() const {
    return longBreakDuration;
}

int PomodoroTimer::getSessionsBeforeLongBreak() const {
    return sessionsBeforeLongBreak;
}

int PomodoroTimer::getCompletedSessions() const {
    return completedSessions;
}

bool PomodoroTimer::getIsWorkSession() const {
    return isWorkSession;
}

bool PomodoroTimer::getIsRunning() const {
    return isRunning;
}

std::time_t PomodoroTimer::getRemainingTime() const {
    if (!isRunning) {
        return remainingTime;
    }
    
    std::time_t currentTime = std::time(nullptr);
    std::time_t elapsedTime = currentTime - startTime;
    
    if (elapsedTime >= remainingTime) {
        return 0;
    }
    
    return remainingTime - elapsedTime;
}

// Setters
void PomodoroTimer::setWorkDuration(int minutes) {
    if (minutes > 0) {
        workDuration = minutes;
        if (isWorkSession && !isRunning) {
            remainingTime = workDuration * 60;
        }
    }
}

void PomodoroTimer::setBreakDuration(int minutes) {
    if (minutes > 0) {
        breakDuration = minutes;
        if (!isWorkSession && !isRunning && completedSessions % sessionsBeforeLongBreak != 0) {
            remainingTime = breakDuration * 60;
        }
    }
}

void PomodoroTimer::setLongBreakDuration(int minutes) {
    if (minutes > 0) {
        longBreakDuration = minutes;
        if (!isWorkSession && !isRunning && completedSessions % sessionsBeforeLongBreak == 0 && completedSessions > 0) {
            remainingTime = longBreakDuration * 60;
        }
    }
}

void PomodoroTimer::setSessionsBeforeLongBreak(int sessions) {
    if (sessions > 0) {
        sessionsBeforeLongBreak = sessions;
    }
}

// Utility methods
std::string PomodoroTimer::getRemainingTimeString() const {
    std::time_t remaining = getRemainingTime();
    int minutes = static_cast<int>(remaining / 60);
    int seconds = static_cast<int>(remaining % 60);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

std::string PomodoroTimer::getCurrentSessionTypeString() const {
    if (isWorkSession) {
        return "Work Session";
    } else {
        if (completedSessions % sessionsBeforeLongBreak == 0 && completedSessions > 0) {
            return "Long Break";
        } else {
            return "Short Break";
        }
    }
}

std::string PomodoroTimer::getProgressString() const {
    std::stringstream ss;
    ss << completedSessions << " sessions completed";
    if (sessionsBeforeLongBreak > 0) {
        ss << " (" << (completedSessions % sessionsBeforeLongBreak) << "/" 
           << sessionsBeforeLongBreak << " before long break)";
    }
    return ss.str();
}