#include "../include/TaskManager.h"
#include <algorithm>

// Constructor
TaskManager::TaskManager() : nextTaskId(1) {}

// Task management
void TaskManager::addTask(const Task& task) {
    Task newTask = task;
    newTask.setId(getNextTaskId());
    tasks.push_back(newTask);
}

bool TaskManager::removeTask(int id) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == id) {
            tasks.erase(it);
            return true;
        }
    }
    return false;
}

bool TaskManager::editTask(int id, const Task& updatedTask) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            task.setTitle(updatedTask.getTitle());
            task.setDescription(updatedTask.getDescription());
            task.setPriority(updatedTask.getPriority());
            task.setDeadline(updatedTask.getDeadline());
            return true;
        }
    }
    return false;
}

Task* TaskManager::getTask(int id) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            return &task;
        }
    }
    return nullptr;
}

// Task retrieval
std::vector<Task> TaskManager::getAllTasks() const {
    return tasks;
}

std::vector<Task> TaskManager::getCompletedTasks() const {
    std::vector<Task> completedTasks;
    for (const auto& task : tasks) {
        if (task.isCompleted()) {
            completedTasks.push_back(task);
        }
    }
    return completedTasks;
}

std::vector<Task> TaskManager::getPendingTasks() const {
    std::vector<Task> pendingTasks;
    for (const auto& task : tasks) {
        if (!task.isCompleted()) {
            pendingTasks.push_back(task);
        }
    }
    return pendingTasks;
}

std::vector<Task> TaskManager::getTasksByPriority() const {
    std::vector<Task> sortedTasks = tasks;
    sortTasksByPriority(sortedTasks);
    return sortedTasks;
}

std::vector<Task> TaskManager::getTasksByDeadline() const {
    std::vector<Task> sortedTasks = tasks;
    sortTasksByDeadline(sortedTasks);
    return sortedTasks;
}

// Task operations
bool TaskManager::markTaskAsCompleted(int id) {
    Task* task = getTask(id);
    if (task != nullptr) {
        task->setCompleted(true);
        return true;
    }
    return false;
}

bool TaskManager::markTaskAsPending(int id) {
    Task* task = getTask(id);
    if (task != nullptr) {
        task->setCompleted(false);
        return true;
    }
    return false;
}

// Statistics
int TaskManager::getTotalTaskCount() const {
    return tasks.size();
}

int TaskManager::getCompletedTaskCount() const {
    return getCompletedTasks().size();
}

int TaskManager::getPendingTaskCount() const {
    return getPendingTasks().size();
}

std::time_t TaskManager::getTotalTimeSpent() const {
    std::time_t totalTime = 0;
    for (const auto& task : tasks) {
        totalTime += task.getTimeSpent();
    }
    return totalTime;
}

// Utility
int TaskManager::getNextTaskId() {
    return nextTaskId++;
}

void TaskManager::sortTasksByPriority(std::vector<Task>& tasks) const {
    std::sort(tasks.begin(), tasks.end(), 
              [](const Task& a, const Task& b) {
                  return a.getPriority() < b.getPriority();
              });
}

void TaskManager::sortTasksByDeadline(std::vector<Task>& tasks) const {
    std::sort(tasks.begin(), tasks.end(), 
              [](const Task& a, const Task& b) {
                  // Handle tasks with no deadline (0)
                  if (a.getDeadline() == 0) return false;
                  if (b.getDeadline() == 0) return true;
                  return a.getDeadline() < b.getDeadline();
              });
}