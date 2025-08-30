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
    
    // Helper methods for UI
    void clearScreen() const;
    void pauseScreen() const;
    void displayHeader(const std::string& title) const;
    void displayFooter() const;
    int getMenuChoice(int maxChoice) const;
    std::string getInput(const std::string& prompt) const;
    int getIntInput(const std::string& prompt, int min = 0, int max = 100) const;
    std::time_t getDateInput(const std::string& prompt) const;
    
    // Task UI methods
    void displayTask(const Task& task) const;
    void displayTaskList(const std::vector<Task>& tasks) const;
    Task createNewTask() const;
    void editTaskUI(Task& task) const;
    
    // Menu handlers
    void handleMainMenu();
    void handleTaskMenu();
    void handleTimeTrackingMenu();
    void handlePomodoroMenu();
    void handleStatisticsMenu();
    void handleSettingsMenu();
    
public:
    // Constructor
    UserInterface();
    
    // Main method to run the application
    void run();
    
    // Menu display methods
    void showMainMenu() const;
    void showTaskMenu() const;
    void showTimeTrackingMenu() const;
    void showPomodoroMenu() const;
    void showStatisticsMenu() const;
    void showSettingsMenu() const;
    
    // Data management
    void loadData();
    void saveData();
};

#endif // USER_INTERFACE_H