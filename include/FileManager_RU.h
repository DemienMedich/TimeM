#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "Task.h"
#include "PomodoroTimer.h"

class FileManager {
public:
    // Конструктор
    FileManager();
    
    // Сохранение данных задач
    bool saveTasks(const std::vector<Task>& tasks, const std::string& filename = "tasks.dat");
    std::vector<Task> loadTasks(const std::string& filename = "tasks.dat");
    
    // Сохранение настроек
    bool saveSettings(const PomodoroTimer& pomodoroTimer, const std::string& filename = "settings.dat");
    bool loadSettings(PomodoroTimer& pomodoroTimer, const std::string& filename = "settings.dat");
    
    // Вспомогательные методы
    bool fileExists(const std::string& filename) const;
    bool createBackup(const std::string& filename) const;
    bool restoreFromBackup(const std::string& filename) const;
};

#endif // FILE_MANAGER_H