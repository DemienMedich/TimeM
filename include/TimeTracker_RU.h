#ifndef TIME_TRACKER_H
#define TIME_TRACKER_H

#include <ctime>
#include "Task.h"

class TimeTracker {
private:
    std::time_t startTime;
    std::time_t totalTime;
    bool isRunning;
    Task* currentTask;

public:
    // Конструктор
    TimeTracker();
    
    // Управление таймером
    void startTimer(Task* task);
    void pauseTimer();
    void stopTimer();
    void resumeTimer();
    
    // Геттеры
    std::time_t getElapsedTime() const;
    std::time_t getTotalTime() const;
    bool getIsRunning() const;
    Task* getCurrentTask() const;
    
    // Вспомогательные методы
    std::string getElapsedTimeString() const;
    std::string getTotalTimeString() const;
};

#endif // TIME_TRACKER_H