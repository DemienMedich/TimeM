#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "Task.h"
#include "PomodoroTimer.h"

class FileManager {
public:
    // Constructor
    FileManager();
    
    // Task data persistence
    bool saveTasks(const std::vector<Task>& tasks, const std::string& filename = "tasks.dat");
    std::vector<Task> loadTasks(const std::string& filename = "tasks.dat");
    
    // Settings persistence
    bool saveSettings(const PomodoroTimer& pomodoroTimer, const std::string& filename = "settings.dat");
    bool loadSettings(PomodoroTimer& pomodoroTimer, const std::string& filename = "settings.dat");
    
    // Utility methods
    bool fileExists(const std::string& filename) const;
    bool createBackup(const std::string& filename) const;
    bool restoreFromBackup(const std::string& filename) const;
};

#endif // FILE_MANAGER_H