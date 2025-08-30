#include "../include/UserInterface.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <ctime>
#include <thread>
#include <chrono>

// Constructor
UserInterface::UserInterface() : running(true) {}

// Main method to run the application
void UserInterface::run() {
    // Load saved data
    loadData();
    
    // Main application loop
    while (running) {
        clearScreen();
        showMainMenu();
        handleMainMenu();
    }
    
    // Save data before exiting
    saveData();
}

// Helper methods for UI
void UserInterface::clearScreen() const {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void UserInterface::pauseScreen() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UserInterface::displayHeader(const std::string& title) const {
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "====================================\n\n";
}

void UserInterface::displayFooter() const {
    std::cout << "\n====================================\n";
}

int UserInterface::getMenuChoice(int maxChoice) const {
    int choice = 0;
    bool validInput = false;
    
    while (!validInput) {
        std::cout << "\nEnter your choice (1-" << maxChoice << "): ";
        std::cin >> choice;
        
        if (std::cin.fail() || choice < 1 || choice > maxChoice) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please try again.\n";
        } else {
            validInput = true;
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string UserInterface::getInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int UserInterface::getIntInput(const std::string& prompt, int min, int max) const {
    int input;
    bool validInput = false;
    
    while (!validInput) {
        std::cout << prompt;
        std::cin >> input;
        
        if (std::cin.fail() || input < min || input > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
        } else {
            validInput = true;
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

std::time_t UserInterface::getDateInput(const std::string& prompt) const {
    std::cout << prompt << " (YYYY-MM-DD HH:MM, or leave empty for no deadline): ";
    std::string input;
    std::getline(std::cin, input);
    
    if (input.empty()) {
        return 0;
    }
    
    std::tm timeinfo = {};
    std::istringstream ss(input);
    ss >> std::get_time(&timeinfo, "%Y-%m-%d %H:%M");
    
    if (ss.fail()) {
        std::cout << "Invalid date format. Using no deadline.\n";
        return 0;
    }
    
    return std::mktime(&timeinfo);
}

// Task UI methods
void UserInterface::displayTask(const Task& task) const {
    std::cout << "ID: " << task.getId() << "\n";
    std::cout << "Title: " << task.getTitle() << "\n";
    std::cout << "Description: " << task.getDescription() << "\n";
    std::cout << "Priority: " << task.getPriorityString() << " (" << task.getPriority() << ")\n";
    std::cout << "Deadline: " << task.getDeadlineString() << "\n";
    std::cout << "Status: " << (task.isCompleted() ? "Completed" : "Pending") << "\n";
    std::cout << "Time spent: " << task.getTimeSpentString() << "\n";
    
    const auto& subtasks = task.getSubtasks();
    if (!subtasks.empty()) {
        std::cout << "Subtasks (" << subtasks.size() << "):\n";
        for (const auto& subtask : subtasks) {
            std::cout << "  - [" << (subtask.isCompleted() ? "X" : " ") << "] " 
                      << subtask.getTitle() << "\n";
        }
    }
}

void UserInterface::displayTaskList(const std::vector<Task>& tasks) const {
    if (tasks.empty()) {
        std::cout << "No tasks found.\n";
        return;
    }
    
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(30) << "Title" 
              << std::setw(15) << "Priority" 
              << std::setw(20) << "Deadline" 
              << std::setw(10) << "Status" 
              << std::setw(15) << "Time Spent" << "\n";
    
    std::cout << std::string(95, '-') << "\n";
    
    for (const auto& task : tasks) {
        std::cout << std::left << std::setw(5) << task.getId() 
                  << std::setw(30) << (task.getTitle().length() > 27 ? 
                                       task.getTitle().substr(0, 27) + "..." : task.getTitle())
                  << std::setw(15) << task.getPriority()
                  << std::setw(20) << task.getDeadlineString()
                  << std::setw(10) << (task.isCompleted() ? "Done" : "Pending")
                  << std::setw(15) << task.getTimeSpentString() << "\n";
    }
}

Task UserInterface::createNewTask() const {
    std::string title = getInput("Enter task title: ");
    std::string description = getInput("Enter task description (optional): ");
    
    std::cout << "\nPriority Levels:\n";
    std::cout << "1 - Urgent & Important\n";
    std::cout << "2 - Important, Not Urgent\n";
    std::cout << "3 - Urgent, Not Important\n";
    std::cout << "4 - Not Urgent & Not Important\n";
    int priority = getIntInput("Enter priority (1-4): ", 1, 4);
    
    std::time_t deadline = getDateInput("Enter deadline");
    
    return Task(0, title, description, priority, deadline);
}

void UserInterface::editTaskUI(Task& task) const {
    clearScreen();
    displayHeader("Edit Task");
    displayTask(task);
    
    std::cout << "\nWhat would you like to edit?\n";
    std::cout << "1. Title\n";
    std::cout << "2. Description\n";
    std::cout << "3. Priority\n";
    std::cout << "4. Deadline\n";
    std::cout << "5. Status\n";
    std::cout << "6. Back\n";
    
    int choice = getMenuChoice(6);
    
    switch (choice) {
        case 1: {
            std::string title = getInput("Enter new title: ");
            task.setTitle(title);
            break;
        }
        case 2: {
            std::string description = getInput("Enter new description: ");
            task.setDescription(description);
            break;
        }
        case 3: {
            std::cout << "\nPriority Levels:\n";
            std::cout << "1 - Urgent & Important\n";
            std::cout << "2 - Important, Not Urgent\n";
            std::cout << "3 - Urgent, Not Important\n";
            std::cout << "4 - Not Urgent & Not Important\n";
            int priority = getIntInput("Enter new priority (1-4): ", 1, 4);
            task.setPriority(priority);
            break;
        }
        case 4: {
            std::time_t deadline = getDateInput("Enter new deadline");
            task.setDeadline(deadline);
            break;
        }
        case 5: {
            bool completed = task.isCompleted();
            std::cout << "Current status: " << (completed ? "Completed" : "Pending") << "\n";
            std::cout << "Change to " << (!completed ? "Completed" : "Pending") << "? (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (confirm == 'y' || confirm == 'Y') {
                task.setCompleted(!completed);
            }
            break;
        }
        case 6:
            return;
    }
    
    std::cout << "Task updated successfully!\n";
    pauseScreen();
}

// Menu handlers
void UserInterface::handleMainMenu() {
    int choice = getMenuChoice(6);
    
    switch (choice) {
        case 1:
            clearScreen();
            showTaskMenu();
            handleTaskMenu();
            break;
        case 2:
            clearScreen();
            showTimeTrackingMenu();
            handleTimeTrackingMenu();
            break;
        case 3:
            clearScreen();
            showPomodoroMenu();
            handlePomodoroMenu();
            break;
        case 4:
            clearScreen();
            showStatisticsMenu();
            handleStatisticsMenu();
            break;
        case 5:
            clearScreen();
            showSettingsMenu();
            handleSettingsMenu();
            break;
        case 6:
            running = false;
            break;
    }
}

void UserInterface::handleTaskMenu() {
    int choice = getMenuChoice(7);
    
    switch (choice) {
        case 1: {
            // View all tasks
            clearScreen();
            displayHeader("All Tasks");
            displayTaskList(taskManager.getAllTasks());
            pauseScreen();
            break;
        }
        case 2: {
            // View pending tasks
            clearScreen();
            displayHeader("Pending Tasks");
            displayTaskList(taskManager.getPendingTasks());
            pauseScreen();
            break;
        }
        case 3: {
            // View completed tasks
            clearScreen();
            displayHeader("Completed Tasks");
            displayTaskList(taskManager.getCompletedTasks());
            pauseScreen();
            break;
        }
        case 4: {
            // Add new task
            clearScreen();
            displayHeader("Add New Task");
            Task newTask = createNewTask();
            taskManager.addTask(newTask);
            std::cout << "Task added successfully!\n";
            pauseScreen();
            break;
        }
        case 5: {
            // Edit task
            clearScreen();
            displayHeader("Edit Task");
            displayTaskList(taskManager.getAllTasks());
            
            if (taskManager.getTotalTaskCount() == 0) {
                pauseScreen();
                break;
            }
            
            int taskId = getIntInput("Enter task ID to edit: ", 1, 9999);
            Task* task = taskManager.getTask(taskId);
            
            if (task == nullptr) {
                std::cout << "Task not found.\n";
                pauseScreen();
                break;
            }
            
            editTaskUI(*task);
            break;
        }
        case 6: {
            // Delete task
            clearScreen();
            displayHeader("Delete Task");
            displayTaskList(taskManager.getAllTasks());
            
            if (taskManager.getTotalTaskCount() == 0) {
                pauseScreen();
                break;
            }
            
            int taskId = getIntInput("Enter task ID to delete: ", 1, 9999);
            
            std::cout << "Are you sure you want to delete this task? (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (confirm == 'y' || confirm == 'Y') {
                if (taskManager.removeTask(taskId)) {
                    std::cout << "Task deleted successfully!\n";
                } else {
                    std::cout << "Task not found.\n";
                }
            }
            
            pauseScreen();
            break;
        }
        case 7:
            // Back to main menu
            break;
    }
}

void UserInterface::handleTimeTrackingMenu() {
    int choice = getMenuChoice(5);
    
    switch (choice) {
        case 1: {
            // Start tracking
            clearScreen();
            displayHeader("Start Time Tracking");
            
            std::vector<Task> pendingTasks = taskManager.getPendingTasks();
            displayTaskList(pendingTasks);
            
            if (pendingTasks.empty()) {
                std::cout << "No pending tasks to track. Create a task first.\n";
                pauseScreen();
                break;
            }
            
            int taskId = getIntInput("Enter task ID to track: ", 1, 9999);
            Task* task = taskManager.getTask(taskId);
            
            if (task == nullptr) {
                std::cout << "Task not found.\n";
                pauseScreen();
                break;
            }
            
            if (task->isCompleted()) {
                std::cout << "Cannot track a completed task.\n";
                pauseScreen();
                break;
            }
            
            timeTracker.startTimer(task);
            std::cout << "Time tracking started for task: " << task->getTitle() << "\n";
            pauseScreen();
            break;
        }
        case 2: {
            // Pause tracking
            clearScreen();
            displayHeader("Pause Time Tracking");
            
            if (!timeTracker.getIsRunning()) {
                std::cout << "No active time tracking session.\n";
                pauseScreen();
                break;
            }
            
            Task* currentTask = timeTracker.getCurrentTask();
            if (currentTask != nullptr) {
                std::cout << "Pausing time tracking for task: " << currentTask->getTitle() << "\n";
                std::cout << "Elapsed time: " << timeTracker.getElapsedTimeString() << "\n";
            }
            
            timeTracker.pauseTimer();
            std::cout << "Time tracking paused.\n";
            pauseScreen();
            break;
        }
        case 3: {
            // Resume tracking
            clearScreen();
            displayHeader("Resume Time Tracking");
            
            if (timeTracker.getIsRunning()) {
                std::cout << "Time tracking is already active.\n";
                pauseScreen();
                break;
            }
            
            Task* currentTask = timeTracker.getCurrentTask();
            if (currentTask == nullptr) {
                std::cout << "No task selected for tracking.\n";
                pauseScreen();
                break;
            }
            
            timeTracker.resumeTimer();
            std::cout << "Time tracking resumed for task: " << currentTask->getTitle() << "\n";
            pauseScreen();
            break;
        }
        case 4: {
            // Stop tracking
            clearScreen();
            displayHeader("Stop Time Tracking");
            
            Task* currentTask = timeTracker.getCurrentTask();
            if (currentTask == nullptr) {
                std::cout << "No active time tracking session.\n";
                pauseScreen();
                break;
            }
            
            std::cout << "Stopping time tracking for task: " << currentTask->getTitle() << "\n";
            std::cout << "Total time tracked: " << timeTracker.getElapsedTimeString() << "\n";
            
            timeTracker.stopTimer();
            std::cout << "Time tracking stopped.\n";
            pauseScreen();
            break;
        }
        case 5:
            // Back to main menu
            break;
    }
}

void UserInterface::handlePomodoroMenu() {
    int choice = getMenuChoice(6);
    
    switch (choice) {
        case 1: {
            // Start Pomodoro
            clearScreen();
            displayHeader("Pomodoro Timer");
            
            if (pomodoroTimer.getIsRunning()) {
                std::cout << "Pomodoro timer is already running.\n";
                pauseScreen();
                break;
            }
            
            pomodoroTimer.startSession();
            
            std::cout << "Pomodoro " << pomodoroTimer.getCurrentSessionTypeString() << " started!\n";
            std::cout << "Duration: " << pomodoroTimer.getRemainingTimeString() << "\n";
            std::cout << "Progress: " << pomodoroTimer.getProgressString() << "\n\n";
            
            std::cout << "Press Enter to return to menu (timer will continue in background)...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        case 2: {
            // Pause Pomodoro
            clearScreen();
            displayHeader("Pause Pomodoro");
            
            if (!pomodoroTimer.getIsRunning()) {
                std::cout << "No active Pomodoro session.\n";
                pauseScreen();
                break;
            }
            
            pomodoroTimer.pauseSession();
            std::cout << "Pomodoro session paused. Remaining time: " 
                      << pomodoroTimer.getRemainingTimeString() << "\n";
            pauseScreen();
            break;
        }
        case 3: {
            // Resume Pomodoro
            clearScreen();
            displayHeader("Resume Pomodoro");
            
            if (pomodoroTimer.getIsRunning()) {
                std::cout << "Pomodoro session is already running.\n";
                pauseScreen();
                break;
            }
            
            pomodoroTimer.resumeSession();
            std::cout << "Pomodoro session resumed. Remaining time: " 
                      << pomodoroTimer.getRemainingTimeString() << "\n";
            pauseScreen();
            break;
        }
        case 4: {
            // Stop Pomodoro
            clearScreen();
            displayHeader("Stop Pomodoro");
            
            if (!pomodoroTimer.getIsRunning()) {
                std::cout << "No active Pomodoro session.\n";
                pauseScreen();
                break;
            }
            
            pomodoroTimer.stopSession();
            std::cout << "Pomodoro session stopped.\n";
            pauseScreen();
            break;
        }
        case 5: {
            // Check Pomodoro Status
            clearScreen();
            displayHeader("Pomodoro Status");
            
            std::cout << "Current session: " << pomodoroTimer.getCurrentSessionTypeString() << "\n";
            std::cout << "Status: " << (pomodoroTimer.getIsRunning() ? "Running" : "Paused") << "\n";
            std::cout << "Remaining time: " << pomodoroTimer.getRemainingTimeString() << "\n";
            std::cout << "Progress: " << pomodoroTimer.getProgressString() << "\n";
            
            pauseScreen();
            break;
        }
        case 6:
            // Back to main menu
            break;
    }
}

void UserInterface::handleStatisticsMenu() {
    int choice = getMenuChoice(4);
    
    switch (choice) {
        case 1: {
            // Task Statistics
            clearScreen();
            displayHeader("Task Statistics");
            
            int totalTasks = taskManager.getTotalTaskCount();
            int completedTasks = taskManager.getCompletedTaskCount();
            int pendingTasks = taskManager.getPendingTaskCount();
            
            std::cout << "Total Tasks: " << totalTasks << "\n";
            std::cout << "Completed Tasks: " << completedTasks << "\n";
            std::cout << "Pending Tasks: " << pendingTasks << "\n";
            
            if (totalTasks > 0) {
                double completionRate = (static_cast<double>(completedTasks) / totalTasks) * 100;
                std::cout << "Completion Rate: " << std::fixed << std::setprecision(2) 
                          << completionRate << "%\n";
            }
            
            pauseScreen();
            break;
        }
        case 2: {
            // Time Statistics
            clearScreen();
            displayHeader("Time Statistics");
            
            std::time_t totalTimeSpent = taskManager.getTotalTimeSpent();
            int hours = static_cast<int>(totalTimeSpent / 3600);
            int minutes = static_cast<int>((totalTimeSpent % 3600) / 60);
            int seconds = static_cast<int>(totalTimeSpent % 60);
            
            std::cout << "Total Time Spent on Tasks: " 
                      << hours << "h " << minutes << "m " << seconds << "s\n\n";
            
            std::cout << "Time Spent by Task:\n";
            std::cout << std::string(50, '-') << "\n";
            
            std::vector<Task> allTasks = taskManager.getAllTasks();
            for (const auto& task : allTasks) {
                if (task.getTimeSpent() > 0) {
                    std::cout << std::left << std::setw(30) << task.getTitle() 
                              << std::setw(20) << task.getTimeSpentString() << "\n";
                }
            }
            
            pauseScreen();
            break;
        }
        case 3: {
            // Pomodoro Statistics
            clearScreen();
            displayHeader("Pomodoro Statistics");
            
            std::cout << "Completed Pomodoro Sessions: " << pomodoroTimer.getCompletedSessions() << "\n";
            std::cout << "Work Duration: " << pomodoroTimer.getWorkDuration() << " minutes\n";
            std::cout << "Break Duration: " << pomodoroTimer.getBreakDuration() << " minutes\n";
            std::cout << "Long Break Duration: " << pomodoroTimer.getLongBreakDuration() << " minutes\n";
            std::cout << "Sessions Before Long Break: " << pomodoroTimer.getSessionsBeforeLongBreak() << "\n";
            
            pauseScreen();
            break;
        }
        case 4:
            // Back to main menu
            break;
    }
}

void UserInterface::handleSettingsMenu() {
    int choice = getMenuChoice(3);
    
    switch (choice) {
        case 1: {
            // Pomodoro Settings
            clearScreen();
            displayHeader("Pomodoro Settings");
            
            std::cout << "Current Settings:\n";
            std::cout << "1. Work Duration: " << pomodoroTimer.getWorkDuration() << " minutes\n";
            std::cout << "2. Break Duration: " << pomodoroTimer.getBreakDuration() << " minutes\n";
            std::cout << "3. Long Break Duration: " << pomodoroTimer.getLongBreakDuration() << " minutes\n";
            std::cout << "4. Sessions Before Long Break: " << pomodoroTimer.getSessionsBeforeLongBreak() << "\n";
            std::cout << "5. Back\n";
            
            int settingChoice = getMenuChoice(5);
            
            switch (settingChoice) {
                case 1: {
                    int duration = getIntInput("Enter new work duration (minutes): ", 1, 60);
                    pomodoroTimer.setWorkDuration(duration);
                    std::cout << "Work duration updated.\n";
                    break;
                }
                case 2: {
                    int duration = getIntInput("Enter new break duration (minutes): ", 1, 30);
                    pomodoroTimer.setBreakDuration(duration);
                    std::cout << "Break duration updated.\n";
                    break;
                }
                case 3: {
                    int duration = getIntInput("Enter new long break duration (minutes): ", 1, 60);
                    pomodoroTimer.setLongBreakDuration(duration);
                    std::cout << "Long break duration updated.\n";
                    break;
                }
                case 4: {
                    int sessions = getIntInput("Enter new sessions before long break: ", 1, 10);
                    pomodoroTimer.setSessionsBeforeLongBreak(sessions);
                    std::cout << "Sessions before long break updated.\n";
                    break;
                }
                case 5:
                    break;
            }
            
            pauseScreen();
            break;
        }
        case 2: {
            // Save/Load Settings
            clearScreen();
            displayHeader("Save/Load Settings");
            
            std::cout << "1. Save current settings\n";
            std::cout << "2. Load saved settings\n";
            std::cout << "3. Back\n";
            
            int saveLoadChoice = getMenuChoice(3);
            
            switch (saveLoadChoice) {
                case 1: {
                    if (fileManager.saveSettings(pomodoroTimer)) {
                        std::cout << "Settings saved successfully.\n";
                    } else {
                        std::cout << "Failed to save settings.\n";
                    }
                    break;
                }
                case 2: {
                    if (fileManager.loadSettings(pomodoroTimer)) {
                        std::cout << "Settings loaded successfully.\n";
                    } else {
                        std::cout << "Failed to load settings or no saved settings found.\n";
                    }
                    break;
                }
                case 3:
                    break;
            }
            
            pauseScreen();
            break;
        }
        case 3:
            // Back to main menu
            break;
    }
}

// Menu display methods
void UserInterface::showMainMenu() const {
    displayHeader("Time Management & Anti-Procrastination App");
    
    // Show current time tracking status if active
    if (timeTracker.getIsRunning()) {
        Task* currentTask = timeTracker.getCurrentTask();
        if (currentTask != nullptr) {
            std::cout << "Currently tracking: " << currentTask->getTitle() << "\n";
            std::cout << "Elapsed time: " << timeTracker.getElapsedTimeString() << "\n\n";
        }
    }
    
    // Show pomodoro status if active
    if (pomodoroTimer.getIsRunning()) {
        std::cout << "Pomodoro: " << pomodoroTimer.getCurrentSessionTypeString() 
                  << " - " << pomodoroTimer.getRemainingTimeString() << " remaining\n\n";
    }
    
    std::cout << "1. Task Management\n";
    std::cout << "2. Time Tracking\n";
    std::cout << "3. Pomodoro Timer\n";
    std::cout << "4. Statistics\n";
    std::cout << "5. Settings\n";
    std::cout << "6. Exit\n";
}

void UserInterface::showTaskMenu() const {
    displayHeader("Task Management");
    
    std::cout << "1. View All Tasks\n";
    std::cout << "2. View Pending Tasks\n";
    std::cout << "3. View Completed Tasks\n";
    std::cout << "4. Add New Task\n";
    std::cout << "5. Edit Task\n";
    std::cout << "6. Delete Task\n";
    std::cout << "7. Back to Main Menu\n";
}

void UserInterface::showTimeTrackingMenu() const {
    displayHeader("Time Tracking");
    
    // Show current tracking status
    if (timeTracker.getIsRunning()) {
        Task* currentTask = timeTracker.getCurrentTask();
        if (currentTask != nullptr) {
            std::cout << "Currently tracking: " << currentTask->getTitle() << "\n";
            std::cout << "Elapsed time: " << timeTracker.getElapsedTimeString() << "\n\n";
        }
    } else if (timeTracker.getCurrentTask() != nullptr) {
        std::cout << "Paused tracking: " << timeTracker.getCurrentTask()->getTitle() << "\n";
        std::cout << "Elapsed time: " << timeTracker.getElapsedTimeString() << "\n\n";
    }
    
    std::cout << "1. Start Tracking\n";
    std::cout << "2. Pause Tracking\n";
    std::cout << "3. Resume Tracking\n";
    std::cout << "4. Stop Tracking\n";
    std::cout << "5. Back to Main Menu\n";
}

void UserInterface::showPomodoroMenu() const {
    displayHeader("Pomodoro Timer");
    
    // Show current pomodoro status
    if (pomodoroTimer.getIsRunning()) {
        std::cout << "Current session: " << pomodoroTimer.getCurrentSessionTypeString() << "\n";
        std::cout << "Remaining time: " << pomodoroTimer.getRemainingTimeString() << "\n";
        std::cout << "Progress: " << pomodoroTimer.getProgressString() << "\n\n";
    } else {
        std::cout << "Pomodoro timer is not active.\n\n";
    }
    
    std::cout << "1. Start Pomodoro\n";
    std::cout << "2. Pause Pomodoro\n";
    std::cout << "3. Resume Pomodoro\n";
    std::cout << "4. Stop Pomodoro\n";
    std::cout << "5. Check Pomodoro Status\n";
    std::cout << "6. Back to Main Menu\n";
}

void UserInterface::showStatisticsMenu() const {
    displayHeader("Statistics");
    
    std::cout << "1. Task Statistics\n";
    std::cout << "2. Time Statistics\n";
    std::cout << "3. Pomodoro Statistics\n";
    std::cout << "4. Back to Main Menu\n";
}

void UserInterface::showSettingsMenu() const {
    displayHeader("Settings");
    
    std::cout << "1. Pomodoro Settings\n";
    std::cout << "2. Save/Load Settings\n";
    std::cout << "3. Back to Main Menu\n";
}

// Data management
void UserInterface::loadData() {
    std::vector<Task> loadedTasks = fileManager.loadTasks();
    for (const auto& task : loadedTasks) {
        taskManager.addTask(task);
    }
    
    fileManager.loadSettings(pomodoroTimer);
}

void UserInterface::saveData() {
    fileManager.saveTasks(taskManager.getAllTasks());
    fileManager.saveSettings(pomodoroTimer);
}