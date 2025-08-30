#include "../include/FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>

// Constructor
FileManager::FileManager() {}

// Task data persistence
bool FileManager::saveTasks(const std::vector<Task>& tasks, const std::string& filename) {
    try {
        // Create backup if file exists
        if (fileExists(filename)) {
            createBackup(filename);
        }
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        // Write the number of tasks
        file << tasks.size() << std::endl;
        
        // Write each task
        for (const auto& task : tasks) {
            file << task.serialize() << std::endl;
        }
        
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving tasks: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Task> FileManager::loadTasks(const std::string& filename) {
    std::vector<Task> tasks;
    
    try {
        if (!fileExists(filename)) {
            return tasks;
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            return tasks;
        }
        
        // Read the number of tasks
        std::string line;
        std::getline(file, line);
        int taskCount = std::stoi(line);
        
        // Read each task
        for (int i = 0; i < taskCount; ++i) {
            std::getline(file, line);
            if (!line.empty()) {
                Task task = Task::deserialize(line);
                tasks.push_back(task);
            }
        }
        
        file.close();
    } catch (const std::exception& e) {
        std::cerr << "Error loading tasks: " << e.what() << std::endl;
    }
    
    return tasks;
}

// Settings persistence
bool FileManager::saveSettings(const PomodoroTimer& pomodoroTimer, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << pomodoroTimer.getWorkDuration() << std::endl;
        file << pomodoroTimer.getBreakDuration() << std::endl;
        file << pomodoroTimer.getLongBreakDuration() << std::endl;
        file << pomodoroTimer.getSessionsBeforeLongBreak() << std::endl;
        
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving settings: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::loadSettings(PomodoroTimer& pomodoroTimer, const std::string& filename) {
    try {
        if (!fileExists(filename)) {
            return false;
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string line;
        
        // Read work duration
        std::getline(file, line);
        int workDuration = std::stoi(line);
        
        // Read break duration
        std::getline(file, line);
        int breakDuration = std::stoi(line);
        
        // Read long break duration
        std::getline(file, line);
        int longBreakDuration = std::stoi(line);
        
        // Read sessions before long break
        std::getline(file, line);
        int sessionsBeforeLongBreak = std::stoi(line);
        
        // Update pomodoro timer settings
        pomodoroTimer.setWorkDuration(workDuration);
        pomodoroTimer.setBreakDuration(breakDuration);
        pomodoroTimer.setLongBreakDuration(longBreakDuration);
        pomodoroTimer.setSessionsBeforeLongBreak(sessionsBeforeLongBreak);
        
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading settings: " << e.what() << std::endl;
        return false;
    }
}

// Utility methods
bool FileManager::fileExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.good();
}

bool FileManager::createBackup(const std::string& filename) const {
    if (!fileExists(filename)) {
        return false;
    }
    
    std::string backupFilename = filename + ".bak";
    
    try {
        std::ifstream src(filename, std::ios::binary);
        std::ofstream dst(backupFilename, std::ios::binary);
        dst << src.rdbuf();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating backup: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::restoreFromBackup(const std::string& filename) const {
    std::string backupFilename = filename + ".bak";
    
    if (!fileExists(backupFilename)) {
        return false;
    }
    
    try {
        std::ifstream src(backupFilename, std::ios::binary);
        std::ofstream dst(filename, std::ios::binary);
        dst << src.rdbuf();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error restoring from backup: " << e.what() << std::endl;
        return false;
    }
}