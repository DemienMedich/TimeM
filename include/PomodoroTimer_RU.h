#ifndef POMODORO_TIMER_H
#define POMODORO_TIMER_H

#include <ctime>
#include <string>

class PomodoroTimer {
private:
    int workDuration;  // в минутах
    int breakDuration; // в минутах
    int longBreakDuration; // в минутах
    int sessionsBeforeLongBreak;
    int completedSessions;
    bool isWorkSession;
    bool isRunning;
    std::time_t startTime;
    std::time_t pausedTime;
    std::time_t remainingTime;
    
public:
    // Конструктор
    PomodoroTimer(int workDuration = 25, int breakDuration = 5, 
                  int longBreakDuration = 15, int sessionsBeforeLongBreak = 4);
    
    // Управление таймером
    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();
    void resetSession();
    
    // Управление сессиями
    bool isSessionComplete() const;
    void switchSession();  // Переключение между работой и перерывом
    
    // Геттеры
    int getWorkDuration() const;
    int getBreakDuration() const;
    int getLongBreakDuration() const;
    int getSessionsBeforeLongBreak() const;
    int getCompletedSessions() const;
    bool getIsWorkSession() const;
    bool getIsRunning() const;
    std::time_t getRemainingTime() const;
    
    // Сеттеры
    void setWorkDuration(int minutes);
    void setBreakDuration(int minutes);
    void setLongBreakDuration(int minutes);
    void setSessionsBeforeLongBreak(int sessions);
    
    // Вспомогательные методы
    std::string getRemainingTimeString() const;
    std::string getCurrentSessionTypeString() const;
    std::string getProgressString() const;
};

#endif // POMODORO_TIMER_H