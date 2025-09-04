#pragma once
#include <string>
#include <vector>

class Task {
    std::string name;
public:
    Task(const std::string& n) : name(n) {}
    const std::string& getName() const { return name; }
};

class TaskManager {
    std::vector<Task> tasks;
public:
    void addTask(const std::string& t) { tasks.emplace_back(t); }
    const std::vector<Task>& getTasks() const { return tasks; }
};
