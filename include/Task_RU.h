#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <ctime>

class Task {
private:
    int id;
    std::string title;
    std::string description;
    int priority;  // 1-4 (матрица срочно/важно)
    std::time_t deadline;
    bool completed;
    std::vector<Task> subtasks;
    std::time_t timeSpent;

public:
    // Конструкторы
    Task();
    Task(int id, const std::string& title, const std::string& description = "", 
         int priority = 2, std::time_t deadline = 0);
    
    // Геттеры
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    int getPriority() const;
    std::time_t getDeadline() const;
    bool isCompleted() const;
    std::vector<Task> getSubtasks() const;
    std::time_t getTimeSpent() const;
    
    // Сеттеры
    void setId(int id);
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setPriority(int priority);
    void setDeadline(std::time_t deadline);
    void setCompleted(bool completed);
    void addTimeSpent(std::time_t time);
    
    // Управление подзадачами
    void addSubtask(const Task& subtask);
    void removeSubtask(int subtaskId);
    void clearSubtasks();
    
    // Вспомогательные методы
    std::string getPriorityString() const;
    std::string getDeadlineString() const;
    std::string getTimeSpentString() const;
    
    // Для сериализации
    std::string serialize() const;
    static Task deserialize(const std::string& data);
};

#endif // TASK_H