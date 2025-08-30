#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include "TaskManager.h"
#include "TimeTracker.h"
#include "PomodoroTimer.h"
#include "FileManager.h"

class UserInterface {
private:
    TaskManager taskManager;
    TimeTracker timeTracker;
    PomodoroTimer pomodoroTimer;
    FileManager fileManager;
    bool running;
    
    // Вспомогательные методы для интерфейса
    void clearScreen() const;
    void pauseScreen() const;
    void displayHeader(const std::string& title) const;
    void displayFooter() const;
    int getMenuChoice(int maxChoice) const;
    std::string getInput(const std::string& prompt) const;
    int getIntInput(const std::string& prompt, int min = 0, int max = 100) const;
    std::time_t getDateInput(const std::string& prompt) const;
    
    // Методы интерфейса для задач
    void displayTask(const Task& task) const;
    void displayTaskList(const std::vector<Task>& tasks) const;
    Task createNewTask() const;
    void editTaskUI(Task& task) const;
    
    // Обработчики меню
    void handleMainMenu();
    void handleTaskMenu();
    void handleTimeTrackingMenu();
    void handlePomodoroMenu();
    void handleStatisticsMenu();
    void handleSettingsMenu();
    
public:
    // Конструктор
    UserInterface();
    
    // Основной метод для запуска приложения
    void run();
    
    // Методы отображения меню
    void showMainMenu() const;
    void showTaskMenu() const;
    void showTimeTrackingMenu() const;
    void showPomodoroMenu() const;
    void showStatisticsMenu() const;
    void showSettingsMenu() const;
    
    // Управление данными
    void loadData();
    void saveData();
};

#endif // USER_INTERFACE_H