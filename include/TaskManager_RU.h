#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>
#include "Task.h"

class TaskManager {
private:
    std::vector<Task> tasks;
    int nextTaskId;

public:
    // Конструктор
    TaskManager();
    
    // Управление задачами
    void addTask(const Task& task);
    bool removeTask(int id);
    bool editTask(int id, const Task& updatedTask);
    Task* getTask(int id);
    
    // Получение задач
    std::vector<Task> getAllTasks() const;
    std::vector<Task> getCompletedTasks() const;
    std::vector<Task> getPendingTasks() const;
    std::vector<Task> getTasksByPriority() const;
    std::vector<Task> getTasksByDeadline() const;
    
    // Операции с задачами
    bool markTaskAsCompleted(int id);
    bool markTaskAsPending(int id);
    
    // Статистика
    int getTotalTaskCount() const;
    int getCompletedTaskCount() const;
    int getPendingTaskCount() const;
    std::time_t getTotalTimeSpent() const;
    
    // Вспомогательные методы
    int getNextTaskId();
    void sortTasksByPriority(std::vector<Task>& tasks) const;
    void sortTasksByDeadline(std::vector<Task>& tasks) const;
};

#endif // TASK_MANAGER_H