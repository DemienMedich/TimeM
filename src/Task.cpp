#include "../include/Task.h"
#include <sstream>
#include <iomanip>

// Constructors
Task::Task() : id(0), title(""), description(""), priority(2), 
               deadline(0), completed(false), timeSpent(0) {}

Task::Task(int id, const std::string& title, const std::string& description, 
           int priority, std::time_t deadline) 
    : id(id), title(title), description(description), 
      priority(priority), deadline(deadline), completed(false), timeSpent(0) {}

// Getters
int Task::getId() const {
    return id;
}

std::string Task::getTitle() const {
    return title;
}

std::string Task::getDescription() const {
    return description;
}

int Task::getPriority() const {
    return priority;
}

std::time_t Task::getDeadline() const {
    return deadline;
}

bool Task::isCompleted() const {
    return completed;
}

std::vector<Task> Task::getSubtasks() const {
    return subtasks;
}

std::time_t Task::getTimeSpent() const {
    return timeSpent;
}

// Setters
void Task::setId(int id) {
    this->id = id;
}

void Task::setTitle(const std::string& title) {
    this->title = title;
}

void Task::setDescription(const std::string& description) {
    this->description = description;
}

void Task::setPriority(int priority) {
    if (priority >= 1 && priority <= 4) {
        this->priority = priority;
    }
}

void Task::setDeadline(std::time_t deadline) {
    this->deadline = deadline;
}

void Task::setCompleted(bool completed) {
    this->completed = completed;
}

void Task::addTimeSpent(std::time_t time) {
    this->timeSpent += time;
}

// Subtask management
void Task::addSubtask(const Task& subtask) {
    subtasks.push_back(subtask);
}

void Task::removeSubtask(int subtaskId) {
    for (auto it = subtasks.begin(); it != subtasks.end(); ++it) {
        if (it->getId() == subtaskId) {
            subtasks.erase(it);
            return;
        }
    }
}

void Task::clearSubtasks() {
    subtasks.clear();
}

// Utility methods
std::string Task::getPriorityString() const {
    switch (priority) {
        case 1: return "Urgent & Important";
        case 2: return "Important, Not Urgent";
        case 3: return "Urgent, Not Important";
        case 4: return "Not Urgent & Not Important";
        default: return "Unknown";
    }
}

std::string Task::getDeadlineString() const {
    if (deadline == 0) {
        return "No deadline";
    }
    
    std::tm* timeinfo = std::localtime(&deadline);
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M");
    return ss.str();
}

std::string Task::getTimeSpentString() const {
    int hours = static_cast<int>(timeSpent / 3600);
    int minutes = static_cast<int>((timeSpent % 3600) / 60);
    int seconds = static_cast<int>(timeSpent % 60);
    
    std::stringstream ss;
    ss << hours << "h " << minutes << "m " << seconds << "s";
    return ss.str();
}

// For serialization
std::string Task::serialize() const {
    std::stringstream ss;
    ss << id << "|" << title << "|" << description << "|" 
       << priority << "|" << deadline << "|" << (completed ? "1" : "0") 
       << "|" << timeSpent;
    
    // Add subtasks
    ss << "|" << subtasks.size();
    for (const auto& subtask : subtasks) {
        ss << "|" << subtask.serialize();
    }
    
    return ss.str();
}

Task Task::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 7) {
        return Task(); // Return default task if data is invalid
    }
    
    int id = std::stoi(tokens[0]);
    std::string title = tokens[1];
    std::string description = tokens[2];
    int priority = std::stoi(tokens[3]);
    std::time_t deadline = std::stoll(tokens[4]);
    bool completed = (tokens[5] == "1");
    std::time_t timeSpent = std::stoll(tokens[6]);
    
    Task task(id, title, description, priority, deadline);
    task.setCompleted(completed);
    task.addTimeSpent(timeSpent);
    
    // Parse subtasks if they exist
    if (tokens.size() > 7) {
        int subtaskCount = std::stoi(tokens[7]);
        int currentIndex = 8;
        
        for (int i = 0; i < subtaskCount && currentIndex < tokens.size(); ++i) {
            // Reconstruct subtask serialized string
            std::string subtaskData = tokens[currentIndex++];
            for (int j = 0; j < 7 && currentIndex < tokens.size(); ++j) {
                subtaskData += "|" + tokens[currentIndex++];
            }
            
            Task subtask = Task::deserialize(subtaskData);
            task.addSubtask(subtask);
        }
    }
    
    return task;
}